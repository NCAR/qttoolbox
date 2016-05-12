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
#include "ColorBar.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QRectF>
#include <QPaintEvent>

ColorBar::ColorBar(QWidget* parent):
QWidget(parent),
_colorMap(0.0, 10.0, "default")
{
	setMinimumSize(30, 100);
	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	configure(_colorMap);
}

/******************************************************************/
ColorBar::~ColorBar()
{
}

/******************************************************************/
void
ColorBar::configure(ColorMap& map) {
	_colorMap = map;
	this->update();
}

/******************************************************************/
void
ColorBar::paintEvent(QPaintEvent* e) {
	
	if (_colorMap.size() == 0)
		return;
	
	int h = e->rect().height();
	int w = e->rect().width();

	h = height();
	w = width();

	double deltaVal = (_colorMap.rangeMax() - _colorMap.rangeMin())/_colorMap.size();
	double deltaY = (double)(height())/_colorMap.size();
	QRectF r(0, 0, w, deltaY);
	QPainter p;
	p.setPen(Qt::SolidLine);
	p.begin(this);
	for (unsigned int i = 0; i < _colorMap.size(); i++) {
		QColor color(_colorMap.red()[i], _colorMap.green()[i], _colorMap.blue()[i]);
        p.setBrush(color);
		double topY = h-(i+1)*deltaY;
		QRectF r(0, topY, w, deltaY);
		// fill the swatch with the color
		p.fillRect(r, color);
		// add the label
		double midValue = _colorMap.rangeMin()
			+ deltaVal*(i+0.5);
		QString label = QString("%1").arg(midValue,0,'f',1);
		p.drawText(0, (int)topY, w, (int)deltaY, 
			Qt::AlignVCenter | Qt::AlignHCenter, 
			label);
	}
	p.end();

}

/******************************************************************/
void
ColorBar::mouseReleaseEvent(QMouseEvent *e) {
	emit released();
}

/******************************************************************/
QImage*
ColorBar::getImage()
{	
	QPixmap pixmap = QPixmap::grabWidget(this);
	QImage* image = new QImage(pixmap.toImage());
	return image;
}

/******************************************************************/
QPixmap*
ColorBar::getPixmap()
{	
	QPixmap* pixmap = new QPixmap(QPixmap::grabWidget(this));
	return pixmap;
}
