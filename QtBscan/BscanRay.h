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
 * BscanRay is a simple class to hold a ray of data for display in a Bscan.
 */
class BscanRay {
public:
    BscanRay(long long time, double dwellPeriod);
    virtual ~BscanRay();
    
    /**
     * @brief Return the time of this ray, in microseconds since 1970-01-01 
     * 00:00:00 UTC
     * @return the time of this ray, in microseconds since 1970-01-01 
     * 00:00:00 UTC
     */
    long long time() const { return _time; }
    /**
     * @brief Return the dwell period for the ray, in seconds
     */
    double dwellPeriod() const { return _dwellPeriod; }
    /**
     * @brief Return true iff the ray contains a product with the given name
     * @return true iff the ray contains a product with the given name
     */
    bool hasProduct(std::string name) const;  
    /**
     * @brief Add a product to the ray
     * @param name the product name
     * @param units the units for the product's data
     * @param gateSpacing the gate spacing for the data, in meters
     * @param data a vector containing data values for the product
     */
    void addProduct(std::string name, std::string units, float gateSpacing,
            const std::vector<double> & data);
    /**
     * @brief Return a vector of names of products in this ray
     * @return a vector of names of products in this ray
     */
    std::vector<std::string> productNames() const;
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
    /**
     * @brief Return the data vector for a named product
     * @param prodName the name of the desired product
     * @return the data for the product as a vector of double
     * @throw NoSuchProductException if the product is not in our list
     */
    const std::vector<double> & operator[](std::string prodName) const
        throw(NoSuchProductException);
private:
    /*
     * @brief Inner class _Product contains name, units, gate count, and data 
     * for a product within a BScanRay.
     */
    class _Product;
    
    /**
     * @brief Get the named product from our list
     * @param name the name of the desired product
     * @return a reference to the named product
     * @throw NoSuchProductException if the product is not in our list
     */
    _Product & _getProduct(std::string name) const;

    // Ray time in microseconds since 1970-01-01 00:00:00 UTC
    long long _time;
    // Dwell period for the ray, in seconds
    double _dwellPeriod;
    // Our _Product objects, kept as a map keyed by product name
    std::map<std::string, _Product &> _products;
};

#endif /* BSCANRAY_H_ */
