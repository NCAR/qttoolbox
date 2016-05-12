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
#ifndef PPITESTH
#define PPITESTH
#include "ui_PPITest.h"

#include <vector>
#include <qtimer.h>
#include <QVBoxLayout>
#include "ColorBar.h"
#include "ColorMap.h"

class PPITest: public QObject, public Ui::PPITest
{
	Q_OBJECT
public:
	PPITest( int nVars, QWidget* parent = 0);
	virtual ~PPITest();

	public slots:

		void varSelectSlot1(int);
		void varSelectSlot2(int);
		void changeDir();
		void clearVarSlot(int);
		void panUpSlot();
		void panDownSlot();
		void panLeftSlot();
		void panRightSlot();
		void resetViewSlot();
		void ringsSlot(bool);
		void gridSlot(bool);
		void backgroundColorSlot();
		void ringColorSlot();
		void startSlot();
		void stopSlot();
		void addBeam();
		void zoomInSlot();
		void zoomOutSlot();
		void saveImageSlot();
		void decimationSlot(int);

protected:
	void pan(double x, double y);
	double _angle;
	double _angleInc;
	int _nVars;
	int _gates;
	double _beamWidth;
	std::vector<int> _varIndices;
	std::vector<std::vector<double> > _beamData;
	std::vector<ColorMap*> _maps1;
	std::vector<ColorMap*> _maps2;
	QTimer _timer;
	double _currentX;
	double _currentY;
	int _decimation;
};

#endif

