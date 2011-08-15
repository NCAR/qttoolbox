/*
 * ColorTable.h
 *
 *  Created on: Nov 23, 2009
 *      Author: burghart
 */

#ifndef COLORTABLE_H_
#define COLORTABLE_H_

#include <QList>
#include <QColor>

class ColorTable : public QObject {
    Q_OBJECT
public:
    ColorTable(double minValue, double maxValue, QString fileName = "",
            QColor tooLowColor = QColor(0, 0, 0), 
            QColor tooHighColor = QColor(255, 0, 0));
    ColorTable();
    ColorTable(const ColorTable &src);
    virtual ~ColorTable();
    /**
     * Return the color associated with the given value.  If the value is
     * in the range [minValue(), maxValue()], the appropriate color from the
     * list will be returned.  If the value is less than minValue() then
     * tooLowColor() will be returned, and if the value is greater than 
     * maxValue() then tooHighColor() will be returned.
     * @param value the value of interest.
     */
    const QColor & colorOf(double value) const {
        if (value < _minValue) {
            return _tooLowColor;
        } else if (value > _maxValue) {
            return _tooHighColor;
        } else {
            int ndx = int(_colors.size() * 
            	(value - _minValue) / (_maxValue - _minValue));
            // Special handling for the case of value == _maxValue.
            if (value == _maxValue)
                ndx--;

            return _colors[ndx];
        }   
    }
    /**
     * Return the current minimum value.
     * @return the current minimum value.
     */
    double minimumValue() const { return _minValue; }
    /**
     * Set the minimum value for display.
     * @param minValue the new minimum value
     */
    void setMinimumValue(double minValue);
    /**
     * Return the current maximum value.
     * @return the current maximum value.
     */
    double maximumValue() const { return _maxValue; }
    /**
     * Set the maximum value for display.
     * @param maxValue the new maximum value
     */
    void setMaximumValue(double maxValue);
    /**
     * Set display value limits.
     * @param minValue the new minimum value
     * @param maxValue the new maximum value
     */
    void setValueLimits(double minValue, double maxValue);
    /**
     * Return the list of in-range colors.
     * @return the list of in-range colors
     */
    const QList<QColor> & colors() const { return _colors; }
    /**
     * Return the color used for data which are off-scale high.
     * @return the color used for data which are off-scale high.
     */
    const QColor & tooHighColor() const { return _tooHighColor; }
    /**
     * Return the color used for data which are off-scale low.
     * @return the color used for data which are off-scale low.
     */
    const QColor & tooLowColor() const { return _tooLowColor; }
    
    ColorTable& operator=(const ColorTable& src);
    
    /**
     * Static method to determine if a given color table file exists
     * @param name the name of the color table being sought, e.g. "chris.ct"
     * @param verbose if true, print a message if the file is not found
     * @param fullPath if this pointer is not null, the full path name of the 
     *    found file (if any) is returned in the QString
     * @return true iff the named color table file can be found
     */
    static bool colorTableFileExists(QString name, bool verbose = false,
    		QString *fullPath = 0);

signals:
    /**
     * This signal is emitted if any trait of the color table changes.
     */
    void changed();
private:
    QList<QColor> _colors;
    double _minValue;
    double _maxValue;
    QColor _tooLowColor;
    QColor _tooHighColor;
};

#endif /* COLORTABLE_H_ */
