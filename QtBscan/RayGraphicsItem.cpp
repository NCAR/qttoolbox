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
 * RayGraphicsItem.cpp
 *
 *  Created on: Aug 20, 2009
 *      Author: burghart
 */
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <QColor>
#include <QImage>
#include <QPainter>

#include "RayGraphicsItem.h"
#include "BscanGraphicsScene.h"
#include "BscanRay.h"
#include "ColorTable.h"

// Bad data value
const float RayGraphicsItem::BadValue = HUGE_VAL;

RayGraphicsItem::RayGraphicsItem(const BscanRay &ray, QString displayVar, 
        const ColorTable & colorTable) :
        _time(1.0e-6 * ray.time()),     // convert useconds to seconds
        _dwellPeriod(ray.dwellPeriod()),
        _nGates(ray.nGates()),
        _rayImage(1, _nGates, QImage::Format_ARGB32) {
    //
    // Draw each gate as a single pixel in our (1 x _nGates) QImage
    //
    _data = ray.hasProduct(displayVar.toStdString()) ?
            ray.productData(displayVar.toStdString()) : std::vector<float>();
    
    for (unsigned int gate = 0; gate < _nGates; gate++) {
        if (_data.empty()) {
            _rayImage.setPixel(0, gate, colorTable.colorOf(BadValue).rgb());
        } else {
            _rayImage.setPixel(0, gate, colorTable.colorOf(_data[gate]).rgb());
        }        
    }
    // Set our bounding rectangle
    _initBoundingRect();
}

RayGraphicsItem::RayGraphicsItem(const RayGraphicsItem &srcItem) :
        _time(srcItem._time),
        _dwellPeriod(srcItem._dwellPeriod),
        _nGates(srcItem._nGates),
        _boundingRect(srcItem._boundingRect),
        _data(srcItem._data) {
    setMatrix(srcItem.matrix());
}

RayGraphicsItem::~RayGraphicsItem() {
}

void
RayGraphicsItem::_initBoundingRect() {
    // Bounding rectangle width is the _dwellPeriod, centered horizontally
    // on _time. Bounding rectangle runs from 0 to _nGates in height.
    _boundingRect = QRectF(_time - 0.5 * _dwellPeriod, 0, _dwellPeriod, _nGates);
}

void
RayGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) {
    // Paint the image into the view, scaling to view coordinates
    if (isVisible()) {
        painter->drawImage(boundingRect(), _rayImage, QRect(0, 0, 1, _nGates));
    }
}
