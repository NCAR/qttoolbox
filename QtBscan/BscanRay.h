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
 * BscanRay.h
 *
 *  Created on: Aug 11, 2011
 *      Author: burghart
 */

#ifndef BSCANRAY_H_
#define BSCANRAY_H_

#include <exception>
#include <map>
#include <string>
#include <vector>

/**
 * BscanRay is a simple class to hold a ray of radial data for display in a
 * BscanWidget.
 */
class BscanRay {
public:
    static const float DEFAULT_MISSING_VALUE;
    /**
     * @brief Create a ray from the given time and origin location, pointing angle,
     * and gate geometry.
     * @param time long long ray time at the center of the dwell period, in 
     *      microseconds since 1970-01-01 00:00:00 UTC
     * @param lat the latitude of the ray's origin, in degrees
     * @param lon the longitude of the ray's origin, in degrees
     * @param alt the altitude of the ray's origin, in meters MSL
     * @param azimuth the azimuth of the ray, in degrees clockwise from true north
     * @param elevation the elevation of the ray, in degrees up from horizontal
     * @param dwellPeriod the dwell period of the ray, in seconds
     * @param nGates the number of gates of data contained in the ray
     * @param gateSpacing the width of each gate of data, in meters
     */
    BscanRay(long long time, float lat, float lon, float alt, 
            float azimuth, float elevation, float dwellPeriod, 
            unsigned int nGates, float gateSpacing);
    /**
     * @brief Copy constructor
     * @param ray the source BscanRay to be copied
     */
    BscanRay(const BscanRay & ray);
    
    /**
     * @brief Default constructor, required for registration as a QMetaType.
     */
    BscanRay() {}
    
    virtual ~BscanRay();
    
    /**
     * @brief Return the time at the middle of the dwell period of this ray, 
     * in microseconds since 1970-01-01 00:00:00 UTC
     * @return the time at the middle of the dwell period of this ray, in 
     * microseconds since 1970-01-01 00:00:00 UTC
     */
    long long time() const { return _time; }
    /**
     * @brief Return the latitude of the origin, in degrees
     * @return the latitude of the origin, in degrees
     */
    float latitude() const { return _lat; }
    /**
     * @brief Return the longitude of the origin, in degrees
     * @return the longitude of the origin, in degrees
     */
    float longitude() const { return _lon; }
    /**
     * @brief Return the altitude of the origin, in meters MSL
     * @return the altitude of the origin, in degrees
     */
    float altitude() const { return _alt; }
    /**
     * @brief Return the azimuth of the ray, in degrees clockwise from true
     * north
     * @return the azimuth of the ray, in degrees clockwise from true north
     */
    float azimuth() const { return _azimuth; }
    /**
     * @brief Return the elevation of the ray, in degrees up from horizontal
     * @return the elevation of the ray, in degrees up from horizontal
     */
    float elevation() const { return _elevation; }
    /**
     * @brief Return the dwell period for the ray, in seconds
     * @return the dwell period for the ray, in seconds
     */
    float dwellPeriod() const { return _dwellPeriod; }
    /**
     * @brief Return the number of gates in the ray
     * @return the number of gates in the ray
     */
    unsigned int nGates() const { return _nGates; }
    /**
     * @brief Return the width of each gate in the ray, in meters
     * @return the width of each gate in the ray, in meters
     */
    float gateSpacing() const { return _gateSpacing; }
    /**
     * @brief Return true iff the ray contains a product with the given name
     * @return true iff the ray contains a product with the given name
     */
    bool hasProduct(std::string name) const;  
    /**
     * @brief Add a product to the ray
     * @param name the product name
     * @param units the units for the product's data
     * @param data a vector containing data values for the product
     * @deprecated The addProduct() method which explicitly sets missing value
     * should be used instead. This one just sets missing value to HUGE_VAL.
     */
    void addProduct(std::string name, std::string units,
            const std::vector<float> & data);
    /**
     * @brief Add a product to the ray
     * @param name the product name
     * @param units the units for the product's data
     * @param missingValue the number used to represent missing data
     * @param data a vector containing data values for the product
     */
    void addProduct(std::string name, std::string units, float missingValue,
            const std::vector<float> & data);
    /**
     * @brief Return the number of products in this ray
     * @return the number of products in this ray
     */
    unsigned int nProducts() const { return _products.size(); }
    /**
     * @brief Return the name of the product at the selected index.
     * @param ndx the index of the product of interest
     * @return the name of the product at the selected index.
     */
    std::string productName(unsigned int ndx) const;
    /**
     * @brief Return the units for product at the selected index
     * @param ndx the index of the product of interest
     */
    std::string productUnits(unsigned int ndx) const;
    /**
     * @brief Return the units for the selected product
     * @param productName the name of the product of interest
     * @throw NoSuchProductException if this ray does not have the requested 
     * product
     */
    std::string productUnits(std::string productName) const;
    /**
     * @brief Return the number used to represent missing data for the selected
     * product
     * @param ndx the index of the product of interest
     */
    float productMissingValue(unsigned int ndx) const;
    /**
     * @brief Return the number used to represent missing data for the selected
     * product
     * @param productName the name of the product of interest
     */
    float productMissingValue(std::string productName) const;
    /**
     * @brief Return the data for product at the selected index
     * @param ndx the index of the product of interest
     */
    const std::vector<float> & productData(unsigned int ndx) const;
    /**
     * @brief Return the data for the selected product
     * @param productName the name of the product of interest
     * @throw NoSuchProductException if this ray does not have the requested 
     * product
     */
    const std::vector<float> & productData(std::string productName) const;
    /**
     * @brief Exception type thrown when a requested product does not exist
     */
    class NoSuchProductException : public std::exception {
    public:
        NoSuchProductException() : _what("No such product") {}
        NoSuchProductException(std::string prodName) {
            _what = "No such product: ";
            _what += prodName;
        }
        virtual ~NoSuchProductException() throw() {}
        virtual const char* what() {
            return(_what.c_str());
        }
    private:
        std::string _what;
    };
private:
    /*
     * @brief Inner class _Product contains name, units, and data 
     * for a product within a BScanRay.
     */
    class _Product;
    
    /**
     * @brief Get the named product from our list
     * @param name the name of the desired product
     * @return a reference to the named product
     * @throw NoSuchProductException if the product is not in our list
     */
    _Product * _getProduct(std::string name) const;

    // Ray time in microseconds since 1970-01-01 00:00:00 UTC
    long long _time;
    // origin latitude, degrees
    float _lat;
    // origin longitude, degrees
    float _lon;
    // origin altitude, meters MSL
    float _alt;
    // ray azimuth, degrees clockwise from true north
    float _azimuth;
    // ray elevation, degrees up from horizontal
    float _elevation;
    // Dwell period for the ray, in seconds
    float _dwellPeriod;
    // Gate count for the ray
    unsigned int _nGates;
    // Gate spacing for the ray, in meters
    float _gateSpacing;
    // Vector of _Product instances
    std::vector<_Product *> _products;
};

#endif /* BSCANRAY_H_ */
