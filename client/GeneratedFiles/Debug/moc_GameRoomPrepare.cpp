/****************************************************************************
** Meta object code from reading C++ file 'GameRoomPrepare.h'
**
** Created: Mon 21. May 16:49:30 2012
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../GameRoomPrepare.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GameRoomPrepare.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_GameRoomPrepare[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x09,
      39,   16,   16,   16, 0x09,
      49,   16,   16,   16, 0x09,
      68,   16,   16,   16, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_GameRoomPrepare[] = {
    "GameRoomPrepare\0\0sl_RefreshCntClient()\0"
    "sl_Exit()\0sl_ChangeCurTank()\0sl_InFight()\0"
};

const QMetaObject GameRoomPrepare::staticMetaObject = {
    { &TBaseGUI::staticMetaObject, qt_meta_stringdata_GameRoomPrepare,
      qt_meta_data_GameRoomPrepare, 0 }
};

const QMetaObject *GameRoomPrepare::metaObject() const
{
    return &staticMetaObject;
}

void *GameRoomPrepare::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GameRoomPrepare))
        return static_cast<void*>(const_cast< GameRoomPrepare*>(this));
    return TBaseGUI::qt_metacast(_clname);
}

int GameRoomPrepare::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = TBaseGUI::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sl_RefreshCntClient(); break;
        case 1: sl_Exit(); break;
        case 2: sl_ChangeCurTank(); break;
        case 3: sl_InFight(); break;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
