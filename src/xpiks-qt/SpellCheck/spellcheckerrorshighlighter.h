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

#ifndef SPELLCHECKERRORSHIGHLIGHTER_H
#define SPELLCHECKERRORSHIGHLIGHTER_H

#include <QtGui>
#include <QSet>
#include <QString>

namespace QMLExtensions {
    class ColorsModel;
}

namespace SpellCheck {
    class SpellCheckErrorsInfo;

    class SpellCheckErrorsHighlighter : public QSyntaxHighlighter
    {
        Q_OBJECT
    public:
        SpellCheckErrorsHighlighter(QTextDocument *document, QMLExtensions::ColorsModel *colorsModel,
                                    SpellCheckErrorsInfo *errorsInfo);

    protected:
        virtual void highlightBlock(const QString &text) override;

    private:
        SpellCheckErrorsInfo *m_SpellCheckErrors;
        QMLExtensions::ColorsModel *m_ColorsModel;
    };
}

#endif // SPELLCHECKERRORSHIGHLIGHTER_H
