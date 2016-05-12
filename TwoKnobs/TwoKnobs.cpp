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
#include "TwoKnobs.h"
#include <qlabel.h>
#include <qwt_knob.h>

TwoKnobs::TwoKnobs( QWidget* parent):
QWidget(parent)
{
	setupUi(this);

	connect(_knob1, SIGNAL(valueChanged(double)), this, SLOT(knobOneChangedSlot(double)));
	connect(_knob2, SIGNAL(valueChanged(double)), this, SLOT(knobTwoChangedSlot(double)));
}

/*
*  Destroys the object and frees any allocated resources
*/
TwoKnobs::~TwoKnobs()
{
	// no need to delete child widgets, Qt does it all for us
}

void
TwoKnobs::setTitles(std::string titleKnobOne, std::string titleKnobTwo)
{
	_label1->setText(titleKnobOne.c_str());
	_label2->setText(titleKnobTwo.c_str());
}

void
TwoKnobs::setRanges(double knobOneMin, double knobOneMax,
					double knobTwoMin, double knobTwoMax)
{
	_knob1->setRange(knobOneMin, knobOneMax);
	_knob2->setRange(knobTwoMin, knobTwoMax);
}

void
TwoKnobs::setValues(double knobOne, double knobTwo)
{
	_knob1->setValue(knobOne);
	_knob2->setValue(knobTwo);
}

void 
TwoKnobs::knobOneChangedSlot(double v)
{
	emit knobOneChanged(v);
}

void 
TwoKnobs::knobTwoChangedSlot(double v)
{
	emit knobTwoChanged(v);
}

void 
TwoKnobs::setScaleMaxMajor(int knobNum, int ticks)
{
	switch (knobNum) {
		case 1:
			_knob1->setScaleMaxMajor(ticks);
			break;
		case 2:
			_knob2->setScaleMaxMajor(ticks);
			break;
		default:
			break;
	}
}

void 
TwoKnobs::setScaleMaxMinor(int knobNum, int ticks)
{
	switch (knobNum) {
		case 1:
			_knob1->setScaleMaxMinor(ticks);
			break;
		case 2:
			_knob2->setScaleMaxMinor(ticks);
			break;
		default:
			break;
	}
}

void 
TwoKnobs::getRanges(double& knobOneMin, double& knobOneMax,
       double& knobTwoMin, double& knobTwoMax)
{
	knobOneMin = _knob1->minValue();
	knobOneMax = _knob1->maxValue();
	knobTwoMin = _knob2->minValue();
	knobTwoMax = _knob2->maxValue();
}

void 
TwoKnobs::getValues(double& val1, double& val2)
{
	val1 = _knob1->value();
	val2 = _knob2->value();
}
