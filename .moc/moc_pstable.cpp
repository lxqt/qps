/****************************************************************************
** Meta object code from reading C++ file 'pstable.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../pstable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'pstable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Pstable[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,    9,    8,    8, 0x0a,
      36,   32,    8,    8, 0x0a,
      64,   56,    8,    8, 0x0a,
      93,   84,    8,    8, 0x0a,
     119,  111,    8,    8, 0x0a,
     140,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Pstable[] = {
    "Pstable\0\0col\0setSortColumn(int)\0row\0"
    "subtree_folded(int)\0p,index\0"
    "showTip(QPoint,int)\0treemode\0"
    "setTreeMode(bool)\0row,col\0"
    "mouseOnCell(int,int)\0mouseOutOfCell()\0"
};

void Pstable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Pstable *_t = static_cast<Pstable *>(_o);
        switch (_id) {
        case 0: _t->setSortColumn((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->subtree_folded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->showTip((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->setTreeMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->mouseOnCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->mouseOutOfCell(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Pstable::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Pstable::staticMetaObject = {
    { &HeadedTable::staticMetaObject, qt_meta_stringdata_Pstable,
      qt_meta_data_Pstable, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Pstable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Pstable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Pstable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Pstable))
        return static_cast<void*>(const_cast< Pstable*>(this));
    return HeadedTable::qt_metacast(_clname);
}

int Pstable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = HeadedTable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
