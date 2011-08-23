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

RayGraphicsItem::RayGraphicsItem(const BscanRay &ray, QString displayVar) {
    // convert ray time (usecs since 1970-01-01 00:00:00 UTC) to seconds since 
    // 1970-01-01 00:00:00 UTC
    _time = 1.0e-6 * ray.time();
    // dwell period
    _dwellPeriod = ray.dwellPeriod();
    // For each var in the product set...
    for (unsigned int v = 0; v < ray.nProducts(); v++) {
        std::string varName(ray.productName(v));
        QString qVarName(varName.c_str());
        _unitsMap[qVarName] = ray.productUnits(varName).c_str();
        // Set the gate count from the first var, and verify that other
        // vars have the same gate count.
        if (v == 0) {
            _nGates = ray.nGates();
        } else {
            if (ray.nGates() != _nGates) {
                std::cerr << "RayGraphicsItem(): Gate count mismatch " << 
                    ray.nGates() << " != " << _nGates << std::endl;
                exit(1);
            }
        }
        // Get the data for this var, in engineering units
        _dataMap[qVarName] = ray.productData(v);
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
