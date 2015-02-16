/****************************************************************************
** Meta object code from reading C++ file 'command.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../command.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'command.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CommandModel[] = {

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

static const char qt_meta_stringdata_CommandModel[] = {
    "CommandModel\0"
};

void CommandModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData CommandModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CommandModel::staticMetaObject = {
    { &QAbstractItemModel::staticMetaObject, qt_meta_stringdata_CommandModel,
      qt_meta_data_CommandModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CommandModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CommandModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CommandModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CommandModel))
        return static_cast<void*>(const_cast< CommandModel*>(this));
    return QAbstractItemModel::qt_metacast(_clname);
}

int CommandModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractItemModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_CommandDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      32,   14,   14,   14, 0x09,
      42,   14,   14,   14, 0x09,
      52,   14,   14,   14, 0x09,
      66,   14,   14,   14, 0x09,
      83,   14,   14,   14, 0x09,
      91,   14,   14,   14, 0x09,
     124,  115,   14,   14, 0x09,
     153,   14,   14,   14, 0x09,
     174,   14,   14,   14, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_CommandDialog[] = {
    "CommandDialog\0\0command_change()\0"
    "new_cmd()\0add_new()\0del_current()\0"
    "set_buttons(int)\0reset()\0"
    "set_select(QModelIndex)\0new_name\0"
    "event_name_midified(QString)\0"
    "event_cmd_modified()\0event_toolbar_checked(bool)\0"
};

void CommandDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CommandDialog *_t = static_cast<CommandDialog *>(_o);
        switch (_id) {
        case 0: _t->command_change(); break;
        case 1: _t->new_cmd(); break;
        case 2: _t->add_new(); break;
        case 3: _t->del_current(); break;
        case 4: _t->set_buttons((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->reset(); break;
        case 6: _t->set_select((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 7: _t->event_name_midified((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->event_cmd_modified(); break;
        case 9: _t->event_toolbar_checked((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CommandDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CommandDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CommandDialog,
      qt_meta_data_CommandDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CommandDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CommandDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CommandDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CommandDialog))
        return static_cast<void*>(const_cast< CommandDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int CommandDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void CommandDialog::command_change()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
static const uint qt_meta_data_checkBoxDelegate[] = {

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

static const char qt_meta_stringdata_checkBoxDelegate[] = {
    "checkBoxDelegate\0"
};

void checkBoxDelegate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData checkBoxDelegate::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject checkBoxDelegate::staticMetaObject = {
    { &QItemDelegate::staticMetaObject, qt_meta_stringdata_checkBoxDelegate,
      qt_meta_data_checkBoxDelegate, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &checkBoxDelegate::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *checkBoxDelegate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *checkBoxDelegate::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_checkBoxDelegate))
        return static_cast<void*>(const_cast< checkBoxDelegate*>(this));
    return QItemDelegate::qt_metacast(_clname);
}

int checkBoxDelegate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QItemDelegate::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_ListModel[] = {

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

static const char qt_meta_stringdata_ListModel[] = {
    "ListModel\0"
};

void ListModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ListModel::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ListModel::staticMetaObject = {
    { &QAbstractTableModel::staticMetaObject, qt_meta_stringdata_ListModel,
      qt_meta_data_ListModel, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ListModel::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ListModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ListModel::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ListModel))
        return static_cast<void*>(const_cast< ListModel*>(this));
    return QAbstractTableModel::qt_metacast(_clname);
}

int ListModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractTableModel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_WatchdogDialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x09,
      23,   15,   15,   15, 0x09,
      31,   15,   15,   15, 0x09,
      37,   15,   15,   15, 0x09,
      47,   43,   15,   15, 0x09,
      68,   43,   15,   15, 0x09,
      85,   15,   15,   15, 0x09,
     107,  103,   15,   15, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_WatchdogDialog[] = {
    "WatchdogDialog\0\0_new()\0apply()\0add()\0"
    "del()\0str\0condChanged(QString)\0"
    "Changed(QString)\0comboChanged(int)\0"
    "idx\0eventcat_slected(QModelIndex)\0"
};

void WatchdogDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        WatchdogDialog *_t = static_cast<WatchdogDialog *>(_o);
        switch (_id) {
        case 0: _t->_new(); break;
        case 1: _t->apply(); break;
        case 2: _t->add(); break;
        case 3: _t->del(); break;
        case 4: _t->condChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->Changed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->comboChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->eventcat_slected((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData WatchdogDialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject WatchdogDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_WatchdogDialog,
      qt_meta_data_WatchdogDialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &WatchdogDialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *WatchdogDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *WatchdogDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_WatchdogDialog))
        return static_cast<void*>(const_cast< WatchdogDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int WatchdogDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}
static const uint qt_meta_data_ExecWindow[] = {

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
      12,   11,   11,   11, 0x09,
      46,   26,   11,   11, 0x09,
      91,   85,   11,   11, 0x09,
     125,   11,   11,   11, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_ExecWindow[] = {
    "ExecWindow\0\0cmd_started()\0exitCode,exitStatus\0"
    "cmd_finished(int,QProcess::ExitStatus)\0"
    "error\0cmd_error(QProcess::ProcessError)\0"
    "cmd_ok()\0"
};

void ExecWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ExecWindow *_t = static_cast<ExecWindow *>(_o);
        switch (_id) {
        case 0: _t->cmd_started(); break;
        case 1: _t->cmd_finished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 2: _t->cmd_error((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        case 3: _t->cmd_ok(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ExecWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ExecWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ExecWindow,
      qt_meta_data_ExecWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ExecWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ExecWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ExecWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExecWindow))
        return static_cast<void*>(const_cast< ExecWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int ExecWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
