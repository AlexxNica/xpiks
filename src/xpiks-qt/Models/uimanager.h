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

#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <memory>
#include <QSet>
#include <QHash>
#include <QObject>
#include <QString>
#include <QStringList>
#include "../QuickBuffer/icurrenteditable.h"
#include "../Models/metadataelement.h"
#include "../QMLExtensions/tabsmodel.h"

namespace Models {
    class ArtworkMetadata;
    class ArtworkProxyBase;
    class SettingsModel;

    class UIManager: public QObject
    {
        Q_OBJECT
        Q_PROPERTY(bool hasCurrentEditable READ getHasCurrentEditable NOTIFY currentEditableChanged)
        Q_PROPERTY(double keywordHeight READ getKeywordHeight NOTIFY keywordHeightChanged)
    public:
        explicit UIManager(Models::SettingsModel *settingsModel, QObject *parent = 0);

    private:
        int generateNextTabID() { int id = m_TabID++; return id; }

    public:
        void initTabs();
        bool getHasCurrentEditable() const { return m_CurrentEditable.operator bool(); }
        double getKeywordHeight() const;
        std::shared_ptr<QuickBuffer::ICurrentEditable> getCurrentEditable() const { return m_CurrentEditable; }

    public:
        void registerCurrentItem(std::shared_ptr<QuickBuffer::ICurrentEditable> &currentItem);
        QMLExtensions::TabsModel *getTabsModel() { return &m_TabsModel; }
        QMLExtensions::ActiveTabsModel *getActiveTabs() { return &m_ActiveTabs; }
        QMLExtensions::InactiveTabsModel *getInactiveTabs() { return &m_InactiveTabs; }

    public:
        Q_INVOKABLE void clearCurrentItem();
        Q_INVOKABLE QObject *retrieveTabsModel(int tabID);

    public:
        void addSystemTab(const QString tabIconComponent, const QString &tabComponent);
        int addPluginTab(int pluginID, const QString tabIconComponent, const QString &tabComponent, QObject *tabModel);
        bool removePluginTab(int pluginID, int tabID);
        void initializeSystemTabs();

    signals:
        void tabsListChanged();
        void tabsIconsChanged();
        void currentEditableChanged();
        void keywordHeightChanged(double value);

    private:
        Models::SettingsModel *m_SettingsModel;
        QMLExtensions::TabsModel m_TabsModel;
        QMLExtensions::ActiveTabsModel m_ActiveTabs;
        QMLExtensions::InactiveTabsModel m_InactiveTabs;
        std::shared_ptr<QuickBuffer::ICurrentEditable> m_CurrentEditable;
        QHash<int, QSet<int> > m_PluginIDToTabIDs;
        QHash<int, int> m_TabsIDsToIndex;
        QHash<int, QObject*> m_TabIDsToModel;
        volatile int m_TabID;
    };
}

#endif // UIMANAGER_H
