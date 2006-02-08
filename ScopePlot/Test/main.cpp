#include "ScopePlot.h"
#include "ScopePlotTest.h"
#include "ScopePoke.h"

#include <qapplication.h>
#include <qstring.h>
#include <iostream>
#include <qtimer.h>


int
main(int argc, char** argv)
{

  QApplication app(argc, argv);

  ScopePlotTest scope;

  ScopePoke scopePoke(scope._scopePlot);

  scope.setScopePoke(&scopePoke);
  scope.show();
  
  app.setMainWidget(&scope);

  app.exec();

  return 0;
}
