#include "ui_ColorBarTest.h"
#include <qapplication.h>

#include <qstring.h>
#include <ColorBar.h>
#include <iostream>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include "ColorBarTest.h"

int
main(int argc, char** argv)
{

	QApplication app(argc, argv);
	QDialog* dialog = new QDialog;

	// create our test dialog. It will contain a ColorBar and 
	// other buttons etc.
	ColorBarTest t(dialog);
	// if we don't show() the test dialog, nothing appears!
	dialog->show();

	// run the whole thing
	app.exec();

	return 0;
}
