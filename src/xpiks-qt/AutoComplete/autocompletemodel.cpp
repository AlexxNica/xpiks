/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2017 Taras Kushnir <kushnirTV@gmail.com>
 *
 * Xpiks is distributed under the GNU Lesser General Public License, version 3.0
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "autocompletemodel.h"
#include "../Common/defines.h"
#include "../Commands/commandmanager.h"
#include "../KeywordsPresets/presetkeywordsmodel.h"

namespace AutoComplete {
    AutoCompleteModel::AutoCompleteModel(QObject *parent):
        QAbstractListModel(parent),
        m_SelectedIndex(-1),
        m_IsActive(false)
    {
    }

    void AutoCompleteModel::setCompletions(std::vector<std::shared_ptr<CompletionItem> > &completions) {
        Q_ASSERT(!completions.empty());
        LOG_INFO << completions.size() << "completions";
        m_LastGeneratedCompletions = std::move(completions);
    }

    bool AutoCompleteModel::moveSelectionUp() {
        const bool canMove = m_SelectedIndex > 0;
        LOG_INTEGR_TESTS_OR_DEBUG << "can move:" << canMove;
        if (canMove) {
            setSelectedIndex(m_SelectedIndex - 1);
        }
        return canMove;
    }

    bool AutoCompleteModel::moveSelectionDown() {
        const int size = (int)m_CompletionList.size();
        const bool canMove = m_SelectedIndex < size - 1;
        LOG_INTEGR_TESTS_OR_DEBUG << "can move:" << canMove;
        if (canMove) {
            setSelectedIndex(m_SelectedIndex + 1);
        }
        return canMove;
    }

    void AutoCompleteModel::acceptSelected(bool tryExpandPreset) {
        LOG_DEBUG << "Selected index:" << m_SelectedIndex << "expand preset" << tryExpandPreset;
        const int size = (int)m_CompletionList.size();

        if (0 <= m_SelectedIndex && m_SelectedIndex < size) {
            auto &item = m_CompletionList.at(m_SelectedIndex);
            auto &completion = item->getCompletion();
            emit completionAccepted(completion, tryExpandPreset);
        }

        emit dismissPopupRequested();

        setIsActive(false);
    }

    void AutoCompleteModel::completionsArrived() {
        LOG_DEBUG << "Updating completions...";

        beginResetModel();
        {
            m_CompletionList.swap(m_LastGeneratedCompletions);
            m_LastGeneratedCompletions.clear();
        }
        endResetModel();

        setSelectedIndex(-1);
        setIsActive(true);

#ifdef INTEGRATION_TESTS
        emit completionsUpdated();
#endif
    }

    QVariant AutoCompleteModel::data(const QModelIndex &index, int role) const {
        int row = index.row();
        if (row < 0 || row >= (int)m_CompletionList.size()) return QVariant();

        auto &item = m_CompletionList[row];
        if (role == Qt::DisplayRole) { return item->getCompletion(); }
        else if (role == IsPresetRole) { return item->isPreset(); }
        return QVariant();
    }

    QHash<int, QByteArray> AutoCompleteModel::roleNames() const {
        QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
        roles[IsPresetRole] = "ispreset";
        return roles;
    }
}
