#include "ScopePlotTest.h"
#include "ScopePoke.h"

#include <qapplication.h>
#include <qstring.h>
#include <iostream>
#include <qtimer.h>
#include <qcheckbox.h>
#include <qcheckbox.h>


int
main(int argc, char** argv)
{

   QApplication app(argc, argv);

   // create our test dialog. It will contain a ScopePlot and 
   // other buttons etc.
   ScopePlotTest scopeTestDialog;

   // Create the test data generator, telling it
   // what ScopePlot to send data to. ScopePoke has
   // a timer which peridically sends synthesized data
   // to the ScopePlot.
   ScopePoke scopePoke(scopeTestDialog._scopePlot);

   // Tell our tesst dialog about scopePoke, so that it
   // can call scopePoke in order to change the type of 
   // plot data being generated and sent to the ScopePlot
   scopeTestDialog.setScopePoke(&scopePoke);

   // connect the grid select and pause buttons
   QObject::connect(scopeTestDialog.xGrid, SIGNAL(toggled(bool)),
        scopeTestDialog._scopePlot, SLOT(enableXgrid(bool)));
   QObject::connect(scopeTestDialog.yGrid, SIGNAL(toggled(bool)),
        scopeTestDialog._scopePlot, SLOT(enableYgrid(bool)));
   QObject::connect(scopeTestDialog.pauseButton, SIGNAL(toggled(bool)),
        scopeTestDialog._scopePlot, SLOT(pause(bool)));

   // if we don't show() the test dialog, nothing appears!
   scopeTestDialog.show();

   // This tells qt to stop running when scopeTestDialog
   // closes.
   app.setMainWidget(&scopeTestDialog);

   // run the whole thing
   app.exec();

   return 0;
}
