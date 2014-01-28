/****************************************************************************
** Meta object code from reading C++ file 'receivergui.h'
**
** Created: Tue Jan 28 15:57:03 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "receivergui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'receivergui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ReceiverGUI[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      21,   13,   12,   12, 0x08,
      48,   13,   12,   12, 0x08,
      75,   12,   12,   12, 0x08,
     103,   97,   12,   12, 0x08,
     130,   97,   12,   12, 0x08,
     158,   13,   12,   12, 0x08,
     185,   13,   12,   12, 0x08,
     213,   97,   12,   12, 0x08,
     246,   97,   12,   12, 0x08,
     279,   12,   12,   12, 0x08,
     308,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ReceiverGUI[] = {
    "ReceiverGUI\0\0checked\0on_radio_CTA_toggled(bool)\0"
    "on_radio_ATC_toggled(bool)\0"
    "on_bt_start_clicked()\0value\0"
    "on_sl_qf_valueChanged(int)\0"
    "on_sl_thr_valueChanged(int)\0"
    "on_ck_coding_toggled(bool)\0"
    "on_ck_showRec_toggled(bool)\0"
    "on_sl_maxFeats_valueChanged(int)\0"
    "on_sl_matchThr_valueChanged(int)\0"
    "on_tx_radioDev_textChanged()\0"
    "on_tx_delay_textChanged()\0"
};

void ReceiverGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ReceiverGUI *_t = static_cast<ReceiverGUI *>(_o);
        switch (_id) {
        case 0: _t->on_radio_CTA_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_radio_ATC_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_bt_start_clicked(); break;
        case 3: _t->on_sl_qf_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->on_sl_thr_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->on_ck_coding_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->on_ck_showRec_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->on_sl_maxFeats_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_sl_matchThr_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->on_tx_radioDev_textChanged(); break;
        case 10: _t->on_tx_delay_textChanged(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ReceiverGUI::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ReceiverGUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ReceiverGUI,
      qt_meta_data_ReceiverGUI, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ReceiverGUI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ReceiverGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ReceiverGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ReceiverGUI))
        return static_cast<void*>(const_cast< ReceiverGUI*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int ReceiverGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
