#ifndef STATUSGAUGETEST_H
#define STATUSGAUGETEST_H

#include <QDialog>
#include <QWidget>
#include <QButtonGroup>
#include "ui_StatusGaugeTest.h"
#include <vector>
#include <string>

class StatusGaugeTest: public QDialog, public Ui::StatusGaugeTest {
	Q_OBJECT
public:
	StatusGaugeTest(QDialog* parent);
	virtual ~StatusGaugeTest();
	
	public slots:
		void releasedSlot();

protected:
};

#endif
