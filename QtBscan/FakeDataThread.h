/*
 * FakeDataThread.h
 *
 *  Created on: Aug 7, 2009
 *      Author: burghart
 */

#ifndef FAKEDATATHREAD_H_
#define FAKEDATATHREAD_H_

#include <QtCore/QMutex>
#include <QtCore/QThread>
#include <QtCore/QTimer>
#include <vector>

class FakeDataThread : public QThread {
    Q_OBJECT
public:
    FakeDataThread(QObject *parent = 0);
    virtual ~FakeDataThread();
    virtual void run();
private slots:
    void makeRay();
signals:
    void newRay(const std::vector<float>& gateVals, double time, double period);
};
#endif /* FAKEDATATHREAD_H_ */
