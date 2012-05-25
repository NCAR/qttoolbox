/*
 * ColorTable.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: burghart
 */

#include <iostream>
#include "ColorTable.h"
#include "BscanShareDir.h"
#include <QFile>
#include <QStringList>
#include <QTextStream>

ColorTable::ColorTable(double minValue, double maxValue, QString name,
        QColor tooLowColor, QColor tooHighColor) :
        _fileName(name),
        _minValue(minValue),
        _maxValue(maxValue),
        _tooLowColor(tooLowColor),
        _tooHighColor(tooHighColor) {
    
    QFile file("");
    bool fileOk = false;

    QString fullPath;
    if (colorTableFileExists(_fileName, true, &fullPath)) {
    	file.setFileName(fullPath);
    	if (file.open(QIODevice::ReadOnly| QIODevice::Text)) {
    		// The color table file exists and is now open!
    		fileOk = true;
    	} else {
    		std::cerr << "Failed to open color table file '" << 
    		file.fileName().toStdString() << std::endl;
    	}
    }

    if (! fileOk) {
    	std::cerr << "Using an all-black color table." << std::endl;
    	_colors.append(QColor(0, 0, 0));
    	emit changed();
    	return;
    }
    
    // Read a simple color table file:
    //
    //     <r> <g> <b>
    //     ...
    //
    // where all <r>, <g>, and <b> are in the range [0,255].
    // Any line beginning with '#' is treated as a comment and ignored    
    QTextStream inStream(&file);
    while (! inStream.atEnd()) {
        QString line = inStream.readLine().simplified();
        QStringList tokens = line.split(QRegExp("\\s+"));
        // skip comments
        if (tokens[0][0] == '#')
            continue;

        if (tokens.size() != 3) {
            std::cerr << "Bad line ignored from '" << _fileName.toStdString() <<
                "': " << line.toStdString() << std::endl;
            continue;
        }

        bool okR, okG, okB;
        int r = tokens[0].toUInt(&okR);
        okR &= (r < 256);
        int g = tokens[1].toUInt(&okG);
        okG &= (g < 256);
        int b = tokens[2].toUInt(&okB);
        okB &= (b < 256);
        if (okR && okG && okB) {
            // Append this color to the list
            _colors.append(QColor(r, g, b));
        } else {
            std::cerr << "Bad RGB ignored from '" << _fileName.toStdString() <<
                "': " << line.toStdString() << std::endl;
        }
    }

    emit changed();
    return;
}

// Default color table constructor.  Limits are 0.0 to 1.0, with one 
// color (gray).
ColorTable::ColorTable() :
    _minValue(0.0),
    _maxValue(1.0),
    _tooLowColor(QColor(0, 0, 255)),
    _tooHighColor(QColor(0, 0, 255)) {
    _colors.append(QColor(128, 128, 128));
}

// Copy constructor
ColorTable::ColorTable(const ColorTable &src) {
    operator=(src);
}

ColorTable::~ColorTable() {
}

void
ColorTable::setMinimumValue(double minValue) {
    setValueLimits(minValue, _maxValue);
}

void
ColorTable::setMaximumValue(double maxValue) {
    setValueLimits(_minValue, maxValue);
}

void
ColorTable::setValueLimits(double minValue, double maxValue) {
    if (minValue == _minValue && maxValue == _maxValue)
        return;
    _minValue = minValue;
    _maxValue = maxValue;
    emit changed();
}

ColorTable&
ColorTable::operator=(const ColorTable& src) {
    _colors = src._colors;
    _minValue = src._minValue;
    _maxValue = src._maxValue;
    _tooLowColor = src._tooLowColor;
    _tooHighColor = src._tooHighColor;
    return *this;
}

bool
ColorTable::colorTableFileExists(QString name, bool verbose, 
		QString *fullPath) {
    // Path list for finding color tables: check BscanShareDir, then just
    // try the unmunged filename
    QStringList searchPath;
    searchPath << BscanShareDir.c_str() << "";
    
    QString testedFiles;    // keep a list of file names we've tested

    QFile file("");
    
    for (QStringList::iterator i = searchPath.begin(); i != searchPath.end(); i++) {
        QString testPath;
        // If the directory is an empty string, just use the file name as-is,
        // otherwise concatenate the directory and the given file name
        if ((*i).isEmpty())
            testPath = name;
        else
            testPath = *i + "/" + name;
        
        // Keep a string listing all of the tested files
        if (i != searchPath.begin())
            testedFiles += ", ";
        testedFiles += testPath;
        
        // If this file exists, we're done!
        file.setFileName(testPath);
        if (file.exists()) {
        	if (fullPath)
        		*fullPath = testPath;
        	return true;
        }
    }
    if (verbose) {
        std::cerr << "Color table " << name.toStdString() << 
            " not found in: " << testedFiles.toStdString() << std::endl;
    }
    return false;
}
