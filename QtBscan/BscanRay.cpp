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
 * BscanRay.cpp
 *
 *  Created on: Aug 11, 2011
 *      Author: burghart
 */

#include "BscanRay.h"
#include <QMetaType>
#include <cmath>

Q_DECLARE_METATYPE(BscanRay)

/**
 * @brief Inner class _Product contains name, units, and data for a 
 * product within a BScanRay.
 */
class BscanRay::_Product {
public:
    /**
     * @brief Instantiate a _Product with the given name, units, gate count, and
     * data.
     * @param name the name of the product
     * @param units a string naming the units of the product's data
     * @param data a vector containing nGates values of data for this
     * @param missingValue the float value used to represent missing data
     * product
     */
    _Product(std::string name, std::string units, float missingValue,
            const std::vector<float> & data);
    virtual ~_Product() {}
    /**
     * @brief Return the name of this product
     * @return the name of this product
     */
    const std::string & name() const { return _name; }
    /**
     * @brief Return the units of this product's data
     * @return the units of this product's data
     */
    const std::string & units() const { return _units; }
    /**
     * @brief Return a vector of this product's data
     * @return a vector of this product's data
     */
    const std::vector<float> & data() const { return _data; }
    /**
     * @brief Return the number used to represent missing values in the data.
     */
    float missingValue() const { return _missingValue; }
private:
    std::string _name;
    std::string _units;
    float _missingValue;
    std::vector<float> _data;
};


BscanRay::BscanRay(long long time, float lat, float lon, float alt,
        float azimuth, float elevation, float dwellPeriod, 
        unsigned int nGates, float gateSpacing) :
    _time(time),
    _lat(lat),
    _lon(lon),
    _alt(alt),
    _azimuth(azimuth),
    _elevation(elevation),
    _dwellPeriod(dwellPeriod),
    _nGates(nGates),
    _gateSpacing(gateSpacing) {
    // Register BscanRay as something we can ship via Qt signal.
    qRegisterMetaType<BscanRay>();
}

BscanRay::BscanRay(const BscanRay & ray) :
    _time(ray._time),
    _lat(ray._lat),
    _lon(ray._lon),
    _alt(ray._alt),
    _azimuth(ray._azimuth),
    _elevation(ray._elevation),
    _dwellPeriod(ray._dwellPeriod),
    _nGates(ray._nGates),
    _gateSpacing(ray._gateSpacing) {
    // Copy over all of the products
    for (unsigned int i = 0; i < ray._products.size(); i++) {
        _products.push_back(new _Product(ray._products[i]->name(),
                ray._products[i]->units(), ray._products[i]->missingValue(),
                ray._products[i]->data()));
    }
}
BscanRay::~BscanRay() {
    for (unsigned int i = 0; i < _products.size(); i++) {
        delete _products[i];
    }
    _products.clear();
}

void
BscanRay::addProduct(std::string name, std::string units, 
        const std::vector<float> & data) {
    // Just call the other addProduct() method, using HUGE_VAL as the 
    // missing value.
    addProduct(name, units, HUGE_VAL, data);
}

void
BscanRay::addProduct(std::string name, std::string units, float missingValue,
        const std::vector<float> & data) {
    _products.push_back(new _Product(name, units, missingValue, data));
}

bool
BscanRay::hasProduct(std::string name) const {
    std::vector<_Product *>::const_iterator it;
    for (it = _products.begin(); it != _products.end(); it++) {
        if ((*it)->name() == name)
            return true;
    }
    return false;
}

BscanRay::_Product *
BscanRay::_getProduct(std::string name) const {
    std::vector<_Product *>::const_iterator it;
    for (it = _products.begin(); it != _products.end(); it++) {
        if ((*it)->name() == name)
            return(*it);
    }
    throw NoSuchProductException(name);
}

std::string
BscanRay::productName(unsigned int ndx) const {
    return(_products[ndx]->name());
}

std::string
BscanRay::productUnits(unsigned int ndx) const {
    return(_products[ndx]->units());
}
std::string
BscanRay::productUnits(std::string productName) const {
    return(_getProduct(productName)->units());
}
const std::vector<float> &
BscanRay::productData(unsigned int ndx) const {
    return(_products[ndx]->data());
}
const std::vector<float> &
BscanRay::productData(std::string productName) const {
    return(_getProduct(productName)->data());
}


BscanRay::_Product::_Product(std::string name, std::string units, 
        float missingValue, const std::vector<float> & data) : 
        _name(name),
        _units(units),
        _missingValue(missingValue),
        _data(data) {
}
