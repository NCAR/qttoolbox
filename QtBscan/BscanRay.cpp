/*
 * BscanRay.cpp
 *
 *  Created on: Aug 11, 2011
 *      Author: burghart
 */

#include "BscanRay.h"
#include <QMetaType>

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
     * product
     */
    _Product(std::string name, std::string units, 
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
private:
    std::string _name;
    std::string _units;
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
    for (int i = 0; i < ray._products.size(); i++) {
        _products.push_back(new _Product(ray._products[i]->name(),
                ray._products[i]->units(), ray._products[i]->data()));
    }
}
BscanRay::~BscanRay() {
    for (int i = 0; i < _products.size(); i++) {
        delete _products[i];
    }
    _products.clear();
}

void
BscanRay::addProduct(std::string name, std::string units,
        const std::vector<float> & data) {
    _products.push_back(new _Product(name, units, data));
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
        const std::vector<float> & data) : 
        _name(name),
        _units(units),
        _data(data) {
}
