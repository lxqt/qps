/****************************************************************************
** Meta object code from reading C++ file 'dialogs.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../dialogs.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dialogs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_IntervalDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x09,
      30,   15,   15,   15, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_IntervalDialog[] = {
    "IntervalDialog\0\0done_dialog()\0"
    "event_label_changed()\0"
};

void IntervalDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        IntervalDialog *_t = static_cast<IntervalDialog *>(_o);
        switch (_id) {
        case 0: _t->done_dialog(); break;
        case 1: _t->event_label_changed(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData IntervalDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject IntervalDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_IntervalDialog,
      qt_meta_data_IntervalDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &IntervalDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *IntervalDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *IntervalDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_IntervalDialog))
        return static_cast<void*>(const_cast< IntervalDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int IntervalDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_SliderDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   14,   13,   13, 0x09,
      37,   13,   13,   13, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_SliderDialog[] = {
    "SliderDialog\0\0val\0slider_change(int)\0"
    "done_dialog()\0"
};

void SliderDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SliderDialog *_t = static_cast<SliderDialog *>(_o);
        switch (_id) {
        case 0: _t->slider_change((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->done_dialog(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SliderDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SliderDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SliderDialog,
      qt_meta_data_SliderDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SliderDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SliderDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SliderDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SliderDialog))
        return static_cast<void*>(const_cast< SliderDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SliderDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
static const uint qt_meta_data_PermissionDialog[] = {

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

static const char qt_meta_stringdata_PermissionDialog[] = {
    "PermissionDialog\0"
};

void PermissionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData PermissionDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject PermissionDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_PermissionDialog,
      qt_meta_data_PermissionDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PermissionDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PermissionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PermissionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PermissionDialog))
        return static_cast<void*>(const_cast< PermissionDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int PermissionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SchedDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x09,
      27,   12,   12,   12, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_SchedDialog[] = {
    "SchedDialog\0\0done_dialog()\0"
    "button_clicked(bool)\0"
};

void SchedDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SchedDialog *_t = static_cast<SchedDialog *>(_o);
        switch (_id) {
        case 0: _t->done_dialog(); break;
        case 1: _t->button_clicked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SchedDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SchedDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_SchedDialog,
      qt_meta_data_SchedDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SchedDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SchedDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SchedDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SchedDialog))
        return static_cast<void*>(const_cast< SchedDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int SchedDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
