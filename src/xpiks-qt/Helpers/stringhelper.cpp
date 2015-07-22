/*
 * This file is a part of Xpiks - cross platform application for
 * keywording and uploading images for microstocks
 * Copyright (C) 2014 Taras Kushnir <kushnirTV@gmail.com>
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

#include "stringhelper.h"
#include <QFile>
#include <QTextStream>
#include <QStringRef>
#include <QVector>
#include <QtGlobal>

namespace Helpers {
#if QT_VERSION >= 0x050400
    QString getLastNLines(const QString &text, int N) {
        QString result;

        QVector<QStringRef> items = text.splitRef(QRegExp("[\r\n]"), QString::SkipEmptyParts);

        int length = items.length();

        if (length > 0) {
            int startIndex = length - N;
            if (startIndex < 0) { startIndex = 0; }
            int pos = items[startIndex].position();
            result = text.right(text.length() - pos);
        } else {
            result = text;
        }

        return result;
    }
#else
    QString getLastNLines(const QString &text, int N) {
        QString result;
        QStringList lastNLines;

        QStringList items = text.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);

        int length = items.length();

        if (length > 0) {
            int startIndex = length - N;
            if (startIndex < 0) { startIndex = 0; }

            for (int pos = startIndex; pos < length; pos++) {
                lastNLines.append(items[pos]);
            }

            result = lastNLines.join("\n");
        } else {
            result = text;
        }

         return result;
    }
#endif
}


