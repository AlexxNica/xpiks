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

#ifndef BACKUPSAVERSERVICE_H
#define BACKUPSAVERSERVICE_H

#include <QObject>
#include <QVector>

namespace Models {
    class ArtworkMetadata;
}

namespace MetadataIO {
    class BackupSaverWorker;

    class BackupSaverService : public QObject
    {
        Q_OBJECT
    public:
        BackupSaverService();

    public:
        void startSaving();
        void stopSaving();
        void saveArtwork(Models::ArtworkMetadata *metadata) const;
        void saveArtworks(const QVector<Models::ArtworkMetadata *> &artworks) const;
        void readArtwork(Models::ArtworkMetadata *metadata) const;
        void readArtworks(const QVector<Models::ArtworkMetadata *> &artworks) const;

    signals:
        void cancelSaving();

    private slots:
        void workerFinished();

    private:
        BackupSaverWorker *m_BackupWorker;
    };
}

#endif // BACKUPSAVERSERVICE_H
