#include "QtConfig.h"
#include <QList>
#include <QStringList>
#include <QVariant>

//////////////////////////////////////////////////////////
QtConfig::QtConfig(
        const std::string organization,
            const std::string application) :
    _settings(QSettings::IniFormat,
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
    _settings.beginGroup(topGroup.c_str());

    // get the child groups
    QStringList children = _settings.childGroups();

    // save them
    for (int i = 0; i < children.size(); i++) {
        result.push_back(children[i].toStdString());
    }

    _settings.endGroup();

    return result;
}

//////////////////////////////////////////////////////////
void QtConfig::setString(
        std::string key,
            std::string t) {
    _settings.setValue(key.c_str(), t.c_str());
    sync();
}

//////////////////////////////////////////////////////////
std::string QtConfig::getString(
        std::string key,
            std::string defaultValue) {
    std::string s = std::string(_settings.value(key.c_str(),
            defaultValue.c_str()).toString().toAscii());

    _settings.setValue(key.c_str(), s.c_str());
    sync();

    return s;
}

//////////////////////////////////////////////////////////
void QtConfig::setDouble(
        std::string key,
            double value) {
    _settings.setValue(key.c_str(), value);
    sync();
}

//////////////////////////////////////////////////////////
double QtConfig::getDouble(
        std::string key,
            double defaultValue) {
    double d = _settings.value(key.c_str(), defaultValue).toDouble();
    _settings.setValue(key.c_str(), d);
    sync();
    return d;
}

//////////////////////////////////////////////////////////
void QtConfig::setFloat(
        std::string key,
            float value) {
    _settings.setValue(key.c_str(), value);
    sync();
}

//////////////////////////////////////////////////////////
float QtConfig::getFloat(
        std::string key,
            float defaultValue) {
    float d = _settings.value(key.c_str(), defaultValue).toDouble();
    _settings.setValue(key.c_str(), d);
    sync();
    return d;
}

//////////////////////////////////////////////////////////
void QtConfig::setInt(
        std::string key,
            int value) {
    _settings.setValue(key.c_str(), value);
    sync();
}

//////////////////////////////////////////////////////////
int QtConfig::getInt(
        std::string key,
            int defaultValue) {
    int i = (int)_settings.value(key.c_str(), defaultValue).toDouble();
    _settings.setValue(key.c_str(), i);
    sync();
    return i;
}

//////////////////////////////////////////////////////////
void QtConfig::setBool(
        std::string key,
            bool value) {
    _settings.setValue(key.c_str(), value);
    sync();
}

//////////////////////////////////////////////////////////
bool QtConfig::getBool(
        std::string key,
            bool defaultValue) {
    bool b = _settings.value(key.c_str(), defaultValue).toBool();
    _settings.setValue(key.c_str(), b);
    sync();
    return b;
}

//////////////////////////////////////////////////////////
void QtConfig::setArray(
        std::string key,
            std::string subKey,
            std::vector<std::vector<int> > defaultValues) {
    std::vector<std::vector<int> > result;

    // Find out if the array exists, and its size
    int arraySize = _settings.beginReadArray(key.c_str());
    _settings.endArray();

    if (arraySize != 0) {
        // if the array already exists
    }

    // now create the array
    _settings.beginWriteArray(key.c_str());
    for (unsigned int i = 0; i < defaultValues.size(); i++) {
        _settings.setArrayIndex(i);
        QList<QVariant> variantList;
        for (unsigned int j = 0; j < defaultValues[i].size(); j++) {
            variantList.append(QVariant(defaultValues[i][j]));
        }
        _settings.setValue(subKey.c_str(), variantList);
        result.push_back(defaultValues[i]);
    }
    _settings.endArray();

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
    unsigned int arraySize = _settings.beginReadArray(key.c_str());
    _settings.endArray();

    if (arraySize == 0) {
        // if the array doesn't exist
        // add the default values
        _settings.beginWriteArray(key.c_str());
        for (unsigned int i = 0; i < defaultValues.size(); i++) {
            _settings.setArrayIndex(i);
            QList<QVariant> variantList;
            for (unsigned int j = 0; j < defaultValues[i].size(); j++) {
                variantList.append(QVariant(defaultValues[i][j]));
            }
            _settings.setValue(subKey.c_str(), variantList);
            result.push_back(defaultValues[i]);
        }
        _settings.endArray();
    } else {
        _settings.beginReadArray(key.c_str());
        for (unsigned int i = 0; i < arraySize; i++) {
            _settings.setArrayIndex(i);
            QList<QVariant> variantList = _settings.value(subKey.c_str()).toList();
            std::vector<int> entry;
            for (int i = 0; i < variantList.size(); i++) {
                entry.push_back(variantList[i].toInt());
            }
            result.push_back(entry);
        }
        _settings.endArray();
    }

    sync();

    return result;
}
//////////////////////////////////////////////////////////
void QtConfig::sync() {
    _settings.sync();
}
