// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
// ** Copyright UCAR (c) 1990 - 2016                                         
// ** University Corporation for Atmospheric Research (UCAR)                 
// ** National Center for Atmospheric Research (NCAR)                        
// ** Boulder, Colorado, USA                                                 
// ** BSD licence applies - redistribution and use in source and binary      
// ** forms, with or without modification, are permitted provided that       
// ** the following conditions are met:                                      
// ** 1) If the software is modified to produce derivative works,            
// ** such modified software should be clearly marked, so as not             
// ** to confuse it with the version available from UCAR.                    
// ** 2) Redistributions of source code must retain the above copyright      
// ** notice, this list of conditions and the following disclaimer.          
// ** 3) Redistributions in binary form must reproduce the above copyright   
// ** notice, this list of conditions and the following disclaimer in the    
// ** documentation and/or other materials provided with the distribution.   
// ** 4) Neither the name of UCAR nor the names of its contributors,         
// ** if any, may be used to endorse or promote products derived from        
// ** this software without specific prior written permission.               
// ** DISCLAIMER: THIS SOFTWARE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS  
// ** OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED      
// ** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.    
// *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=* 
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
