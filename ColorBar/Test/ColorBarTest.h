#ifndef ColorBarTEST_H
#define ColorBarTEST_H

#include <QDialog>
#include <QWidget>
#include <QButtonGroup>
#include "ui_ColorBarTest.h"
#include <vector>
#include <string>

class ColorBarTest: public QWidget, public Ui::ColorBarTest {
	Q_OBJECT
public:
	ColorBarTest(QDialog* dialog);
	virtual ~ColorBarTest();
	public slots:
		void buttonSlot(int);

protected:
	std::vector<std::string> _maps;
	QButtonGroup _bgroup;
};

#endif
