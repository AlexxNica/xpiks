/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2015 Taras Kushnir <kushnirTV@gmail.com>
 *
 * Xpiks is distributed under the GNU General Public License, version 3.0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "spellchecksuggestionmodel.h"
#include <QQmlEngine>
#include <QDebug>
#include <QHash>
#include <QString>
#include "spellsuggestionsitem.h"
#include "../Models/artworkmetadata.h"
#include "spellcheckerservice.h"
#include "ispellcheckable.h"
#include "../Commands/commandmanager.h"
#include "../Common/flags.h"
#include "../Common/defines.h"

namespace SpellCheck {

    QVector<SpellSuggestionsItem *> combineSuggestionRequests(const QVector<SpellSuggestionsItem *> &items) {
        QHash<QString, QVector<SpellSuggestionsItem*> > dict;

        int size = items.size();
        for (int i = 0; i < size; ++i) {
            SpellSuggestionsItem *item = items.at(i);
            const QString &word = item->getWord();
            if (!dict.contains(word)) {
                dict.insert(word, QVector<SpellSuggestionsItem*>());
            }

            dict[word].append(item);
        }

        QVector<SpellSuggestionsItem *> result;
        result.reserve(size);

        QHashIterator<QString, QVector<SpellSuggestionsItem*> > i(dict);
        while (i.hasNext()) {
            i.next();

            const QVector<SpellSuggestionsItem*> &vector = i.value();

            if (vector.size() > 1) {
                result.append(new CombinedSpellSuggestions(i.key(), vector));
            } else {
                result.append(vector.first());
            }
        }

        return result;
    }

    SpellCheckSuggestionModel::SpellCheckSuggestionModel():
        QAbstractListModel(),
        Common::BaseEntity(),
        m_ItemIndex(-1)
    {
    }

    SpellCheckSuggestionModel::~SpellCheckSuggestionModel() {
        qDeleteAll(m_SuggestionsList);
    }

    QObject *SpellCheckSuggestionModel::getSuggestionItself(int index) const {
        SpellSuggestionsItem *item = NULL;

        if (0 <= index && index < m_SuggestionsList.length()) {
            item = m_SuggestionsList.at(index);
            QQmlEngine::setObjectOwnership(item, QQmlEngine::CppOwnership);
        }

        return item;
    }

    void SpellCheckSuggestionModel::clearModel() {
        beginResetModel();
        qDeleteAll(m_SuggestionsList);
        m_SuggestionsList.clear();
        endResetModel();
        m_ItemIndex = -1;
    }

    void SpellCheckSuggestionModel::submitCorrections() const {
        bool anyChanged = false;

        foreach (SpellSuggestionsItem *item, m_SuggestionsList) {
            if (item->anyReplacementSelected()) {
                item->replaceToSuggested(m_CurrentItem);
                anyChanged = true;
            }
        }

        m_CurrentItem->afterReplaceCallback();

        if (m_ItemIndex != -1) {
            m_CommandManager->updateArtworks(QVector<int>() << m_ItemIndex);
        }

        if (anyChanged) {
            m_CommandManager->submitItemForSpellCheck(m_CurrentItem);
        }
    }

    void SpellCheckSuggestionModel::resetAllSuggestions() {
        int size = m_SuggestionsList.length();

        for (int i = 0; i < size; ++i) {
            SpellSuggestionsItem *item = m_SuggestionsList.at(i);
            item->setReplacementIndex(-1);
        }
    }

    void SpellCheckSuggestionModel::setupModel(SpellCheck::ISpellCheckable *item, int index, int flags) {
        Q_ASSERT(item != NULL);
        qInfo() << "Setting suggestions with flags" << flags;
        QVector<SpellSuggestionsItem*> requests;

        if (Common::HasFlag(flags, Common::CorrectKeywords)) {
            QVector<SpellSuggestionsItem*> keywordsSuggestionsRequests = item->createKeywordsSuggestionsList();
            requests << keywordsSuggestionsRequests;
        }

        if (Common::HasFlag(flags, Common::CorrectTitle)) {
            QVector<SpellSuggestionsItem*> titleSuggestionsRequests = item->createTitleSuggestionsList();
            requests << titleSuggestionsRequests;
        }

        if (Common::HasFlag(flags, Common::CorrectKeywords)) {
            QVector<SpellSuggestionsItem*> descriptionSuggestionsRequests = item->createDescriptionSuggestionsList();
            requests << descriptionSuggestionsRequests;
        }

        QVector<SpellSuggestionsItem*> combinedRequests = combineSuggestionRequests(requests);
        QVector<SpellSuggestionsItem*> executedRequests = setupSuggestions(combinedRequests);

        beginResetModel();
        m_CurrentItem = item;
        qDeleteAll(m_SuggestionsList);
        m_SuggestionsList.clear();
        m_SuggestionsList << executedRequests;
        endResetModel();

        m_ItemIndex = index;
    }

    QVector<SpellSuggestionsItem *> SpellCheckSuggestionModel::setupSuggestions(const QVector<SpellSuggestionsItem *> &items) {
        qDebug() << "Suggesting corrections for" << items.length() << "item(s)";
        SpellCheckerService *service = m_CommandManager->getSpellCheckerService();
        // another vector for requests with available suggestions
        QVector<SpellSuggestionsItem*> executedRequests;
        executedRequests.reserve(items.length());

        foreach (SpellSuggestionsItem* item, items) {
            QStringList suggestions = service->suggestCorrections(item->getWord());
            if (!suggestions.isEmpty()) {
                item->setSuggestions(suggestions);
                executedRequests.append(item);
            } else {
                delete item;
            }
        }

        return executedRequests;
    }

    int SpellCheckSuggestionModel::rowCount(const QModelIndex &parent) const {
        Q_UNUSED(parent);
        return m_SuggestionsList.length();
    }

    QVariant SpellCheckSuggestionModel::data(const QModelIndex &index, int role) const {
        int row = index.row();
        if (row < 0 || row >= m_SuggestionsList.length()) { return QVariant(); }

        SpellSuggestionsItem *item = m_SuggestionsList.at(row);

        switch (role) {
        case WordRole:
            return item->getWord();
        case ReplacementIndexRole:
            return item->getReplacementIndex();
        case ReplacementOriginRole:
            return item->getReplacementOrigin();
        default:
            return QVariant();
        }
    }

    QHash<int, QByteArray> SpellCheckSuggestionModel::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[WordRole] = "word";
        roles[ReplacementIndexRole] = "replacementindex";
        roles[ReplacementOriginRole] = "replacementorigin";
        return roles;
    }
}