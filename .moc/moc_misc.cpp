/****************************************************************************
** Meta object code from reading C++ file 'misc.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../misc.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'misc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_TFrame[] = {

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
      12,    8,    7,    7, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_TFrame[] = {
    "TFrame\0\0val\0setValue(int)\0"
};

void TFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TFrame *_t = static_cast<TFrame *>(_o);
        switch (_id) {
        case 0: _t->setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TFrame::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TFrame::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_TFrame,
      qt_meta_data_TFrame, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TFrame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TFrame))
        return static_cast<void*>(const_cast< TFrame*>(this));
    return QLabel::qt_metacast(_clname);
}

int TFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_TBloon[] = {

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
      12,    8,    7,    7, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_TBloon[] = {
    "TBloon\0\0val\0update(int)\0"
};

void TBloon::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        TBloon *_t = static_cast<TBloon *>(_o);
        switch (_id) {
        case 0: _t->update((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData TBloon::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject TBloon::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_TBloon,
      qt_meta_data_TBloon, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &TBloon::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *TBloon::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *TBloon::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_TBloon))
        return static_cast<void*>(const_cast< TBloon*>(this));
    return QLabel::qt_metacast(_clname);
}

int TBloon::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_FaderWidget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       2,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      19,   12, 0x42095103,
      33,   29, 0x02095103,

       0        // eod
};

static const char qt_meta_stringdata_FaderWidget[] = {
    "FaderWidget\0QBrush\0fadeBrush\0int\0"
    "fadeDuration\0"
};

void FaderWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData FaderWidget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FaderWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_FaderWidget,
      qt_meta_data_FaderWidget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FaderWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FaderWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FaderWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FaderWidget))
        return static_cast<void*>(const_cast< FaderWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int FaderWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QBrush*>(_v) = fadeBrush(); break;
        case 1: *reinterpret_cast< int*>(_v) = fadeDuration(); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setFadeBrush(*reinterpret_cast< QBrush*>(_v)); break;
        case 1: setFadeDuration(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 2;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 2;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 2;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
static const uint qt_meta_data_UFrame[] = {

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

static const char qt_meta_stringdata_UFrame[] = {
    "UFrame\0"
};

void UFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData UFrame::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject UFrame::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_UFrame,
      qt_meta_data_UFrame, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &UFrame::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *UFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *UFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UFrame))
        return static_cast<void*>(const_cast< UFrame*>(this));
    return QFrame::qt_metacast(_clname);
}

int UFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_XButton[] = {

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

static const char qt_meta_stringdata_XButton[] = {
    "XButton\0"
};

void XButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData XButton::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject XButton::staticMetaObject = {
    { &QAbstractButton::staticMetaObject, qt_meta_stringdata_XButton,
      qt_meta_data_XButton, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &XButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *XButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *XButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_XButton))
        return static_cast<void*>(const_cast< XButton*>(this));
    return QAbstractButton::qt_metacast(_clname);
}

int XButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QAbstractButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_SearchBox[] = {

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
      11,   10,   10,   10, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_SearchBox[] = {
    "SearchBox\0\0event_xbutton_clicked()\0"
};

void SearchBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SearchBox *_t = static_cast<SearchBox *>(_o);
        switch (_id) {
        case 0: _t->event_xbutton_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData SearchBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SearchBox::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_SearchBox,
      qt_meta_data_SearchBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SearchBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SearchBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SearchBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SearchBox))
        return static_cast<void*>(const_cast< SearchBox*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int SearchBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_LogBox[] = {

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

static const char qt_meta_stringdata_LogBox[] = {
    "LogBox\0"
};

void LogBox::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData LogBox::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject LogBox::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_LogBox,
      qt_meta_data_LogBox, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &LogBox::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *LogBox::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *LogBox::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_LogBox))
        return static_cast<void*>(const_cast< LogBox*>(this));
    return QLabel::qt_metacast(_clname);
}

int LogBox::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_StatusBar[] = {

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

static const char qt_meta_stringdata_StatusBar[] = {
    "StatusBar\0"
};

void StatusBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData StatusBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject StatusBar::staticMetaObject = {
    { &QStatusBar::staticMetaObject, qt_meta_stringdata_StatusBar,
      qt_meta_data_StatusBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StatusBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StatusBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StatusBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StatusBar))
        return static_cast<void*>(const_cast< StatusBar*>(this));
    return QStatusBar::qt_metacast(_clname);
}

int StatusBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QStatusBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_ControlBar[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   12,   11,   11, 0x05,
      38,   11,   11,   11, 0x05,
      59,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      74,   11,   11,   11, 0x0a,
      95,   91,   11,   11, 0x0a,
     113,   11,   11,   11, 0x0a,
     128,   11,   11,   11, 0x0a,
     150,   11,   11,   11, 0x0a,
     177,   11,   11,   11, 0x0a,
     201,   11,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ControlBar[] = {
    "ControlBar\0\0treemode\0modeChange(bool)\0"
    "viewChange(QAction*)\0need_refresh()\0"
    "linear_clicked()\0idx\0view_changed(int)\0"
    "tree_clicked()\0show_thread_clicked()\0"
    "event_search_box_changed()\0"
    "event_command_pressed()\0setPaused(bool)\0"
};

void ControlBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ControlBar *_t = static_cast<ControlBar *>(_o);
        switch (_id) {
        case 0: _t->modeChange((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->viewChange((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 2: _t->need_refresh(); break;
        case 3: _t->linear_clicked(); break;
        case 4: _t->view_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->tree_clicked(); break;
        case 6: _t->show_thread_clicked(); break;
        case 7: _t->event_search_box_changed(); break;
        case 8: _t->event_command_pressed(); break;
        case 9: _t->setPaused((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ControlBar::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ControlBar::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_ControlBar,
      qt_meta_data_ControlBar, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ControlBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ControlBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ControlBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ControlBar))
        return static_cast<void*>(const_cast< ControlBar*>(this));
    return QFrame::qt_metacast(_clname);
}

int ControlBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
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
void ControlBar::modeChange(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ControlBar::viewChange(QAction * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ControlBar::need_refresh()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}
static const uint qt_meta_data_QTabWidgetX[] = {

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

static const char qt_meta_stringdata_QTabWidgetX[] = {
    "QTabWidgetX\0"
};

void QTabWidgetX::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData QTabWidgetX::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QTabWidgetX::staticMetaObject = {
    { &QTabWidget::staticMetaObject, qt_meta_stringdata_QTabWidgetX,
      qt_meta_data_QTabWidgetX, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QTabWidgetX::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QTabWidgetX::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QTabWidgetX::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QTabWidgetX))
        return static_cast<void*>(const_cast< QTabWidgetX*>(this));
    return QTabWidget::qt_metacast(_clname);
}

int QTabWidgetX::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTabWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
static const uint qt_meta_data_ServerAdaptor[] = {

 // content:
       6,       // revision
       0,       // classname
       2,   14, // classinfo
       2,   18, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // classinfo: key, value
      50,   14,
     299,   66,

 // signals: signature, parameters, type, tag, flags
     320,  319,  319,  319, 0x05,

 // slots: signature, parameters, type, tag, flags
     330,  319,  319,  319, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_ServerAdaptor[] = {
    "ServerAdaptor\0com.trolltech.Examples.CarInterface\0"
    "D-Bus Interface\0"
    "  <interface name=\"com.trolltech.Examples.CarInterface\" >\n    <meth"
    "od name=\"accelerate\" />\n    <method name=\"decelerate\" />\n    <me"
    "thod name=\"turnLeft\" />\n    <method name=\"turnRight\" />\n    <sig"
    "nal name=\"crashed\" />\n  </interface>\n\0"
    "D-Bus Introspection\0\0crashed()\0"
    "accelerate()\0"
};

void ServerAdaptor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ServerAdaptor *_t = static_cast<ServerAdaptor *>(_o);
        switch (_id) {
        case 0: _t->crashed(); break;
        case 1: _t->accelerate(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData ServerAdaptor::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ServerAdaptor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ServerAdaptor,
      qt_meta_data_ServerAdaptor, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ServerAdaptor::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ServerAdaptor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ServerAdaptor::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ServerAdaptor))
        return static_cast<void*>(const_cast< ServerAdaptor*>(this));
    return QObject::qt_metacast(_clname);
}

int ServerAdaptor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
void ServerAdaptor::crashed()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
