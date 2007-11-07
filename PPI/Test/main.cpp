#include <qapplication.h>
#include "PPITest.h"
#include <stdlib.h>
#include "qdialog.h"

int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	int nVars = 6;

	if (argc > 1)
		nVars = atoi(argv[1]);

	QDialog* dialog = new QDialog;

	PPITest t(nVars, dialog);

	dialog->show();

	app.exec();

	return 0;
}

