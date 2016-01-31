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

#ifndef FTPCOORDINATOR_H
#define FTPCOORDINATOR_H

#include <QObject>
#include <QVector>
#include <QSemaphore>
#include "../Common/baseentity.h"
#include <QAtomicInt>
#include <QMutex>

namespace Models {
    class ArtworkMetadata;
    class UploadInfo;
}

namespace Conectivity {
    class UploadContext;

    class FtpCoordinator : public QObject, public Common::BaseEntity {
        Q_OBJECT
    public:
        explicit FtpCoordinator(int maxParallelUploads, QObject *parent = 0);

    public:
        void uploadArtworks(const QVector<Models::ArtworkMetadata *> &artworksToUpload,
                            const QVector<Models::UploadInfo *> &uploadInfos,
                            bool includeVectors);
        void cancelUpload();

    signals:
        void uploadStarted();
        void cancelAll();
        void uploadFinished(bool anyError);
        void overallProgressChanged(double percentDone);

    private slots:
        void transferFailed(const QString &filepath, const QString &host);
        void workerProgressChanged(double oldPercents, double newPercents);
        void workerFinished(bool anyErrors);

    private:
        void initUpload(int uploadBatchesCount);
        void finalizeUpload();

    private:
        QMutex m_WorkerMutex;
        QSemaphore m_UploadSemaphore;
        int m_MaxParallelUploads;
        double m_OverallProgress;
        QAtomicInt m_FinishedWorkersCount;
        volatile int m_AllWorkersCount;
        volatile bool m_AnyFailed;
    };
}

#endif // FTPCOORDINATOR_H