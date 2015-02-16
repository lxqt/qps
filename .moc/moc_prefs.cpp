/****************************************************************************
** Meta object code from reading C++ file 'prefs.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../prefs.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'prefs.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Preferences[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      28,   12,   12,   12, 0x0a,
      43,   12,   12,   12, 0x0a,
      60,   12,   12,   12, 0x0a,
      76,   12,   12,   12, 0x0a,
      85,   12,   12,   12, 0x0a,
     103,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Preferences[] = {
    "Preferences\0\0prefs_change()\0update_boxes()\0"
    "update_reality()\0update_config()\0"
    "closed()\0font_changed(int)\0"
    "fontset_changed(int)\0"
};

void Preferences::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Preferences *_t = static_cast<Preferences *>(_o);
        switch (_id) {
        case 0: _t->prefs_change(); break;
        case 1: _t->update_boxes(); break;
        case 2: _t->update_reality(); break;
        case 3: _t->update_config(); break;
        case 4: _t->closed(); break;
        case 5: _t->font_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->fontset_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Preferences::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Preferences::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Preferences,
      qt_meta_data_Preferences, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Preferences::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Preferences::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Preferences::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Preferences))
        return static_cast<void*>(const_cast< Preferences*>(this));
    return QDialog::qt_metacast(_clname);
}

int Preferences::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void Preferences::prefs_change()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
