/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014-2017 Taras Kushnir <kushnirTV@gmail.com>
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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <vector>
#include <memory>
#include <QHash>
#include <QDir>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include "../Common/baseentity.h"
#include "uiprovider.h"

namespace Plugins {
    class XpiksPluginInterface;
    class PluginWrapper;

    class PluginManager : public QAbstractListModel, public Common::BaseEntity
    {
        Q_OBJECT
    public:
        PluginManager();
        virtual ~PluginManager();

    public:
        enum UploadInfoRepository_Roles {
            PrettyNameRole = Qt::UserRole + 1,
            VersionRole,
            AuthorRole,
            PluginIDRole,
            IsEnabledRole
        };

    private:
        bool initPluginsDir();
        const QString &getPluginsDirectoryPath() const { return m_PluginsDirectoryPath; }
        void processInvalidFile(const QString &filename, const QString &pluginFullPath);

    public:
        void loadPlugins();
        void unloadPlugins();
        bool hasExportedActions(int row) const;
        UIProvider *getUIProvider() { return &m_UIProvider; }

    public slots:
        void onCurrentEditableChanged();
        void onLastActionUndone(int commandID);
        void onPresetsUpdated();

    public:
        Q_INVOKABLE QObject *getPluginActions(int index) const;
        Q_INVOKABLE void triggerPluginAction(int pluginID, int actionID) const;
        Q_INVOKABLE void removePlugin(int index);
        Q_INVOKABLE bool pluginExists(const QUrl &pluginUrl);
        Q_INVOKABLE bool installPlugin(const QUrl &pluginUrl);
        Q_INVOKABLE bool replaceInstallPlugin(const QUrl &pluginUrl);

    private:
        bool addPlugin(const QString &fullpath);
        bool doAddPlugin(const QString &filepath);
        int findPluginIndex(const QString &fullpath) const;
        bool isPluginAdded(const QString &fullpath) const { return findPluginIndex(fullpath) != -1; }
        std::shared_ptr<PluginWrapper> loadPlugin(const QString &filepath);
        std::shared_ptr<PluginWrapper> instantiatePlugin(const QString &filepath, XpiksPluginInterface *plugin);

        // QAbstractItemModel interface
    public:
        virtual int rowCount(const QModelIndex &parent=QModelIndex()) const override;
        virtual QVariant data(const QModelIndex &index, int role) const override;
    protected:
        virtual QHash<int, QByteArray> roleNames() const override;

    private:
        int getNextPluginID() { return m_LastPluginID++; }

    private:
        QString m_PluginsDirectoryPath;
        QString m_FailedPluginsDirectory;
        std::vector<std::shared_ptr<PluginWrapper> > m_PluginsList;
        QHash<int, std::shared_ptr<PluginWrapper> > m_PluginsDict;
        UIProvider m_UIProvider;
        int m_LastPluginID;
    };

    class PluginsWithActionsModel: public QSortFilterProxyModel {
        Q_OBJECT
    public:
        PluginsWithActionsModel(QObject *parent = 0):
            QSortFilterProxyModel(parent)
        {}

    public:
        Q_INVOKABLE int getOriginalIndex(int index);

    protected:
        virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    };
}

#endif // PLUGINMANAGER_H
