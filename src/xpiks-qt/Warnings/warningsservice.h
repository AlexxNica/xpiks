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

#ifndef WARNINGSSERVICE_H
#define WARNINGSSERVICE_H

#include <QObject>
#include "../Common/baseentity.h"
#include "../Common/iservicebase.h"
#include "../Models/artworkmetadata.h"
#include "../Common/flags.h"
#include "warningssettingsmodel.h"

namespace Warnings {
    class WarningsCheckingWorker;

    class WarningsService:
        public QObject,
        public Common::BaseEntity,
        public Common::IServiceBase<Models::ArtworkMetadata, Common::WarningsCheckFlags>
    {
    Q_OBJECT

    public:
        explicit WarningsService(QObject *parent=0);
        virtual ~WarningsService() {}

    public:
        void initWarningsSettings();
        const WarningsSettingsModel *getWarningsSettingsModel() const { return &m_WarningsSettingsModel; }
        void cancelCurrentBatch();

    public:
        virtual void startService(const std::shared_ptr<Common::ServiceStartParams> &params) override;
        virtual void stopService() override;

        virtual bool isAvailable() const override { return true; }
        virtual bool isBusy() const override;

        virtual void submitItem(Models::ArtworkMetadata *item) override;
        virtual void submitItem(Models::ArtworkMetadata *item, Common::WarningsCheckFlags flags) override;
        virtual void submitItems(const QVector<Models::ArtworkMetadata *> &items) override;
        virtual void setCommandManager(Commands::CommandManager *commandManager) override;

    private slots:
        void workerDestoyed(QObject *object);
        void workerStopped();
        void updateWarningsSettings();

    signals:
        void queueIsEmpty();

    private:
        WarningsCheckingWorker *m_WarningsWorker;
        WarningsSettingsModel m_WarningsSettingsModel;
    };
}

#endif // WARNINGSSERVICE_H
