/****************************************************************************
** Meta object code from reading C++ file 'StatusGaugeTest.h'
**
** Created: Thu Apr 24 11:50:20 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "StatusGaugeTest.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StatusGaugeTest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_StatusGaugeTest[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_StatusGaugeTest[] = {
    "StatusGaugeTest\0\0buttonSlot(int)\0"
};

const QMetaObject StatusGaugeTest::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_StatusGaugeTest,
      qt_meta_data_StatusGaugeTest, 0 }
};

const QMetaObject *StatusGaugeTest::metaObject() const
{
    return &staticMetaObject;
}

void *StatusGaugeTest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StatusGaugeTest))
	return static_cast<void*>(const_cast<StatusGaugeTest*>(this));
    if (!strcmp(_clname, "Ui::StatusGaugeTest"))
	return static_cast<Ui::StatusGaugeTest*>(const_cast<StatusGaugeTest*>(this));
    return QWidget::qt_metacast(_clname);
}

int StatusGaugeTest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: buttonSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 1;
    }
    return _id;
}
