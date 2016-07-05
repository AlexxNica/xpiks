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

#ifndef CONECTIVITYHELPERS_H
#define CONECTIVITYHELPERS_H

#include <QStringList>
#include <QVector>
#include <memory>
#include "uploadcontext.h"

namespace Models {
    class ArtworkMetadata;
    class UploadInfo;
    struct ProxySettings;
    class SettingsModel;
}

namespace Encryption {
    class SecretsManager;
}

namespace Conectivity {
    class UploadBatch;

    void extractFilePathes(const QVector<Models::ArtworkMetadata *> &artworkList,
                           QStringList &filePathes,
                           QStringList &zipsPathes);

    void generateUploadContexts(const QVector<Models::UploadInfo *> &uploadInfos,
                                QVector<std::shared_ptr<UploadContext> > &contexts,
                                Encryption::SecretsManager *secretsManager,
                                Models::SettingsModel *settingsModel);

    QVector<UploadBatch*> generateUploadBatches(const QVector<Models::ArtworkMetadata *> &artworksToUpload,
                                                const QVector<Models::UploadInfo *> &uploadInfos,
                                                Encryption::SecretsManager *secretsManager,
                                                Models::SettingsModel *settingsModel);
}

#endif // CONECTIVITYHELPERS_H
