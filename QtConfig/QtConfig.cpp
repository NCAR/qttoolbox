#include "QtConfig.h"
#include <QList>
#include <QStringList>
#include <QVariant>

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
