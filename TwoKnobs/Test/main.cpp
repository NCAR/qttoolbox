#include "TwoKnobs.h"
#include "ui_TwoKnobsTest.h"
#include <qapplication.h>

#include <qstring.h>
#include <iostream>
#include <qlabel.h>

int
main(int argc, char** argv)
{

  QApplication app(argc, argv);
  QDialog* dialog = new QDialog;


  Ui::TwoKnobsTest t;
  t.setupUi(dialog);

//  t.myTwoKnobs->setTitles("Gain", "Offset");
//  t.myTwoKnobs->setRanges(-10000, 10000, 0, 50);

//  QObject::connect(t.myTwoKnobs, SIGNAL(knobOneChanged(double)),
//     t._numOne, SLOT(setNum(double))); 

//  QObject::connect(t.myTwoKnobs, SIGNAL(knobTwoChanged(double)),
//     t._numTwo, SLOT(setNum(double))); 

  dialog->show();

  app.exec();

  return 0;
}
