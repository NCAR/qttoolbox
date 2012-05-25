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
#include <cmath>

/**
 * @brief ColorTable provides a mapping between a range of values and a defined
 * list of colors. 
 */
class ColorTable : public QObject {
    Q_OBJECT
public:
    /**
     * @brief ColorTable constructor.
     * @param minValue The minimum value which will map into the list of 
     * colors.
     * @param maxValue the maximum value which will map into the list of 
     * colors
     * @param fileName the name of the file defining the list of colors in the
     * color table
     * @param tooLowColor the color to be returned for values below minValue
     * (default: black)
     * @param tooHighColor the color to be returned for values above maxValue
     * (default: red)
     */
    ColorTable(double minValue, double maxValue, QString fileName = "",
            QColor tooLowColor = QColor(0, 0, 0), 
            QColor tooHighColor = QColor(255, 0, 0));
    ColorTable();
    ColorTable(const ColorTable &src);
    virtual ~ColorTable();
    /**
     * @brief Return the color associated with the given value.  If the value is
     * in the range [minValue(), maxValue()], the appropriate color from the
     * list will be returned.  If the value is less than minValue() then
     * tooLowColor() will be returned, and if the value is greater than 
     * maxValue() then tooHighColor() will be returned.
     *
     * This method should remain inlined, since it gets called a *lot*!
     *
     * @param value the value of interest.
     */
    const QColor & colorOf(double value) const {
        if (std::isnan(value)) {
            return _tooLowColor;   // arbitrary choice here...
        } else if (value < _minValue) {
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
     * @brief Return the current minimum value.
     * @return the current minimum value.
     */
    double minimumValue() const { return _minValue; }
    /**
     * @brief Set the minimum value for display.
     * @param minValue the new minimum value
     */
    void setMinimumValue(double minValue);
    /**
     * Return the current maximum value.
     * @return the current maximum value.
     */
    double maximumValue() const { return _maxValue; }
    /**
     * @brief Set the maximum value for display.
     * @param maxValue the new maximum value
     */
    void setMaximumValue(double maxValue);
    /**
     * @brief Set display value limits.
     * @param minValue the new minimum value
     * @param maxValue the new maximum value
     */
    void setValueLimits(double minValue, double maxValue);
    /**
     * @brief Return the list of in-range colors.
     * @return the list of in-range colors
     */
    const QList<QColor> & colors() const { return _colors; }
    /**
     * @brief Return the color used for data which are off-scale high.
     * @return the color used for data which are off-scale high.
     */
    const QColor & tooHighColor() const { return _tooHighColor; }
    /**
     * @brief Return the color used for data which are off-scale low.
     * @return the color used for data which are off-scale low.
     */
    const QColor & tooLowColor() const { return _tooLowColor; }
    /**
     * @brief Return the color table file name
     * @return the color table file name
     */
    QString fileName() const { return _fileName; }
    
    ColorTable& operator=(const ColorTable& src);
    
    /**
     * @brief Static method to determine if a given color table file exists
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
    QString _fileName;
    QList<QColor> _colors;
    double _minValue;
    double _maxValue;
    QColor _tooLowColor;
    QColor _tooHighColor;
};

#endif /* COLORTABLE_H_ */
