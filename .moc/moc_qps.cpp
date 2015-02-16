/****************************************************************************
** Meta object code from reading C++ file 'qps.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../qps.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qps.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Qps[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      49,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       5,    4,    4,    4, 0x0a,
      24,    4,    4,    4, 0x0a,
      76,    4,    4,    4, 0x0a,
      95,    4,    4,    4, 0x0a,
     106,    4,    4,    4, 0x0a,
     116,    4,    4,    4, 0x0a,
     127,    4,    4,    4, 0x0a,
     138,    4,    4,    4, 0x0a,
     149,    4,    4,    4, 0x0a,
     171,    4,    4,    4, 0x0a,
     193,    4,    4,    4, 0x0a,
     201,    4,    4,    4, 0x0a,
     211,    4,    4,    4, 0x0a,
     225,    4,    4,    4, 0x0a,
     244,    4,    4,    4, 0x0a,
     266,    4,    4,    4, 0x0a,
     288,    4,    4,    4, 0x0a,
     312,    4,    4,    4, 0x0a,
     332,    4,    4,    4, 0x0a,
     345,    4,    4,    4, 0x0a,
     359,    4,    4,    4, 0x0a,
     372,    4,    4,    4, 0x0a,
     388,    4,    4,    4, 0x0a,
     402,    4,    4,    4, 0x0a,
     418,    4,    4,    4, 0x0a,
     433,    4,    4,    4, 0x0a,
     447,    4,    4,    4, 0x0a,
     467,    4,    4,    4, 0x0a,
     486,  483,    4,    4, 0x0a,
     500,    4,    4,    4, 0x0a,
     520,    4,    4,    4, 0x0a,
     540,    4,    4,    4, 0x0a,
     552,  483,    4,    4, 0x0a,
     577,  573,    4,    4, 0x0a,
     605,  603,    4,    4, 0x0a,
     623,  621,    4,    4, 0x0a,
     653,  647,    4,    4, 0x0a,
     686,    4,    4,    4, 0x0a,
     723,  717,    4,    4, 0x0a,
     740,  717,    4,    4, 0x0a,
     768,  759,    4,    4, 0x0a,
     791,  789,    4,    4, 0x0a,
     820,  816,    4,    4, 0x0a,
     838,    4,    4,    4, 0x0a,
     854,    4,    4,    4, 0x0a,
     869,    4,    4,    4, 0x0a,
     879,    4,    4,    4, 0x0a,
     902,  897,    4,    4, 0x0a,
     919,    4,    4,    4, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Qps[] = {
    "Qps\0\0start_screenshot()\0"
    "clicked_trayicon(QSystemTrayIcon::ActivationReason)\0"
    "clicked_trayicon()\0sig_term()\0sig_hup()\0"
    "sig_stop()\0sig_cont()\0sig_kill()\0"
    "signal_menu(QAction*)\0run_command(QAction*)\0"
    "about()\0license()\0menu_update()\0"
    "menu_toggle_path()\0menu_toggle_infobar()\0"
    "menu_toggle_ctrlbar()\0menu_toggle_statusbar()\0"
    "menu_toggle_cumul()\0menu_prefs()\0"
    "menu_renice()\0menu_sched()\0Action_Detail()\0"
    "menu_parent()\0menu_children()\0"
    "menu_dynasty()\0menu_custom()\0"
    "menu_remove_field()\0menu_edit_cmd()\0"
    "id\0mig_menu(int)\0make_command_menu()\0"
    "view_menu(QAction*)\0save_quit()\0"
    "add_fields_menu(int)\0act\0"
    "add_fields_menu(QAction*)\0i\0tabChanged(int)\0"
    "p\0show_popup_menu(QPoint)\0p,col\0"
    "context_heading_menu(QPoint,int)\0"
    "update_menu_selection_status()\0index\0"
    "field_added(int)\0field_removed(int)\0"
    "treemode\0set_table_mode(bool)\0d\0"
    "details_closed(Details*)\0row\0"
    "open_details(int)\0config_change()\0"
    "update_timer()\0refresh()\0refresh_details()\0"
    "link\0test_popup(QUrl)\0update_menu_status()\0"
};

void Qps::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Qps *_t = static_cast<Qps *>(_o);
        switch (_id) {
        case 0: _t->start_screenshot(); break;
        case 1: _t->clicked_trayicon((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 2: _t->clicked_trayicon(); break;
        case 3: _t->sig_term(); break;
        case 4: _t->sig_hup(); break;
        case 5: _t->sig_stop(); break;
        case 6: _t->sig_cont(); break;
        case 7: _t->sig_kill(); break;
        case 8: _t->signal_menu((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 9: _t->run_command((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 10: _t->about(); break;
        case 11: _t->license(); break;
        case 12: _t->menu_update(); break;
        case 13: _t->menu_toggle_path(); break;
        case 14: _t->menu_toggle_infobar(); break;
        case 15: _t->menu_toggle_ctrlbar(); break;
        case 16: _t->menu_toggle_statusbar(); break;
        case 17: _t->menu_toggle_cumul(); break;
        case 18: _t->menu_prefs(); break;
        case 19: _t->menu_renice(); break;
        case 20: _t->menu_sched(); break;
        case 21: _t->Action_Detail(); break;
        case 22: _t->menu_parent(); break;
        case 23: _t->menu_children(); break;
        case 24: _t->menu_dynasty(); break;
        case 25: _t->menu_custom(); break;
        case 26: _t->menu_remove_field(); break;
        case 27: _t->menu_edit_cmd(); break;
        case 28: _t->mig_menu((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->make_command_menu(); break;
        case 30: _t->view_menu((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 31: _t->save_quit(); break;
        case 32: _t->add_fields_menu((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 33: _t->add_fields_menu((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 34: _t->tabChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 35: _t->show_popup_menu((*reinterpret_cast< QPoint(*)>(_a[1]))); break;
        case 36: _t->context_heading_menu((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 37: _t->update_menu_selection_status(); break;
        case 38: _t->field_added((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 39: _t->field_removed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 40: _t->set_table_mode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 41: _t->details_closed((*reinterpret_cast< Details*(*)>(_a[1]))); break;
        case 42: _t->open_details((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 43: _t->config_change(); break;
        case 44: _t->update_timer(); break;
        case 45: _t->refresh(); break;
        case 46: _t->refresh_details(); break;
        case 47: _t->test_popup((*reinterpret_cast< const QUrl(*)>(_a[1]))); break;
        case 48: _t->update_menu_status(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Qps::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Qps::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Qps,
      qt_meta_data_Qps, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Qps::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Qps::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Qps::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Qps))
        return static_cast<void*>(const_cast< Qps*>(this));
    return QWidget::qt_metacast(_clname);
}

int Qps::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 49)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 49;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
