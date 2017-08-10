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

#ifndef ICURRENTEDITABLE_H
#define ICURRENTEDITABLE_H

#include <QString>

namespace QuickBuffer {
    class ICurrentEditable {
    public:
        virtual ~ICurrentEditable() {}

        virtual qint64 getItemID() = 0;

        virtual QString getTitle() = 0;
        virtual QString getDescription() = 0;
        virtual QStringList getKeywords() = 0;

        virtual void setTitle(const QString &title) = 0;
        virtual void setDescription(const QString &description) = 0;
        virtual void setKeywords(const QStringList &keywords) = 0;

        virtual bool appendPreset(int presetIndex) = 0;
        virtual bool expandPreset(int keywordIndex, int presetIndex) = 0;
        virtual bool removePreset(int presetIndex) = 0;

        virtual bool hasKeywords(const QStringList &keywordsList) = 0;

        virtual void spellCheck() = 0;
        virtual void update() = 0;
    };
}

#endif // ICURRENTEDITABLE_H
