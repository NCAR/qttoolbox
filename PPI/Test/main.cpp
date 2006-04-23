#include <qapplication.h>
#include "PPITest.h"
#include <stdlib.h>


int main( int argc, char** argv )
{
	QApplication app( argc, argv );

  int nVars = 6;

  if (argc > 1)
    nVars = atoi(argv[1]);

	PPITest dialog(nVars, 0, 0, TRUE );
	app.setMainWidget(&dialog);

	dialog.exec();

	return 0;
}

