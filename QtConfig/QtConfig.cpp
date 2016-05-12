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
#include "QtConfig.h"
#include <QList>
#include <QStringList>
#include <QVariant>
#include <iostream>

//////////////////////////////////////////////////////////
QtConfig::QtConfig(
        const std::string organization,
            const std::string application) :
    QSettings(QSettings::IniFormat,
            QSettings::UserScope,
            organization.c_str(),
            application.c_str()) {
}

//////////////////////////////////////////////////////////
QtConfig::QtConfig(const std::string filePath):
QSettings(filePath.c_str(), QSettings::IniFormat) {

}

//////////////////////////////////////////////////////////
QtConfig::~QtConfig() {
}

//////////////////////////////////////////////////////////
std::vector<std::string> QtConfig::childGroups(
        std::string topGroup) {

    std::vector<std::string> result;

    // set the top group
    beginGroup(topGroup.c_str());

    // get the child groups
    QStringList children = QSettings::childGroups();

    // save them
    for (int i = 0; i < children.size(); i++) {
        result.push_back(children[i].toStdString());
    }

    endGroup();

    return result;
}

//////////////////////////////////////////////////////////
void QtConfig::setString(
        std::string key,
            std::string t) {
    setValue(key.c_str(), t.c_str());
    sync();
}

//////////////////////////////////////////////////////////
std::string QtConfig::getString(
        std::string key,
            std::string defaultValue) {
    std::string s = 
    	value(key.c_str(), defaultValue.c_str()).toString().toStdString();

	if (! contains(key.c_str())) {
    	setValue(key.c_str(), s.c_str());
    	sync();
    }

    return s;
}

//////////////////////////////////////////////////////////
void QtConfig::setDouble(
        std::string key,
            double value) {
    setValue(key.c_str(), value);
    sync();
}

//////////////////////////////////////////////////////////
double QtConfig::getDouble(
        std::string key,
            double defaultValue) {
    double d = value(key.c_str(), defaultValue).toDouble();
    if (! contains(key.c_str())) {
    	setValue(key.c_str(), d);
    	sync();
    }
    return d;
}

//////////////////////////////////////////////////////////
void QtConfig::setFloat(
        std::string key,
            float value) {
    setValue(key.c_str(), value);
    sync();
}

//////////////////////////////////////////////////////////
float QtConfig::getFloat(
        std::string key,
            float defaultValue) {
    float d = value(key.c_str(), defaultValue).toDouble();
    if (! contains(key.c_str())) {
    	setValue(key.c_str(), d);
    	sync();
    }
    return d;
}

//////////////////////////////////////////////////////////
void QtConfig::setInt(
        std::string key,
            int value) {
    setValue(key.c_str(), value);
    sync();
}

//////////////////////////////////////////////////////////
int QtConfig::getInt(
        std::string key,
            int defaultValue) {
    int i = (int)value(key.c_str(), defaultValue).toDouble();
    if (! contains(key.c_str())) {
    	setValue(key.c_str(), i);
    	sync();
    }	
    return i;
}

//////////////////////////////////////////////////////////
void QtConfig::setBool(
        std::string key,
            bool value) {
    setValue(key.c_str(), value);
    sync();
}

//////////////////////////////////////////////////////////
bool QtConfig::getBool(
        std::string key,
            bool defaultValue) {
    bool b = value(key.c_str(), defaultValue).toBool();
    if (! contains(key.c_str())) {
    	setValue(key.c_str(), b);
    	sync();
    }
    return b;
}

//////////////////////////////////////////////////////////
void QtConfig::setArray(
        std::string key,
            std::string subKey,
            std::vector<std::vector<int> > defaultValues) {
    std::vector<std::vector<int> > result;

    // Find out if the array exists, and its size
    int arraySize = beginReadArray(key.c_str());
    endArray();

    if (arraySize != 0) {
        // if the array already exists
    }

    // now create the array
    beginWriteArray(key.c_str());
    for (unsigned int i = 0; i < defaultValues.size(); i++) {
        setArrayIndex(i);
        QList<QVariant> variantList;
        for (unsigned int j = 0; j < defaultValues[i].size(); j++) {
            variantList.append(QVariant(defaultValues[i][j]));
        }
        setValue(subKey.c_str(), variantList);
        result.push_back(defaultValues[i]);
    }
    endArray();

    sync();

    return;
}
//////////////////////////////////////////////////////////
std::vector<std::vector<int> > QtConfig::getArray(
        std::string key,
            std::string subKey,
            std::vector<std::vector<int> > defaultValues) {
    std::vector<std::vector<int> > result;

    // Find out if the array exists, and its size
    unsigned int arraySize = beginReadArray(key.c_str());
    endArray();

    if (arraySize == 0) {
        // if the array doesn't exist
        // add the default values
        beginWriteArray(key.c_str());
        for (unsigned int i = 0; i < defaultValues.size(); i++) {
            setArrayIndex(i);
            QList<QVariant> variantList;
            for (unsigned int j = 0; j < defaultValues[i].size(); j++) {
                variantList.append(QVariant(defaultValues[i][j]));
            }
            setValue(subKey.c_str(), variantList);
            result.push_back(defaultValues[i]);
        }
        endArray();
    } else {
        beginReadArray(key.c_str());
        for (unsigned int i = 0; i < arraySize; i++) {
            setArrayIndex(i);
            QList<QVariant> variantList = value(subKey.c_str()).toList();
            std::vector<int> entry;
            for (int i = 0; i < variantList.size(); i++) {
                entry.push_back(variantList[i].toInt());
            }
            result.push_back(entry);
        }
        endArray();
    }

    sync();

    return result;
}
//////////////////////////////////////////////////////////
void QtConfig::setArray(std::string key,
		std::vector<std::map<std::string, std::string> > values) {
    std::vector<std::string> result;

    // Find out if the array exists, and its size
    int arraySize = beginReadArray(key.c_str());
    endArray();

    // now create the array
    beginWriteArray(key.c_str());
    for (unsigned int i = 0; i < values.size(); i++) {
        setArrayIndex(i);
        std::map<std::string, std::string> entry = values[i];
        std::map<std::string, std::string>::iterator j;
        for (j = entry.begin(); j != entry.end(); j++) {
        	setValue(QString(j->first.c_str()), QString(j->second.c_str()));
        }
    }
    endArray();

    sync();

    return;
}
//////////////////////////////////////////////////////////
std::vector<std::map<std::string, std::string> > QtConfig::getArray(
        std::string key,
        std::vector<std::map<std::string, std::string> > defaultValues) {
	std::vector<std::map<std::string, std::string> > result;

    // Find out if the array exists, and its size
    unsigned int arraySize = beginReadArray(key.c_str());
    endArray();

    if (arraySize == 0) {
        // if the array doesn't exist
        // add the default values
        beginWriteArray(key.c_str());
        for (unsigned int i = 0; i < defaultValues.size(); i++) {
            setArrayIndex(i);
            std::map<std::string, std::string> entry = defaultValues[i];
            std::map<std::string, std::string>::iterator j;
            for (j = entry.begin(); j != entry.end(); j++) {
            	setValue(QString(j->first.c_str()), QString(j->second.c_str()));
            }
        }
        endArray();
    } else {
        beginReadArray(key.c_str());
        for (unsigned int i = 0; i < arraySize; i++) {
            setArrayIndex(i);
        	QStringList keys = childKeys();
    		std::map<std::string, std::string> entry;
        	for (int i = 0; i < keys.size(); i++) {
        		std::string key = keys[i].toStdString();
        	    entry[key] =
        	    	value(key.c_str()).toString().toStdString();
        	}
            result.push_back(entry);
        }
        endArray();
    }

    sync();

    return result;
}
//////////////////////////////////////////////////////////
std::string QtConfig::fileName() {
	return QSettings::fileName().toStdString();
}
//////////////////////////////////////////////////////////
std::vector<std::string> QtConfig::getKeys() {
	std::vector<std::string> result;
	QStringList keys = allKeys();
	for (int i = 0; i < keys.size(); i++) {
		result.push_back(keys[i].toStdString());
	}
	return result;
}
