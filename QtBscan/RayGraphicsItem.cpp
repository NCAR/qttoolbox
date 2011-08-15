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
#include "ColorTable.h"

#include "hcrddsTypeSupportC.h"

// Bad data value
const float RayGraphicsItem::BadValue = HUGE_VAL;

RayGraphicsItem::RayGraphicsItem(RadarDDS::ProductSet &ray, 
        QString displayVar) {
    // convert timetag (usecs since 1970-01-01 00:00:00 UTC) to 
    // seconds since 1970-01-01 00:00:00 UTC
    _time = 1.0e-6 * ray.products[0].hskp.timetag;
    // dwell period calculation is for single prt only!
    _dwellPeriod = ray.products[0].samples * ray.products[0].hskp.prt1;
    // For each var in the product set...
    for (unsigned int v = 0; v < ray.products.length(); v++) {
        // Get a reference to the RadarDDS::Product
        RadarDDS::Product &p = ray.products[v];
        // Name and units
        QString varName = p.name.in();
        _unitsMap[varName] = p.units.in();
        // Set the gate count from the first var, and verify that other
        // vars have the same gate count.
        if (v == 0) {
            _nGates = p.data.length();
        } else {
            if (p.data.length() != _nGates) {
                std::cerr << "RayGraphicsItem(): Gate count mismatch " << 
                    p.data.length() << " != " << _nGates << std::endl;
                exit(1);
            }
        }
        // Get the data for this var, in engineering units
        _dataMap[varName] = std::vector<float>(_nGates);
        for (unsigned int g = 0; g < _nGates; g++) {
            _dataMap[varName][g] = p.data[g] * p.scale + p.offset;
        }
    }
    // Set our display var
    setDisplayVar(displayVar);
    // Set our bounding rectangle
    _initBoundingRect();
}

RayGraphicsItem::RayGraphicsItem(const RayGraphicsItem &srcItem) :
    _dataMap(srcItem._dataMap),
    _unitsMap(srcItem._unitsMap),
    _time(srcItem._time),
    _dwellPeriod(srcItem._dwellPeriod),
    _nGates(srcItem._nGates),
    _boundingRect(srcItem._boundingRect) {
    setDisplayVar(srcItem._displayVar);
    setMatrix(srcItem.matrix());
}

void
RayGraphicsItem::_initBoundingRect() {
    _boundingRect = QRectF(_time - 0.5 * _dwellPeriod, 0, _dwellPeriod, _nGates);
}

void
RayGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) {
    BscanGraphicsScene *bscene = dynamic_cast<BscanGraphicsScene*>(scene());
    if (! bscene) {
        std::cerr << 
            "RayGraphicsItem cannot paint in a non-BscanGraphicsScene..." <<
            std::endl;
        return;
    }
    const ColorTable &colorTable = bscene->colorTable();
    //
    // Draw each gate as a single pixel in a (1 x _nGates) QImage
    //
    QImage rayImg(1, _nGates, QImage::Format_ARGB32);
    for (unsigned int gate = 0; gate < _nGates; gate++) {
        rayImg.setPixel(0, gate, colorTable.colorOf(gateValue(gate)).rgb());
    }
    
    // Paint the image into the view, scaling to view coordinates
    painter->drawImage(boundingRect(), rayImg, QRect(0, 0, 1, _nGates));
}

QString
RayGraphicsItem::displayVarUnits() const {
    // Get the units string from our latest RayGraphicsItem
    UnitsMap_t::const_iterator it = _unitsMap.find(_displayVar);
    if (it == _unitsMap.end())
        return QString();
    else
        return it->second;
    
}

void
RayGraphicsItem::setDisplayVar(QString varName) {
        _displayVar = varName;
        // _dvData is a pointer to the data for _displayVar (if we have it),
        // otherwise it is null
        if (_dataMap.find(_displayVar) != _dataMap.end()) {
            _dvData = &_dataMap[_displayVar];
        } else {
            _dvData = 0;
        }
}
