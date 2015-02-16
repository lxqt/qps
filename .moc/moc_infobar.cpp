/****************************************************************************
** Meta object code from reading C++ file 'infobar.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../infobar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'infobar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_subcpuRack[] = {

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

static const char qt_meta_stringdata_subcpuRack[] = {
    "subcpuRack\0"
};

void subcpuRack::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData subcpuRack::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject subcpuRack::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_subcpuRack,
      qt_meta_data_subcpuRack, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &subcpuRack::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *subcpuRack::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *subcpuRack::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_subcpuRack))
        return static_cast<void*>(const_cast< subcpuRack*>(this));
    return QWidget::qt_metacast(_clname);
}

int subcpuRack::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_Infobar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      19,    8,    8,    8, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Infobar[] = {
    "Infobar\0\0clicked()\0config_change()\0"
};

void Infobar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Infobar *_t = static_cast<Infobar *>(_o);
        switch (_id) {
        case 0: _t->clicked(); break;
        case 1: _t->config_change(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Infobar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Infobar::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_Infobar,
      qt_meta_data_Infobar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Infobar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Infobar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Infobar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Infobar))
        return static_cast<void*>(const_cast< Infobar*>(this));
    return QFrame::qt_metacast(_clname);
}

int Infobar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Infobar::clicked()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Infobar::config_change()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
static const uint qt_meta_data_GraphBase[] = {

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

static const char qt_meta_stringdata_GraphBase[] = {
    "GraphBase\0"
};

void GraphBase::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData GraphBase::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GraphBase::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_GraphBase,
      qt_meta_data_GraphBase, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GraphBase::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GraphBase::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GraphBase::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GraphBase))
        return static_cast<void*>(const_cast< GraphBase*>(this));
    return QWidget::qt_metacast(_clname);
}

int GraphBase::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_IO_Graph[] = {

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

static const char qt_meta_stringdata_IO_Graph[] = {
    "IO_Graph\0"
};

void IO_Graph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData IO_Graph::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject IO_Graph::staticMetaObject = {
    { &GraphBase::staticMetaObject, qt_meta_stringdata_IO_Graph,
      qt_meta_data_IO_Graph, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IO_Graph::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IO_Graph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IO_Graph::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IO_Graph))
        return static_cast<void*>(const_cast< IO_Graph*>(this));
    return GraphBase::qt_metacast(_clname);
}

int IO_Graph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = GraphBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_Infobar2[] = {

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

static const char qt_meta_stringdata_Infobar2[] = {
    "Infobar2\0"
};

void Infobar2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Infobar2::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Infobar2::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_Infobar2,
      qt_meta_data_Infobar2, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Infobar2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Infobar2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Infobar2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Infobar2))
        return static_cast<void*>(const_cast< Infobar2*>(this));
    return QFrame::qt_metacast(_clname);
}

int Infobar2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_PDisplay[] = {

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

static const char qt_meta_stringdata_PDisplay[] = {
    "PDisplay\0"
};

void PDisplay::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData PDisplay::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PDisplay::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PDisplay,
      qt_meta_data_PDisplay, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PDisplay::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PDisplay::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PDisplay::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PDisplay))
        return static_cast<void*>(const_cast< PDisplay*>(this));
    return QWidget::qt_metacast(_clname);
}

int PDisplay::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
