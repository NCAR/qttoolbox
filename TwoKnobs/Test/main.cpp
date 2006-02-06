#include "TwoKnobs.h"
#include "TwoKnobsTest.h"

#include <qapplication.h>
#include <qstring.h>
#include <iostream>
#include <qlabel.h>

int
main(int argc, char** argv)
{

  QApplication app(argc, argv);

  TwoKnobsTest t;
  t.myTwoKnobs->setTitles("Gain", "Offset");
  t.myTwoKnobs->setRanges(-10000, 10000, 0, 50);

  QObject::connect(t.myTwoKnobs, SIGNAL(knobOneChanged(double)),
     t._numOne, SLOT(setNum(double))); 

  QObject::connect(t.myTwoKnobs, SIGNAL(knobTwoChanged(double)),
     t._numTwo, SLOT(setNum(double))); 

  app.setMainWidget(&t);

  t.show();

  app.exec();

  return 0;
}
