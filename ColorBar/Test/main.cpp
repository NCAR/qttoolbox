#include "ColorBarTest.h"

#include <qapplication.h>
#include <qstring.h>
#include <iostream>

int
main(int argc, char** argv)
{

   QApplication app(argc, argv);

   // create our test dialog. It will contain a ColorBar and 
   // other buttons etc.
   ColorBarTest testDialog;

   // if we don't show() the test dialog, nothing appears!
   testDialog.show();

   // This tells qt to stop running when scopeTestDialog
   // closes.
   app.setMainWidget(&testDialog);

   // run the whole thing
   app.exec();

   return 0;
}
