/*
 * FakeDataThread.cpp
 *
 *  Created on: Aug 7, 2009
 *      Author: burghart
 */
#include "FakeDataThread.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <QDateTime>

static int MsecInterval = 50;   // ray interval in ms

FakeDataThread::FakeDataThread(QObject *parent) : QThread(parent) {
}

FakeDataThread::~FakeDataThread() {
    quit();
    wait();
}

void
FakeDataThread::run() {
    QTimer rayTimer;
    QObject::connect(&rayTimer, SIGNAL(timeout()), this, SLOT(makeRay()));
    rayTimer.start(MsecInterval);
    exec();
}

void
FakeDataThread::makeRay() {
    static unsigned int rayCount = 0;
    static double dwellTime = 0.001 * MsecInterval;   // dwell time in seconds
    static double startTime = 0;
    if (! startTime) {
    	QDateTime now = QDateTime::currentDateTime().toUTC();
        startTime = now.toTime_t() + 0.001 * now.time().msec();
    }
    double time = startTime + (dwellTime * rayCount++);
    std::vector<float> gateVals(1000);
    for (unsigned int g = 0; g < 1000; g++)
        gateVals[g] = fmod(time + g, 256);
    emit newRay(gateVals, time, dwellTime);
}
