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
  scope.show();

  PokeScope pokeScope(scope._scopePlot);

  app.setMainWidget(&scope);

  app.exec();

  return 0;
}
