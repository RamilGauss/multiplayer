/****************************************************************************
** Meta object code from reading C++ file 'WaitForm.h'
**
** Created: Thu 17. May 23:37:22 2012
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../WaitForm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'WaitForm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_WaitForm[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x09,
      25,    9,    9,    9, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_WaitForm[] = {
    "WaitForm\0\0sl_ExitQueue()\0sl_Timer()\0"
};

const QMetaObject WaitForm::staticMetaObject = {
    { &TBaseGUI::staticMetaObject, qt_meta_stringdata_WaitForm,
      qt_meta_data_WaitForm, 0 }
};

const QMetaObject *WaitForm::metaObject() const
{
    return &staticMetaObject;
}

void *WaitForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WaitForm))
        return static_cast<void*>(const_cast< WaitForm*>(this));
    return TBaseGUI::qt_metacast(_clname);
}

int WaitForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TBaseGUI::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sl_ExitQueue(); break;
        case 1: sl_Timer(); break;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
