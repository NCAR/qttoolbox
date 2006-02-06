#include "ScopePlot.h"

#include <qapplication.h>
#include <qstring.h>
#include <iostream>

int
main(int argc, char** argv)
{

  QApplication app(argc, argv);

  ScopePlot s(0);
  app.setMainWidget(&s);

  s.show();

  app.exec();

  return 0;
}
