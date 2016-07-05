/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2016 Taras Kushnir <kushnirTV@gmail.com>
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

#include "combinedartworksmodel.h"
#include "../Helpers/indiceshelper.h"
#include "../Commands/combinededitcommand.h"
#include "../Commands/commandmanager.h"
#include "../Commands/commandbase.h"
#include "../Suggestion/keywordssuggestor.h"
#include "artworkmetadata.h"
#include "metadataelement.h"
#include "../SpellCheck/spellcheckiteminfo.h"
#include "../Common/defines.h"
#include "../QMLExtensions/colorsmodel.h"

namespace Models {
    CombinedArtworksModel::CombinedArtworksModel(QObject *parent) :
        AbstractListModel(parent),
        Common::BaseEntity(),
        m_CommonKeywordsModel(m_HoldPlaceholder, this),
        m_EditFlags(0),
        m_AreKeywordsModified(false),
        m_IsDescriptionModified(false),
        m_IsTitleModified(false)
    {
        QObject::connect(&m_CommonKeywordsModel, SIGNAL(spellCheckErrorsChanged()),
                         this, SLOT(spellCheckErrorsChangedHandler()));

        QObject::connect(&m_CommonKeywordsModel, SIGNAL(completionsAvailable()),
                         this, SIGNAL(completionsAvailable()));
    }

    void CombinedArtworksModel::initArtworks(std::vector<MetadataElement> &artworks) {
        size_t paramLength = artworks.size();

        if (paramLength > 0) {
            beginResetModel();
            m_ArtworksList = std::move(artworks);
            endResetModel();
        }

        if (paramLength == 1) {
            enableAllFields();
        }
    }

    void CombinedArtworksModel::recombineArtworks() {
        LOG_DEBUG << m_ArtworksList.size() << "artwork(s)";
        if (m_ArtworksList.empty()) { return; }

        if (m_ArtworksList.size() == 1) {
            assignFromOneArtwork();
        } else {
            assignFromManyArtworks();
        }

        if (m_CommonKeywordsModel.getSpellCheckInfo() == NULL) {
            m_CommonKeywordsModel.setSpellCheckInfo(&m_SpellCheckInfo);
        }

        m_CommandManager->submitItemForSpellCheck(&m_CommonKeywordsModel);
    }

    void CombinedArtworksModel::acceptSuggestedKeywords(const QStringList &keywords)  {
        foreach (const QString &keyword, keywords) {
            this->appendKeyword(keyword);
        }
    }

    void CombinedArtworksModel::setChangeDescription(bool value)  {
        if (Common::HasFlag(m_EditFlags, Common::EditDesctiption) != value) {
            Common::ApplyFlag(m_EditFlags, value, Common::EditDesctiption);
            emit changeDescriptionChanged();
        }
    }

    void CombinedArtworksModel::setChangeTitle(bool value) {
        if (Common::HasFlag(m_EditFlags, Common::EditTitle) != value) {
            Common::ApplyFlag(m_EditFlags, value, Common::EditTitle);
            emit changeTitleChanged();
        }
    }

    void CombinedArtworksModel::setChangeKeywords(bool value) {
        if (Common::HasFlag(m_EditFlags, Common::EditKeywords) != value) {
            Common::ApplyFlag(m_EditFlags, value, Common::EditKeywords);
            emit changeKeywordsChanged();
        }
    }

    void CombinedArtworksModel::setAppendKeywords(bool value) {
        if (Common::HasFlag(m_EditFlags, Common::AppendKeywords) != value) {
            Common::ApplyFlag(m_EditFlags, value, Common::AppendKeywords);
            emit appendKeywordsChanged();
        }
    }

    int CombinedArtworksModel::getSelectedArtworksCount() const {
        int selectedCount = 0;
        size_t count = m_ArtworksList.size();
        for (size_t i = 0; i < count; ++i) {
            if (m_ArtworksList.at(i).isSelected()) {
                selectedCount++;
            }
        }

        return selectedCount;
    }

#ifdef CORE_TESTS
        QStringList CombinedArtworksModel::getKeywords() {
            return m_CommonKeywordsModel.getKeywords();
        }
#endif

    void CombinedArtworksModel::editKeyword(int index, const QString &replacement) {
        if (m_CommonKeywordsModel.editKeyword(index, replacement)) {
            m_AreKeywordsModified = true;
            m_CommandManager->submitKeywordForSpellCheck(&m_CommonKeywordsModel, index);
        }
    }

    QString CombinedArtworksModel::removeKeywordAt(int keywordIndex) {
        QString keyword;
        if (m_CommonKeywordsModel.takeKeywordAt(keywordIndex, keyword)) {
            emit keywordsCountChanged();
            m_AreKeywordsModified = true;
        }

        return keyword;
    }

    void CombinedArtworksModel::removeLastKeyword() {
        QString keyword;
        if (m_CommonKeywordsModel.takeLastKeyword(keyword)) {
            emit keywordsCountChanged();
            m_AreKeywordsModified = true;
        }
    }

    void CombinedArtworksModel::appendKeyword(const QString &keyword) {
        if (m_CommonKeywordsModel.appendKeyword(keyword)) {
            emit keywordsCountChanged();
            m_AreKeywordsModified = true;

            m_CommandManager->submitKeywordForSpellCheck(&m_CommonKeywordsModel, m_CommonKeywordsModel.rowCount() - 1);
        }
    }

    void CombinedArtworksModel::pasteKeywords(const QStringList &keywords) {
        LOG_DEBUG << keywords.length() << "keyword(s)";
        if (m_CommonKeywordsModel.appendKeywords(keywords) > 0) {
            emit keywordsCountChanged();
            m_AreKeywordsModified = true;

            m_CommandManager->submitItemForSpellCheck(&m_CommonKeywordsModel, Common::SpellCheckKeywords);
        }
    }

    void CombinedArtworksModel::setArtworksSelected(int index, bool newState) {
        if (index < 0 || index >= (int)m_ArtworksList.size()) {
            return;
        }

        m_ArtworksList.at(index).setSelected(newState);
        QModelIndex qIndex = this->index(index);
        emit dataChanged(qIndex, qIndex, QVector<int>() << IsSelectedRole);
        emit selectedArtworksCountChanged();
    }

    void CombinedArtworksModel::removeSelectedArtworks() {
        int count = (int)m_ArtworksList.size();
        QVector<int> indicesToRemove;
        indicesToRemove.reserve(count);

        for (int i = 0; i < count; ++i) {
            const MetadataElement &item = m_ArtworksList.at(i);
            if (item.isSelected()) {
                indicesToRemove.append(i);
            }
        }

        QVector<QPair<int, int> > rangesToRemove;
        Helpers::indicesToRanges(indicesToRemove, rangesToRemove);
        removeItemsAtIndices(rangesToRemove);

        recombineArtworks();
        emit artworksCountChanged();
    }

    void CombinedArtworksModel::saveEdits() {
        bool needToSave = false;

        if (getChangeTitle() ||
                getChangeDescription() ||
                getChangeKeywords()) {
            needToSave = m_ArtworksList.size() > 1;
            needToSave = needToSave || (getChangeKeywords() && m_AreKeywordsModified);
            needToSave = needToSave || (getChangeTitle() && m_IsTitleModified);
            needToSave = needToSave || (getChangeDescription() && m_IsDescriptionModified);
        }

        if (needToSave) {
            processCombinedEditCommand();
        }
    }

    void CombinedArtworksModel::resetModelData() {
        beginResetModel();
        m_ArtworksList.clear();
        endResetModel();

        m_AreKeywordsModified = false;
        m_IsDescriptionModified = false;
        m_IsTitleModified = false;

        // TEMPORARY (enable everything on initial launch) --
        m_EditFlags = 0;
        enableAllFields();
        // TEMPORARY (enable everything on initial launch) --

        m_CommonKeywordsModel.setSpellCheckInfo(NULL);
        initDescription("");
        initTitle("");
        initKeywords(QStringList());
    }

    void CombinedArtworksModel::clearKeywords() {
        if (m_CommonKeywordsModel.clearKeywords()) {
            m_AreKeywordsModified = true;
        }
    }

    void CombinedArtworksModel::suggestCorrections() {
        m_CommandManager->setupSpellCheckSuggestions(&m_CommonKeywordsModel, -1, Common::CorrectAll);
    }

    void CombinedArtworksModel::initDescriptionHighlighting(QQuickTextDocument *document) {
        SpellCheck::SpellCheckItemInfo *info = m_CommonKeywordsModel.getSpellCheckInfo();
        if (info == NULL) {
            Q_ASSERT(false);
            // OneItem edits will use artwork's spellcheckinfo
            // combined edit will use this one
            info = &m_SpellCheckInfo;
        }

        QMLExtensions::ColorsModel *colorsModel = m_CommandManager->getColorsModel();
        info->createHighlighterForDescription(document->textDocument(), colorsModel, &m_CommonKeywordsModel);
        m_CommonKeywordsModel.notifyDescriptionSpellCheck();
    }

    void CombinedArtworksModel::initTitleHighlighting(QQuickTextDocument *document) {
        SpellCheck::SpellCheckItemInfo *info = m_CommonKeywordsModel.getSpellCheckInfo();
        if (info == NULL) {
            Q_ASSERT(false);
            // OneItem edits will use artwork's spellcheckinfo
            // combined edit will use this one
            info = &m_SpellCheckInfo;
        }

        QMLExtensions::ColorsModel *colorsModel = m_CommandManager->getColorsModel();
        info->createHighlighterForTitle(document->textDocument(), colorsModel, &m_CommonKeywordsModel);
        m_CommonKeywordsModel.notifyTitleSpellCheck();
    }

    void CombinedArtworksModel::spellCheckDescription() {
        if (!m_CommonKeywordsModel.getDescription().trimmed().isEmpty()) {
            m_CommandManager->submitItemForSpellCheck(&m_CommonKeywordsModel, Common::SpellCheckDescription);
        } else {
            m_CommonKeywordsModel.notifySpellCheckResults(Common::SpellCheckDescription);
        }
    }

    void CombinedArtworksModel::spellCheckTitle() {
        if (!m_CommonKeywordsModel.getTitle().trimmed().isEmpty()) {
            m_CommandManager->submitItemForSpellCheck(&m_CommonKeywordsModel, Common::SpellCheckTitle);
        } else {
            m_CommonKeywordsModel.notifySpellCheckResults(Common::SpellCheckTitle);
        }
    }

    void CombinedArtworksModel::assignFromSelected() {
        int selectedCount = 0;
        size_t count = m_ArtworksList.size();
        int firstSelectedIndex = -1;
        for (size_t i = 0; i < count; ++i) {
            if (m_ArtworksList.at(i).isSelected()) {
                selectedCount++;

                if (firstSelectedIndex == -1) {
                    firstSelectedIndex = (int)i;
                }
            }
        }

        if (selectedCount == 1) {
            Q_ASSERT(firstSelectedIndex != -1);
            LOG_DEBUG << "Assigning fields";
            MetadataElement &firstSelected = m_ArtworksList.at(firstSelectedIndex);
            ArtworkMetadata *metadata = firstSelected.getOrigin();
            setTitle(metadata->getTitle());
            setDescription(metadata->getDescription());
            setKeywords(metadata->getKeywords());
        } else {
            LOG_WARNING << "Method called with" << count << "items selected";
        }
    }

    void CombinedArtworksModel::processCombinedEditCommand() {
        std::shared_ptr<Commands::CombinedEditCommand> combinedEditCommand(new Commands::CombinedEditCommand(
                    m_EditFlags,
                    m_ArtworksList,
                    m_CommonKeywordsModel.getDescription(),
                    m_CommonKeywordsModel.getTitle(),
                    m_CommonKeywordsModel.getKeywords()));

        m_CommandManager->processCommand(combinedEditCommand);
    }

    void CombinedArtworksModel::enableAllFields() {
        setChangeDescription(true);
        setChangeTitle(true);
        setChangeKeywords(true);
    }

    void CombinedArtworksModel::assignFromOneArtwork() {
        Q_ASSERT(m_ArtworksList.size() == 1);
        MetadataElement &info = m_ArtworksList.at(0);
        ArtworkMetadata *metadata = info.getOrigin();

        if (!m_IsDescriptionModified) {
            initDescription(metadata->getDescription());
        }

        if (!m_IsTitleModified) {
            initTitle(metadata->getTitle());
        }

        if (!m_IsDescriptionModified && !m_IsTitleModified) {
            // TODO: would be better to merge errors instead of assignment
            Common::BasicKeywordsModel *keywordsModel = metadata->getKeywordsModel();
            m_CommonKeywordsModel.setSpellCheckInfo(keywordsModel->getSpellCheckInfo());
        }

        if (!m_AreKeywordsModified) {
            initKeywords(metadata->getKeywords());
            Common::BasicKeywordsModel *keywordsModel = metadata->getKeywordsModel();
            m_CommonKeywordsModel.setSpellStatuses(keywordsModel);
        }
    }

    void CombinedArtworksModel::assignFromManyArtworks() {
        bool anyItemsProcessed = false;
        bool descriptionsDiffer = false;
        bool titleDiffer = false;
        QString description;
        QString title;
        QSet<QString> commonKeywords;

        size_t artworksCount = m_ArtworksList.size();
        for (size_t i = 0; i < artworksCount; ++i) {
            MetadataElement &info = m_ArtworksList.at(i);
            ArtworkMetadata *metadata = info.getOrigin();

            if (!anyItemsProcessed) {
                description = metadata->getDescription();
                title = metadata->getTitle();
                commonKeywords.unite(metadata->getKeywordsSet());
                anyItemsProcessed = true;
                continue;
            }

            QString currDescription = metadata->getDescription();
            QString currTitle = metadata->getTitle();
            descriptionsDiffer = descriptionsDiffer || description != currDescription;
            titleDiffer = titleDiffer || title != currTitle;
            commonKeywords.intersect(metadata->getKeywordsSet());
        }

        if (artworksCount > 0) {
            if (descriptionsDiffer) {
                description = "";
            }

            if (titleDiffer) {
                title = "";
            }

            initDescription(description);
            initTitle(title);

            if (!m_AreKeywordsModified) {
                initKeywords(commonKeywords.toList());
            }
        }
    }

    void CombinedArtworksModel::spellCheckErrorsChangedHandler() {
        emit descriptionChanged();
        emit titleChanged();
    }

    int CombinedArtworksModel::rowCount(const QModelIndex &parent) const {
        Q_UNUSED(parent);
        return (int)m_ArtworksList.size();
    }

    QVariant CombinedArtworksModel::data(const QModelIndex &index, int role) const {
        if (index.row() < 0 || index.row() >= (int)m_ArtworksList.size())
            return QVariant();

        const MetadataElement &item = m_ArtworksList.at(index.row());

        switch (role) {
        case PathRole:
            return item.getOrigin()->getFilepath();
        case IsSelectedRole:
            return item.isSelected();
        default:
            return QVariant();
        }
    }

    QHash<int, QByteArray> CombinedArtworksModel::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[PathRole] = "path";
        roles[IsSelectedRole] = "isselected";
        return roles;
    }

    void CombinedArtworksModel::removeInnerItem(int row) {
        m_ArtworksList.erase(m_ArtworksList.begin() + row);
    }

    void CombinedArtworksModel::generateAboutToBeRemoved() {
         m_CommonKeywordsModel.notifyAboutToBeRemoved();
    }

    void CombinedArtworksModel::removeUnavailableItems() {
        LOG_DEBUG << "#";
        QVector<int> indicesToRemove;
        size_t size = m_ArtworksList.size();
        for (size_t i = 0; i < size; i++) {
            MetadataElement &item = m_ArtworksList.at(i);

            if (item.getOrigin()->isUnavailable()) {
                indicesToRemove.append((int)i);
            }
        }

        QVector<QPair<int, int> > rangesToRemove;
        Helpers::indicesToRanges(indicesToRemove, rangesToRemove);

        removeItemsAtIndices(rangesToRemove);
        recombineArtworks();

        if (m_ArtworksList.empty()) {
            emit requestCloseWindow();
        }

        emit itemsNumberChanged();
    }
}
