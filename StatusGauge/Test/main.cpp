#include "ui_StatusGaugeTest.h"
#include <qapplication.h>

#include <qstring.h>
#include <StatusGauge.h>
#include <iostream>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include "StatusGaugeTest.h"

int
main(int argc, char** argv)
{

	QApplication app(argc, argv);
	
    QDialog* dialog = new QDialog;

    // create our test dialog. It will contain a StatusGauge and 
	// other buttons etc.
	StatusGaugeTest t(dialog);
	
	// if we don't show() the test dialog, nothing appears!
	dialog->show();

	// run the whole thing
	app.exec();

	return 0;
}
