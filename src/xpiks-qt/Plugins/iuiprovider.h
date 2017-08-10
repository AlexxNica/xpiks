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
#ifndef IUIPROVIDER_H
#define IUIPROVIDER_H

#include <QUrl>
#include <QHash>
#include <QObject>
#include <QString>
#include <memory>
#include "../QuickBuffer/icurrenteditable.h"

namespace Plugins {
    class IUIProvider {
    public:
        virtual ~IUIProvider() {}

        virtual void openWindow(const QUrl &rcPath, const QHash<QString, QObject*> &contextModels = QHash<QString, QObject*>()) const = 0;
        virtual int addTab(const QString &tabIconUrl, const QString &tabComponentUrl, QObject *tabModel) const = 0;
        virtual bool removeTab(int tabID) const = 0;
        virtual std::shared_ptr<QuickBuffer::ICurrentEditable> getCurrentEditable() const = 0;
    };
}

#endif // IUIPROVIDER_H
