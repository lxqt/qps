/****************************************************************************
** Meta object code from reading C++ file 'htable.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../htable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'htable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VPointer[] = {

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

static const char qt_meta_stringdata_VPointer[] = {
    "VPointer\0"
};

void VPointer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData VPointer::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject VPointer::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VPointer,
      qt_meta_data_VPointer, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VPointer::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VPointer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VPointer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VPointer))
        return static_cast<void*>(const_cast< VPointer*>(this));
    return QWidget::qt_metacast(_clname);
}

int VPointer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_FloatingHead[] = {

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

static const char qt_meta_stringdata_FloatingHead[] = {
    "FloatingHead\0"
};

void FloatingHead::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData FloatingHead::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FloatingHead::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FloatingHead,
      qt_meta_data_FloatingHead, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FloatingHead::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FloatingHead::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FloatingHead::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FloatingHead))
        return static_cast<void*>(const_cast< FloatingHead*>(this));
    return QWidget::qt_metacast(_clname);
}

int FloatingHead::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_TableHead[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   11,   10,   10, 0x05,
      46,   11,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      66,   10,   10,   10, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_TableHead[] = {
    "TableHead\0\0where,col\0rightClicked(QPoint,int)\0"
    "toolTip(QPoint,int)\0scrollSideways(int)\0"
};

void TableHead::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TableHead *_t = static_cast<TableHead *>(_o);
        switch (_id) {
        case 0: _t->rightClicked((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->toolTip((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->scrollSideways((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TableHead::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TableHead::staticMetaObject = {
    { &QtTableView::staticMetaObject, qt_meta_stringdata_TableHead,
      qt_meta_data_TableHead, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TableHead::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TableHead::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TableHead::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TableHead))
        return static_cast<void*>(const_cast< TableHead*>(this));
    return QtTableView::qt_metacast(_clname);
}

int TableHead::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtTableView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void TableHead::rightClicked(QPoint _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TableHead::toolTip(QPoint _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
static const uint qt_meta_data_TableBody[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x09,
      21,   10,   10,   10, 0x09,
      38,   34,   10,   10, 0x09,
      68,   60,   10,   10, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_TableBody[] = {
    "TableBody\0\0jumpTop()\0jumpBottom()\0row\0"
    "centerVertically(int)\0from,to\0"
    "showRange(int,int)\0"
};

void TableBody::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TableBody *_t = static_cast<TableBody *>(_o);
        switch (_id) {
        case 0: _t->jumpTop(); break;
        case 1: _t->jumpBottom(); break;
        case 2: _t->centerVertically((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->showRange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TableBody::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TableBody::staticMetaObject = {
    { &QtTableView::staticMetaObject, qt_meta_stringdata_TableBody,
      qt_meta_data_TableBody, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TableBody::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TableBody::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TableBody::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TableBody))
        return static_cast<void*>(const_cast< TableBody*>(this));
    return QtTableView::qt_metacast(_clname);
}

int TableBody::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QtTableView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
static const uint qt_meta_data_HeadedTable[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   13,   12,   12, 0x05,
      35,   12,   12,   12, 0x05,
      58,   54,   12,   12, 0x05,
      83,   77,   12,   12, 0x05,
     104,   54,   12,   12, 0x05,
     131,  121,   12,   12, 0x05,
     157,  149,   12,   12, 0x05,
     176,   13,   12,   12, 0x05,
     192,   12,   12,   12, 0x05,
     204,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
     217,   12,   12,   12, 0x0a,
     229,   12,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_HeadedTable[] = {
    "HeadedTable\0\0col\0titleClicked(int)\0"
    "selectionChanged()\0row\0doubleClicked(int)\0"
    "where\0rightClicked(QPoint)\0foldSubTree(int)\0"
    "col,place\0colMoved(int,int)\0row,col\0"
    "flyOnCell(int,int)\0flyOnHCell(int)\0"
    "outOfCell()\0outOfHCell()\0selectAll()\0"
    "repaintAll()\0"
};

void HeadedTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HeadedTable *_t = static_cast<HeadedTable *>(_o);
        switch (_id) {
        case 0: _t->titleClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->selectionChanged(); break;
        case 2: _t->doubleClicked((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->rightClicked((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 4: _t->foldSubTree((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->colMoved((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->flyOnCell((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->flyOnHCell((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->outOfCell(); break;
        case 9: _t->outOfHCell(); break;
        case 10: _t->selectAll(); break;
        case 11: _t->repaintAll(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HeadedTable::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HeadedTable::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_HeadedTable,
      qt_meta_data_HeadedTable, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HeadedTable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HeadedTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HeadedTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HeadedTable))
        return static_cast<void*>(const_cast< HeadedTable*>(this));
    return QWidget::qt_metacast(_clname);
}

int HeadedTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void HeadedTable::titleClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HeadedTable::selectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void HeadedTable::doubleClicked(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HeadedTable::rightClicked(QPoint _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void HeadedTable::foldSubTree(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void HeadedTable::colMoved(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void HeadedTable::flyOnCell(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void HeadedTable::flyOnHCell(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void HeadedTable::outOfCell()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void HeadedTable::outOfHCell()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}
QT_END_MOC_NAMESPACE
