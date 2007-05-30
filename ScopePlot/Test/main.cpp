#include "ScopePlotTest.h"
#include "ScopePoke.h"

#include <QApplication>
#include <QDialog>
#include <QString>
#include <iostream>
#include <QTimer>
#include <QCheckBox>

int
main(int argc, char** argv)
{

   QApplication app(argc, argv);
   QDialog* dialog = new QDialog;
   
   // create our test dialog. It will contain a ScopePlot and 
   // other buttons etc.
   ScopePlotTest t(dialog);

   // Create the test data generator, telling it
   // what ScopePlot to send data to. ScopePoke has
   // a timer which peridically sends synthesized data
   // to the ScopePlot.
   ScopePoke scopePoke(t._scopePlot);

   // Tell our test dialog about scopePoke, so that it
   // can call scopePoke in order to change the type of 
   // plot data being generated and sent to the ScopePlot
   t.setScopePoke(&scopePoke);

   // connect the grid select and pause buttons
 //  QObject::connect(t.xGrid, SIGNAL(toggled(bool)),
   //     t._scopePlot, SLOT(enableXgrid(bool)));
   //QObject::connect(t.yGrid, SIGNAL(toggled(bool)),
     //   t._scopePlot, SLOT(enableYgrid(bool)));
   //QObject::connect(t.pauseButton, SIGNAL(toggled(bool)),
    //    t._scopePlot, SLOT(pause(bool)));

   // if we don't show() the test dialog, nothing appears!
   dialog->show();

  // run the whole thing
   app.exec();

   return 0;
}
