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
#include "StatusGaugeTest.h"
#include "StatusGauge.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

/////////////////////////////////////////////////////
StatusGaugeTest::StatusGaugeTest(QDialog* parent):
    QDialog(parent)
{
	setupUi(parent);
	
	unsigned int num = 7;
	std::vector<std::string> colors;
    colors.push_back("gold");
    colors.push_back("green");
    colors.push_back("purple");
    colors.push_back("red");
    colors.push_back("cyan");
	
    QHBoxLayout* layout1 = new QHBoxLayout;
    QVBoxLayout* layout2 = new QVBoxLayout;
    for (unsigned int i = 0; i < num; i++) {
        StatusGauge* g1 = new StatusGauge(colors, 70.0, StatusGauge::GaugeVertical);
        layout1->addWidget(g1);
        g1->on(i % colors.size(), true);
        connect(g1, SIGNAL(released()), this, SLOT(releasedSlot()));
    }
    groupBox1->setLayout(layout1);
    for (unsigned int i = 0; i < num; i++) {
        if (i == num-1) {
            // leave out the first indicator for the 
            // very last gauge
            colors[0] = "";
        }
        StatusGauge* g2 = new StatusGauge(colors, 95.0, StatusGauge::GaugeHorizontal);
        layout2->addWidget(g2);
        g2->on(i % colors.size(), true);
        connect(g2, SIGNAL(released()), this, SLOT(releasedSlot()));
    }
    groupBox2->setLayout(layout2);
}

/////////////////////////////////////////////////////
StatusGaugeTest::~StatusGaugeTest() {
}

/////////////////////////////////////////////////////
void
StatusGaugeTest::releasedSlot() {
    QMessageBox::information(this, "Note!", "A StatusGauge was punched!");
}

