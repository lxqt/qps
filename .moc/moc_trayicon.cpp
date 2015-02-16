/****************************************************************************
** Meta object code from reading C++ file 'trayicon.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../trayicon.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'trayicon.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TrayIcon[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: signature, parameters, type, tag, flags
      10,    9,    9,    9, 0x05,
      28,   26,    9,    9, 0x05,
      48,    9,    9,    9, 0x05,
      70,    9,    9,    9, 0x05,

 // slots: signature, parameters, type, tag, flags
      84,   79,    9,    9, 0x0a,
     105,  101,    9,    9, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_TrayIcon[] = {
    "TrayIcon\0\0clicked(QPoint)\0,\0"
    "clicked(QPoint,int)\0doubleClicked(QPoint)\0"
    "closed()\0icon\0setIcon(QPixmap)\0tip\0"
    "setToolTip(QString)\0"
};

void TrayIcon::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TrayIcon *_t = static_cast<TrayIcon *>(_o);
        switch (_id) {
        case 0: _t->clicked((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 1: _t->clicked((*reinterpret_cast< const QPoint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->doubleClicked((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 3: _t->closed(); break;
        case 4: _t->setIcon((*reinterpret_cast< const QPixmap(*)>(_a[1]))); break;
        case 5: _t->setToolTip((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TrayIcon::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TrayIcon::staticMetaObject = {
    { &QSystemTrayIcon::staticMetaObject, qt_meta_stringdata_TrayIcon,
      qt_meta_data_TrayIcon, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TrayIcon::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TrayIcon::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TrayIcon::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TrayIcon))
        return static_cast<void*>(const_cast< TrayIcon*>(this));
    return QSystemTrayIcon::qt_metacast(_clname);
}

int TrayIcon::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QSystemTrayIcon::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void TrayIcon::clicked(const QPoint & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TrayIcon::clicked(const QPoint & _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void TrayIcon::doubleClicked(const QPoint & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void TrayIcon::closed()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}
QT_END_MOC_NAMESPACE
