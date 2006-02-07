#include "ScopePlot.h"
#include "ScopePoke.h"

#include <qapplication.h>
#include <qstring.h>
#include <iostream>
#include <qtimer.h>


int
main(int argc, char** argv)
{

  QApplication app(argc, argv);

  ScopePlot scope(0);
  scope.show();

  PokeScope pokeScope(&scope);

  app.setMainWidget(&scope);

  app.exec();

  return 0;
}
