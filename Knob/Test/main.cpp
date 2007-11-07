#include "ui_KnobTest.h"

#include <qapplication.h>

#include <qstring.h>
#include <iostream>
#include <qlabel.h>

int
main(int argc, char** argv)
{

  QApplication app(argc, argv);
  QDialog* dialog = new QDialog;


  Ui::KnobTest t;
  t.setupUi(dialog);

  t.knob->setTitle("Gain");
  t.knob->setRange(-10000, 10000);

  QObject::connect(t.knob, SIGNAL(valueChanged(double)),
     t.testText, SLOT(setNum(double))); 

  dialog->show();

  app.exec();

  return 0;
}
