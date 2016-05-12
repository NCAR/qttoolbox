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
 * RayGraphicsItem.h
 *
 *  Created on: Aug 20, 2009
 *      Author: burghart
 */

#ifndef RAYGRAPHICSITEM_H_
#define RAYGRAPHICSITEM_H_

#include <map>
#include <set>
#include <QGraphicsItem>

#include "ColorTable.h"

class BscanRay;

/**
 * RayGraphicsItem is a QGraphicsItem for representing radar rays.  It 
 * generates as a simple rectangular view appropriate for time-height/bscan 
 * displays.
 */
class RayGraphicsItem : public QGraphicsItem {
public:
    /**
     * Construct a RayGraphicsItem from a BscanRay, display variable name,
     * and color table.
     * @param ray a RadarDDS:ProductSet containing the data for this item
     * @param displayVar the name of the variable to be displayed
     * @param colorTable the ColorTable to use in rendering the ray.
     */
    RayGraphicsItem(const BscanRay &ray, QString displayVar, 
            const ColorTable &colorTable);
    RayGraphicsItem(const RayGraphicsItem &srcItem);
    virtual ~RayGraphicsItem();
    
    virtual QRectF boundingRect() const { return _boundingRect; }
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
            QWidget *widget = 0);
    /**
     * Return the number of gates contained in this item
     */
    int nGates() const { return _nGates; }
    /**
     * Return the time for this item, in seconds since 1970-01-01 00:00:00 UTC.
     */
    double time() const { return _time; }
    /**
     * Return the data value for our displayed variable, at the given gate.
     * @param gate the gate to be reported.
     */
    float gateValue(unsigned int gate) const {
        if (! _data.empty() && gate < _nGates)
            return _data[gate];
        else
            return BadValue;
    }
    /**
     * Bad data value
     */
    static const float BadValue;
private:
    void _initBoundingRect();
    
    double _time;           // seconds since 1970-01-01 00:00:00 UTC
    double _dwellPeriod;    // seconds
    unsigned int _nGates;
    QImage _rayImage;
    QRectF _boundingRect;
    // per-gate data for the displayed variable
    std::vector<float> _data;
};
#endif /* RAYGRAPHICSITEM_H_ */
