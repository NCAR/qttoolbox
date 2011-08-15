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
namespace RadarDDS {
    class ProductSet;
}
/**
 * RayGraphicsItem is a QGraphicsItem for representing radar rays.  It 
 * generates as a simple rectangular view appropriate for time-height/bscan 
 * displays.
 */
class RayGraphicsItem : public QGraphicsItem {
public:
    /**
     * Construct a RayGraphicsItem from a RadarDDS::ProductSet
     * @param ray a RadarDDS:ProductSet containing the data for this item
     * @param displayVar the name of the variable to be displayed
     */
    RayGraphicsItem(RadarDDS::ProductSet &ray, QString displayVar = "");
    RayGraphicsItem(const RayGraphicsItem &srcItem);
    virtual ~RayGraphicsItem() {}
    
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
        if (_dvData && gate < _nGates)
            return (*_dvData)[gate];
        else
            return BadValue;
    }
    /**
     * Get the currently displayed variable.
     * @return the name of the currently displayed variable.
     */
    QString displayVar() const { return _displayVar; }
    /**
     * Get units of the displayed variable.
     * @return the units string for the currently displayed variable.
     */
    QString displayVarUnits() const;
    /**
     * Set the variable for display
     * @param varName the name of the variable to display.
     */
    void setDisplayVar(QString varName);
    /**
     * Return a std::map from our var names to their units.
     * @return a std::map mapping var names to their units strings
     */
    QStringList varNames() const {
        QStringList names;
        UnitsMap_t::const_iterator it;
        for (it = _unitsMap.begin(); it != _unitsMap.end(); it++)
            names << it->first;

        return names;
    }
    /**
     * Bad data value
     */
    static const float BadValue;
private:
    void _initBoundingRect();
    
    // Map from var name to vector of gate-by-gate data
    typedef std::map<QString,std::vector<float> > DataMap_t;
    DataMap_t _dataMap;
    // Map from var name to units string
    typedef std::map<QString,QString> UnitsMap_t;
    UnitsMap_t _unitsMap;
    double _time;
    double _dwellPeriod;
    unsigned int _nGates;
    QString _displayVar;        // name of the var to display
    std::vector<float> *_dvData;    // data for _displayVar
    QRectF _boundingRect;
};
#endif /* RAYGRAPHICSITEM_H_ */
