/*
 * BscanRay.cpp
 *
 *  Created on: Aug 11, 2011
 *      Author: burghart
 */

#include "BscanRay.h"

/**
 * @brief Inner class _Product contains name, units, gate count, and data for a 
 * product within a BScanRay.
 */
class BscanRay::_Product {
public:
    /**
     * @brief Instantiate a _Product with the given name, units, gate count, and
     * data.
     * @param name the name of the product
     * @param units a string naming the units of the product's data
     * @param gateSpacing the gate spacing for the data, in meters
     * @param data a vector containing nGates values of data for this
     * product
     */
    _Product(std::string name, std::string units, float gateSpacing,
            const std::vector<double> & data);
    virtual ~_Product();
    /**
     * @brief Return the gate count for the product
     * @return the gate count for the product
     */
    unsigned int nGates() const { return((unsigned int)_data.size()); }
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
    const std::vector<double> & data() const { return _data; }
    /**
     * @brief Return the product's data value for the selected gate
     * @param gateNum the number of the selected gate
     * @return the product's data value for the selected gate
     * @throws out_of_range exception for a gate number which is out of
     * range
     */
    double operator[](unsigned int gateNum) const { return _data[gateNum]; }
private:
    unsigned int _nGates;
    std::string _name;
    std::string _units;
    std::vector<double> _data;
};


BscanRay::BscanRay(long long time, double dwellPeriod) :
    _time(time),
    _dwellPeriod(dwellPeriod) {
}

BscanRay::~BscanRay() {
    // TODO Auto-generated destructor stub
}

bool
BscanRay::hasProduct(std::string name) const {
    return(_products.find(name) != _products.end());
}

std::vector<std::string>
BscanRay::productNames() const {
    std::vector<std::string> names;
    // The keys from our _products map are the product names
    for (std::map<std::string, _Product &>::const_iterator it = _products.begin();
            it != _products.end(); it++) {
        names.push_back((*it).first);
    }
    return names;
}

BscanRay::_Product &
BscanRay::_getProduct(std::string name) const {
    std::map<std::string, _Product &>::const_iterator it = _products.find(name);
    if (it == _products.end()) {
        throw NoSuchProductException(name);
    }
    return((*it).second);
}

