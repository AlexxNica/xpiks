#include "testshelpers.h"
#include <QThread>
#include <QCoreApplication>
#include <QDebug>

void sleepWaitUntil(int seconds, const std::function<bool ()> &condition) {
    int times = 0;
    bool becameTrue = false;

    while (times < seconds) {
        if (!condition()) {
            QCoreApplication::processEvents(QEventLoop::AllEvents);
            QThread::sleep(1);
            times++;
        } else {
            qDebug() << "Condition became true in" << (times + 1) << "try out of" << seconds;
            becameTrue = true;
            break;
        }
    }

    if (!becameTrue) {
        qDebug() << "Condition never was true";
    }
}
