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

#include "ColorBarTest.h"
#include "ColorBar.h"
#include "ColorMap.h"
#include <QRadioButton>
#include <QButtonGroup>

/////////////////////////////////////////////////////
ColorBarTest::ColorBarTest(QDialog* dialog)
{
	setupUi(dialog);

	_maps = ColorMap::builtinMaps();

	QVBoxLayout* vbox = new QVBoxLayout;
	for (unsigned int i = 0; i < _maps.size(); i++) {
		QRadioButton* b = new QRadioButton(_maps[i].c_str());
		_bgroup.addButton(b, i);
		vbox->addWidget(b);
		if (i == 0)
			b->setChecked(true);
	}
	connect(&_bgroup, SIGNAL(buttonClicked(int)), this, SLOT(buttonSlot(int)));
	_groupBox->setLayout(vbox);
	ColorMap m(0.0, 160.0, _maps[0]);
	_colorBar->configure(m);

}

/////////////////////////////////////////////////////
ColorBarTest::~ColorBarTest() {
}

/////////////////////////////////////////////////////
void
ColorBarTest::buttonSlot(int) {
	int id = _bgroup.checkedId();
	std::string map = _maps[id];
	ColorMap m(0.0, 160.0, map);
	_colorBar->configure(m);
}
