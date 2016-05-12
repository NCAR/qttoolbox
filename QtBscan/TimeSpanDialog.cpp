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
/*
 * TimeSpanDialog.cpp
 *
 *  Created on: Sep 3, 2009
 *      Author: burghart
 */
#include <QtGui/QMessageBox>

#include "TimeSpanDialog.h"
#include "BscanGraphicsScene.h"
#include "ui_TimeSpanDialog.h"

TimeSpanDialog::TimeSpanDialog(BscanGraphicsScene* scene) : _scene(scene) {
    _ui.setupUi(this);
}

int
TimeSpanDialog::exec() {
    initFromScene();
    return QDialog::exec();
}

void
TimeSpanDialog::accept() {
    _scene->setTimeSpan(_ui.timeSpanSpinBox->value());
    done(0);
}

void
TimeSpanDialog::initFromScene() {
    _ui.timeSpanSpinBox->setValue(_scene->timeSpan());
}
