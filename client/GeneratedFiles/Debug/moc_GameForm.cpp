/****************************************************************************
** Meta object code from reading C++ file 'GameForm.h'
**
** Created: Wed 4. Jul 22:16:37 2012
**      by: The Qt Meta Object Compiler version 61 (Qt 4.5.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GameForm.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GameForm.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 61
#error "This file was generated using the moc from 4.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GameForm[] = {

 // content:
       2,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   12, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors

 // slots: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x09,
      20,    9,    9,    9, 0x09,
      30,    9,    9,    9, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_GameForm[] = {
    "GameForm\0\0sl_Exit()\0showGUI()\0hideGUI()\0"
};

const QMetaObject GameForm::staticMetaObject = {
    { &TBaseGUI::staticMetaObject, qt_meta_stringdata_GameForm,
      qt_meta_data_GameForm, 0 }
};

const QMetaObject *GameForm::metaObject() const
{
    return &staticMetaObject;
}

void *GameForm::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GameForm))
        return static_cast<void*>(const_cast< GameForm*>(this));
    return TBaseGUI::qt_metacast(_clname);
}

int GameForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TBaseGUI::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sl_Exit(); break;
        case 1: showGUI(); break;
        case 2: hideGUI(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
