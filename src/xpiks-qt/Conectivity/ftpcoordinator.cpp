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

#include "ftpcoordinator.h"
#include <QStringList>
#include <QSharedData>
#include <QThread>
#include "../Models/artworkmetadata.h"
#include "../Models/uploadinfo.h"
#include "../Helpers/filenameshelpers.h"
#include "../Encryption/secretsmanager.h"
#include "../Commands/commandmanager.h"
#include "curlftpuploader.h"
#include "uploadcontext.h"
#include "ftpuploaderworker.h"

#include <curl/curl.h>

#define TIMEOUT_SECONDS 10
#define RETRIES_COUNT 3

namespace Conectivity {
    void extractFilePathes(const QVector<Models::ArtworkMetadata *> &artworkList,
                           QStringList &filePathes,
                           QStringList &zipsPathes) {

        int size = artworkList.length();
        filePathes.reserve(size);
        zipsPathes.reserve(size);
        qDebug() << "extractFilePathes #" << "Generating filepathes for" << size << "item(s)";

        for (int i = 0; i < size; ++i) {
            Models::ArtworkMetadata *metadata = artworkList.at(i);
            QString filepath = metadata->getFilepath();
            filePathes.append(filepath);

            if (metadata->hasVectorAttached()) {
                filePathes.append(metadata->getAttachedVectorPath());

                QString zipPath = Helpers::getArchivePath(filepath);
                zipsPathes.append(zipPath);
            } else {
                zipsPathes.append(filepath);
            }
        }
    }

    void generateUploadContexts(const QVector<Models::UploadInfo *> &uploadInfos,
                                QVector<QSharedPointer<UploadContext> > &contexts,
                                Encryption::SecretsManager *secretsManager,
                                int timeoutSeconds) {
        int size = uploadInfos.size();
        contexts.reserve(size);

        for (int i = 0; i < size; ++i) {
            UploadContext *context = new UploadContext();
            Models::UploadInfo *info = uploadInfos.at(i);

            context->m_Host = info->getHost();
            context->m_Username = info->getUsername();
            context->m_Password = secretsManager->decodePassword(info->getPassword());
            context->m_UsePassiveMode = !info->getDisableFtpPassiveMode();

            // TODO: move to configs/options
            context->m_TimeoutSeconds = timeoutSeconds;
            context->m_RetriesCount = RETRIES_COUNT;

            if (context->m_Host.contains("dreamstime")) {
                context->m_DirForVectors = "additional";
            }

            contexts.append(QSharedPointer<UploadContext>(context));
        }
    }

    QVector<UploadBatch*> generateUploadBatches(const QVector<Models::ArtworkMetadata *> &artworksToUpload,
                                                const QVector<Models::UploadInfo *> &uploadInfos,
                                                Encryption::SecretsManager *secretsManager,
                                                int timeoutSeconds) {
        qDebug() << "generateUploadBatches #" << artworksToUpload.length() << "file(s)";
        QVector<UploadBatch*> batches;

        QStringList filePathes;
        QStringList zipFilePathes;
        extractFilePathes(artworksToUpload, filePathes, zipFilePathes);

        QVector<QSharedPointer<UploadContext> > contexts;
        generateUploadContexts(uploadInfos, contexts, secretsManager, timeoutSeconds);

        int size = contexts.size();
        batches.reserve(size);

        for (int i = 0; i < size; ++i) {
            UploadBatch *batch;
            const QSharedPointer<UploadContext> &context = contexts.at(i);

            if (uploadInfos[i]->getZipBeforeUpload()) {
                batch = new UploadBatch(context, zipFilePathes);
            } else {
                batch = new UploadBatch(context, filePathes);
            }

            batches.append(batch);
        }

        return batches;
    }

    FtpCoordinator::FtpCoordinator(int maxParallelUploads, int secondsTimeout, QObject *parent) :
        QObject(parent),
        m_UploadSemaphore(maxParallelUploads),
        m_MaxParallelUploads(maxParallelUploads),
        m_SecondsTimeout(secondsTimeout),
        m_OverallProgress(0.0),
        m_FinishedWorkersCount(0),
        m_AnyFailed(false)
    {
    }

    void FtpCoordinator::uploadArtworks(const QVector<Models::ArtworkMetadata *> &artworksToUpload,
                                        const QVector<Models::UploadInfo *> &uploadInfos) {
        qInfo() << "FtpCoordinator::uploadArtworks #" << "Trying to upload" << artworksToUpload.size() <<
                   "file(s) to" << uploadInfos.size() << "host(s)";


        if (artworksToUpload.isEmpty() || uploadInfos.isEmpty()) {
            qWarning() << "FtpCoordinator::uploadArtworks #" << "Nothing or nowhere to upload. Skipping...";
            return;
        }

        Encryption::SecretsManager *secretsManager = m_CommandManager->getSecretsManager();
        QVector<UploadBatch*> batches = generateUploadBatches(artworksToUpload,
                                                              uploadInfos,
                                                              secretsManager,
                                                              m_SecondsTimeout);

        Q_ASSERT(batches.size() == uploadInfos.size());

        int size = batches.size();

        initUpload(size);
        emit uploadStarted();

        for (int i = 0; i < size; ++i) {
            FtpUploaderWorker *worker = new FtpUploaderWorker(&m_UploadSemaphore,
                                                              batches.at(i), uploadInfos.at(i));
            QThread *thread = new QThread();
            worker->moveToThread(thread);
            QObject::connect(thread, SIGNAL(started()), worker, SLOT(process()));
            QObject::connect(worker, SIGNAL(stopped()), thread, SLOT(quit()));

            QObject::connect(worker, SIGNAL(stopped()), worker, SLOT(deleteLater()));
            QObject::connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

            //QObject::connect(worker, SIGNAL(uploadStarted()), this, SIGNAL(uploadStarted()));
            QObject::connect(worker, SIGNAL(uploadFinished(bool)), this, SLOT(workerFinished(bool)));
            QObject::connect(this, SIGNAL(cancelAll()), worker, SIGNAL(workerCancelled()));
            QObject::connect(worker, SIGNAL(progressChanged(double,double)),
                             this, SLOT(workerProgressChanged(double,double)));
            QObject::connect(worker, SIGNAL(transferFailed(QString, QString)),
                             this, SLOT(transferFailed(QString, QString)));

            thread->start();
        }
    }

    void FtpCoordinator::cancelUpload() {
        qDebug() << "FtpCoordinator::cancelUpload #";
        emit cancelAll();
    }

    void FtpCoordinator::transferFailed(const QString &filepath, const QString &host) {
        qWarning() << "FtpCoordinator::transferFailed #" << "Upload failed for file [" << filepath << "] to host {" << host << "}";
        // TODO: show failed transfers on the UI
    }

    void FtpCoordinator::workerProgressChanged(double oldPercents, double newPercents) {
        Q_ASSERT(m_AllWorkersCount > 0);
        double change = (newPercents - oldPercents) / m_AllWorkersCount;
        m_OverallProgress += change;
        emit overallProgressChanged(m_OverallProgress);
    }

    void FtpCoordinator::workerFinished(bool anyErrors) {
        qDebug() << "FtpCoordinator::workerFinished #" << "anyErrors =" << anyErrors;

        if (anyErrors) {
            m_AnyFailed = true;
        }

        int workersDone = m_FinishedWorkersCount.fetchAndAddOrdered(1) + 1;

        if (workersDone == m_AllWorkersCount) {
            finalizeUpload();
            emit uploadFinished(m_AnyFailed);
            emit overallProgressChanged(100.0);
        }
    }

    void FtpCoordinator::initUpload(int uploadBatchesCount) {
        m_AnyFailed = false;
        m_AllWorkersCount = uploadBatchesCount;
        m_FinishedWorkersCount = 0;
        m_OverallProgress = 0.0;
        qDebug() << "FtpCoordinator::initUpload #" << "Initializing CURL";

        curl_global_init(CURL_GLOBAL_ALL);
    }

    void FtpCoordinator::finalizeUpload() {
        Q_ASSERT(m_FinishedWorkersCount == m_AllWorkersCount);
        curl_global_cleanup();
        qDebug() << "FtpCoordinator::finalizeUpload #" << "Cleaning up CURL";
    }
}