#ifndef STATUSGAUGETEST_H
#define STATUSGAUGETEST_H

#include <QDialog>
#include <QWidget>
#include <QButtonGroup>
#include "ui_StatusGaugeTest.h"
#include <vector>
#include <string>

class StatusGaugeTest: public QWidget, public Ui::StatusGaugeTest {
	Q_OBJECT
public:
	StatusGaugeTest(QDialog* dialog);
	virtual ~StatusGaugeTest();
	public slots:
		void buttonSlot(int);

protected:
};

#endif
