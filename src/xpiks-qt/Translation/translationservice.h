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

#ifndef TRANSLATIONSERVICE_H
#define TRANSLATIONSERVICE_H

#include <QObject>
#include "translationmanager.h"
#include "../Common/iservicebase.h"

namespace Translation {
    class TranslationWorker;

    class TranslationService :
            public QObject,
            public Common::IServiceBase<QString>
    {
        Q_OBJECT
    public:
        explicit TranslationService(TranslationManager &manager, QObject *parent = 0);
        virtual ~TranslationService() {}

        virtual void startService() override;
        virtual void stopService() override;

        virtual bool isAvailable() const override { return true; }
        virtual bool isBusy() const override;

    protected:
        virtual void submitItem(QString *item) override;
        virtual void submitItem(QString *item, int flags) override;
        virtual void submitItems(const QVector<QString*> &items) override;

    public:
        void selectDictionary(const QString &dictionaryPath);
        void translate(const QString &what);

    private slots:
        void workerFinished();
        void workerDestroyed(QObject* object);

    private:
        TranslationManager &m_TranslationManager;
        TranslationWorker *m_TranslationWorker;
        volatile bool m_RestartRequired;
    };
}

#endif // TRANSLATIONSERVICE_H
