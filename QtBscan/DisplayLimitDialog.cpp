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
 * DisplayLimitDialog.cpp
 *
 *  Created on: Sep 3, 2009
 *      Author: burghart
 */
#include <QtGui/QMessageBox>

#include "DisplayLimitDialog.h"
#include "BscanGraphicsScene.h"
#include "ui_DisplayLimitDialog.h"

DisplayLimitDialog::DisplayLimitDialog(BscanGraphicsScene* scene) : _scene(scene) {
    _ui.setupUi(this);
}

int
DisplayLimitDialog::exec() {
    initFromScene();
    return QDialog::exec();
}

void
DisplayLimitDialog::accept() {
    double min = _ui.minSpinBox->value();
    double max = _ui.maxSpinBox->value();
    if (min < max) {
        _scene->setDisplayLimits(min, max);
        done(0);
    } else {
        QMessageBox msgBox;
        msgBox.setText("Min must be less than max!");
        msgBox.exec();
    }
}

void
DisplayLimitDialog::initFromScene() {
    _ui.minSpinBox->setMinimum(-10000.0);
    _ui.minSpinBox->setMaximum(10000.0);
    _ui.maxSpinBox->setMinimum(-10000.0);
    _ui.maxSpinBox->setMaximum(10000.0);
    _ui.minSpinBox->setValue(_scene->colorTable()->minimumValue());
    _ui.maxSpinBox->setValue(_scene->colorTable()->maximumValue());
}
