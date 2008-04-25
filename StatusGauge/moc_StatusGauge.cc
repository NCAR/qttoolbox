/****************************************************************************
** Meta object code from reading C++ file 'StatusGauge.h'
**
** Created: Thu Apr 24 11:49:23 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StatusGauge.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StatusGauge.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_StatusGauge[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_StatusGauge[] = {
    "StatusGauge\0\0released()\0"
};

const QMetaObject StatusGauge::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_StatusGauge,
      qt_meta_data_StatusGauge, 0 }
};

const QMetaObject *StatusGauge::metaObject() const
{
    return &staticMetaObject;
}

void *StatusGauge::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StatusGauge))
	return static_cast<void*>(const_cast<StatusGauge*>(this));
    return QWidget::qt_metacast(_clname);
}

int StatusGauge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: released(); break;
        }
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void StatusGauge::released()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
