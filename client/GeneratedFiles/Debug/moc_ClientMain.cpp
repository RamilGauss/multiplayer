/****************************************************************************
** Meta object code from reading C++ file 'ClientMain.h'
**
** Created: Thu 3. May 16:00:28 2012
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ClientMain.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ClientMain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ClientMain[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x09,
      22,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_ClientMain[] = {
    "ClientMain\0\0sl_Exit()\0sl_Enter()\0"
};

const QMetaObject ClientMain::staticMetaObject = {
    { &TBaseGUI::staticMetaObject, qt_meta_stringdata_ClientMain,
      qt_meta_data_ClientMain, 0 }
};

const QMetaObject *ClientMain::metaObject() const
{
    return &staticMetaObject;
}

void *ClientMain::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ClientMain))
        return static_cast<void*>(const_cast< ClientMain*>(this));
    return TBaseGUI::qt_metacast(_clname);
}

int ClientMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TBaseGUI::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sl_Exit(); break;
        case 1: sl_Enter(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
