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

#include "asynccoordinator.h"

namespace Helpers {
    AsyncCoordinator::AsyncCoordinator():
        QObject(),
        m_OpCount(1),
        m_StatusReported(0)
    {
        m_Timer.setSingleShot(true);
        QObject::connect(&m_Timer, &QTimer::timeout, this, &AsyncCoordinator::onTimeout);
    }

    void AsyncCoordinator::allBegun(int timeoutSeconds) {
        m_Timer.start(timeoutSeconds*1000);
        justEnded();
    }

    void AsyncCoordinator::aboutToBegin(int operationsToAdd) {
        m_OpCount.fetchAndAddOrdered(operationsToAdd);
    }

    void AsyncCoordinator::justEnded() {
        if (1 == m_OpCount.fetchAndSubOrdered(1)) {
            reportStatus(AllDone);
        }
    }

    void AsyncCoordinator::cancel() {
        reportStatus(Cancel);
    }

    void AsyncCoordinator::onTimeout() {
        reportStatus(Timeout);
    }

    void AsyncCoordinator::reportStatus(AsyncCoordinator::CoordinationStatus status) {
        if (0 == m_StatusReported.fetchAndStoreOrdered(1)) {
            emit statusReported((int)status);
        }
    }
}