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
 * GateLimitDialog.cpp
 *
 *  Created on: Sep 3, 2009
 *      Author: burghart
 */
#include <QtWidgets/QMessageBox>

#include "GateLimitDialog.h"
#include "BscanGraphicsScene.h"
#include "ui_GateLimitDialog.h"

GateLimitDialog::GateLimitDialog(BscanGraphicsScene* scene) : _scene(scene) {
    _ui.setupUi(this);
    connect(_ui.resetButton, SIGNAL(clicked()), this, SLOT(initFromScene()));
    connect(_ui.showAllButton, SIGNAL(clicked()), this, SLOT(selectAllGates()));
}

int
GateLimitDialog::exec() {
    initFromScene();
    return QDialog::exec();
}

void
GateLimitDialog::accept() {
    int min = _ui.minGateSpinBox->value();
    int max = _ui.maxGateSpinBox->value();
    if (min <= max) {
        _scene->setGateLimits(min, max);
        done(0);
    } else {
        QMessageBox msgBox;
        msgBox.setText("Min gate must be <= max gate!");
        msgBox.exec();
    }
}

void
GateLimitDialog::initFromScene() {
    _ui.minGateSpinBox->setMaximum(_scene->nGates());
    _ui.maxGateSpinBox->setMaximum(_scene->nGates());
    _ui.minGateSpinBox->setValue(_scene->minGate());
    _ui.maxGateSpinBox->setValue(_scene->maxGate());
}

void
GateLimitDialog::selectAllGates() {
    _ui.minGateSpinBox->setValue(0);
    _ui.maxGateSpinBox->setValue(_scene->nGates());
}
