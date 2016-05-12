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
#include <QButtonGroup>
#include <QCheckBox>
#include <QFileDialog>
#include <QDateTime>

ScopePlotTest::ScopePlotTest(QWidget* parent):
QObject(parent),
_scopePoke(0)
{
	setupUi(parent);

	// put the plot type buttons in a button group
	QButtonGroup* plotGroup = new QButtonGroup;
	plotGroup->addButton(tsButton, 0);
	plotGroup->addButton(xyButton, 1);
	plotGroup->addButton(specButton, 2);
	connect(xGrid, SIGNAL(toggled(bool)), _scopePlot, SLOT(enableXgrid(bool)));
	connect(yGrid, SIGNAL(toggled(bool)), _scopePlot, SLOT(enableYgrid(bool)));
	connect(pause, SIGNAL(toggled(bool)), _scopePlot, SLOT(pause(bool)));
	connect(saveImage, SIGNAL(released()), this, SLOT(saveImageSlot()));
	connect(plotGroup, SIGNAL(buttonReleased(int)), this, SLOT(plotType(int)));

}

ScopePlotTest::~ScopePlotTest() {
}

void
ScopePlotTest::setScopePoke(ScopePoke* scopePoke) {
	_scopePoke = scopePoke;
}

void 
ScopePlotTest::plotType(int j)
{


	switch (j) {
	  case 0:
		  if (_scopePoke)
			  _scopePoke->setPlotType(ScopePlot::TIMESERIES);
		  break;
	  case 1:
		  if (_scopePoke)
			  _scopePoke->setPlotType(ScopePlot::IVSQ);
		  break;
	  case 2:
		  if (_scopePoke)
			  _scopePoke->setPlotType(ScopePlot::SPECTRUM);
		  break;
	  default: {}
	}

}

void
ScopePlotTest::saveImageSlot()
{
	QString f = QString("C:/tmp/ScopePlot-");
	f += QDateTime::currentDateTime().toString("dd-MM-yyyy-hh-mm-ss");
	f += ".png";
	
	QString fileName = QFileDialog::getSaveFileName(0, tr("Save Image File"),
		f, tr("PNG files (*.png)"));

	if (fileName == "")
		return;

	this->_scopePlot->saveImageToFile(fileName.toStdString());
}


