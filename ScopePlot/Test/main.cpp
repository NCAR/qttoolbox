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

  QTimer timer;

  PokeScope pokeScope(&scope);

  QObject::connect(&timer, SIGNAL(timeout()), 
     &pokeScope, SLOT(timeout()));

  app.setMainWidget(&scope);

  timer.start(10);
  app.exec();

  return 0;
}
