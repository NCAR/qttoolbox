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
#include "StatusGauge.h"

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPainter>
#include <QBrush>
#include <QColor>
#include <QRectF>
#include <QPaintEvent>

StatusGauge::StatusGauge(QWidget* parent) :
    QWidget(parent), _cellPercent(90.0), _orient(GaugeVertical) {

    setMinimumSize(30, 100);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    // set the default inidcators
    std::vector<std::string> colors;
    colors.push_back("green");
    colors.push_back("yellow");
    colors.push_back("red");

    reconfigure(colors, _cellPercent);
}

/******************************************************************/
StatusGauge::StatusGauge(std::vector<std::string> colors,
                         double cellPercent,
                         StatusGaugeOrientation orient,
                         QWidget* parent) :
    QWidget(parent), _cellPercent(cellPercent), _orient(GaugeVertical) {

    reconfigure(colors, _cellPercent, orient);
}

/******************************************************************/
StatusGauge::~StatusGauge() {
}

/******************************************************************/
void StatusGauge::paintEvent(QPaintEvent* e) {

    int h = height();
    int w = width();

    int n = _ison.size();
    // what is the height or width of our boxes, and the
    // indicator diameter?
    double delta;
    double diameter;
    if (_orient == GaugeVertical) {
        // divide vertical column into boxes
        delta = h/n;
        diameter = delta*(_cellPercent/100.0);
        if (diameter > w*(_cellPercent/100.0))
            diameter = w*(_cellPercent/100.0);
    } else {
        // divide horizontal strip into boxes
        delta = w/n;
        diameter = delta*(_cellPercent/100.0);
        if (diameter > h*(_cellPercent/100.0))
            diameter = h*(_cellPercent/100.0);
    }
    QPainter p;
    p.setPen(Qt::SolidLine);
    p.begin(this);
    for (unsigned int i = 0; i < _ison.size(); i++) {
        // if no color is specified, then the client didn't want an
        // indicator in this position
        if (_colors[i].size() > 0) {
            int leftX;
            int topY;
            if (_orient == GaugeVertical) {
                leftX = (int)((w-diameter)/2);
                topY = (int)(h-(i+1)*delta + (delta-diameter)/2);
            } else {
                leftX = (int)(i*delta + delta/2-diameter/2);
                topY = (int)(h-diameter)/2;
            }

            QColor color(_colors[i].c_str());
            QBrush brush(color);
            if (!_ison[i]) {
                brush.setStyle(Qt::Dense6Pattern);
            } else {
                brush.setStyle(Qt::SolidPattern);
            }
            p.setBrush(brush);
            p.drawEllipse(leftX, topY, (int)diameter, (int)diameter);
        }
    }
    p.end();

}

/******************************************************************/
void StatusGauge::mouseReleaseEvent(QMouseEvent *e) {
emit                     released();
}

/******************************************************************/
QImage* StatusGauge::getImage() {
    QPixmap pixmap = QPixmap::grabWidget(this);
    QImage* image = new QImage(pixmap.toImage());
    return image;
}

/******************************************************************/
QPixmap* StatusGauge::getPixmap() {
    QPixmap* pixmap = new QPixmap(QPixmap::grabWidget(this));
    return pixmap;
}

/******************************************************************/
void StatusGauge::on(int index,
                     bool ison) {
    if (index < 0 || index >= (int)_ison.size())
        return;
    _ison[index] = ison;

    repaint();
}
/******************************************************************/
int StatusGauge::size() {
    return _ison.size();
}

/******************************************************************/
void StatusGauge::reconfigure(std::vector<std::string> colors,
                              double cellPercent,
                              StatusGaugeOrientation orient) {
    _colors = colors;
    _cellPercent = cellPercent;
    _orient = orient;

    if (_cellPercent < 0)
        _cellPercent = 0.0;

    // initialize on/off status
    _ison.resize(_colors.size());
    for (unsigned int i = 0; i < _ison.size(); i++)
        _ison[i] = false;
    
    repaint();

}
/******************************************************************/

