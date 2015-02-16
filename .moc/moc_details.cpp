/****************************************************************************
** Meta object code from reading C++ file 'details.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../details.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'details.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Details[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Details[] = {
    "Details\0\0closed(Details*)\0"
};

void Details::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Details *_t = static_cast<Details *>(_o);
        switch (_id) {
        case 0: _t->closed((*reinterpret_cast< Details*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Details::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Details::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Details,
      qt_meta_data_Details, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Details::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Details::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Details::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Details))
        return static_cast<void*>(const_cast< Details*>(this));
    return QWidget::qt_metacast(_clname);
}

int Details::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void Details::closed(Details * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
static const uint qt_meta_data_SimpleTable[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

static const char qt_meta_stringdata_SimpleTable[] = {
    "SimpleTable\0"
};

void SimpleTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SimpleTable::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SimpleTable::staticMetaObject = {
    { &HeadedTable::staticMetaObject, qt_meta_stringdata_SimpleTable,
      qt_meta_data_SimpleTable, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SimpleTable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SimpleTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SimpleTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SimpleTable))
        return static_cast<void*>(const_cast< SimpleTable*>(this));
    return HeadedTable::qt_metacast(_clname);
}

int SimpleTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = HeadedTable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_Sockets[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,    9,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Sockets[] = {
    "Sockets\0\0addr\0update_hostname(uint)\0"
};

void Sockets::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Sockets *_t = static_cast<Sockets *>(_o);
        switch (_id) {
        case 0: _t->update_hostname((*reinterpret_cast< uint(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Sockets::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Sockets::staticMetaObject = {
    { &SimpleTable::staticMetaObject, qt_meta_stringdata_Sockets,
      qt_meta_data_Sockets, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Sockets::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Sockets::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Sockets::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Sockets))
        return static_cast<void*>(const_cast< Sockets*>(this));
    return SimpleTable::qt_metacast(_clname);
}

int Sockets::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SimpleTable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_Environ[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Environ[] = {
    "Environ\0\0sort_change(int)\0"
};

void Environ::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Environ *_t = static_cast<Environ *>(_o);
        switch (_id) {
        case 0: _t->sort_change((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Environ::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Environ::staticMetaObject = {
    { &SimpleTable::staticMetaObject, qt_meta_stringdata_Environ,
      qt_meta_data_Environ, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Environ::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Environ::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Environ::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Environ))
        return static_cast<void*>(const_cast< Environ*>(this));
    return SimpleTable::qt_metacast(_clname);
}

int Environ::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = SimpleTable::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
