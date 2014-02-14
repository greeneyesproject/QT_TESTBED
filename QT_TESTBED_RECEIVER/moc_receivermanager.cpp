/****************************************************************************
** Meta object code from reading C++ file 'receivermanager.h'
**
** Created: Fri Feb 14 13:34:29 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "receivermanager.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'receivermanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_receiverManager[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      37,   16,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
      57,   16,   16,   16, 0x0a,
      78,   16,   16,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_receiverManager[] = {
    "receiverManager\0\0sig_update_image1()\0"
    "sig_update_image2()\0slot_update_image1()\0"
    "slot_update_image2()\0"
};

void receiverManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        receiverManager *_t = static_cast<receiverManager *>(_o);
        switch (_id) {
        case 0: _t->sig_update_image1(); break;
        case 1: _t->sig_update_image2(); break;
        case 2: _t->slot_update_image1(); break;
        case 3: _t->slot_update_image2(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData receiverManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject receiverManager::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_receiverManager,
      qt_meta_data_receiverManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &receiverManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *receiverManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *receiverManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_receiverManager))
        return static_cast<void*>(const_cast< receiverManager*>(this));
    return QThread::qt_metacast(_clname);
}

int receiverManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void receiverManager::sig_update_image1()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void receiverManager::sig_update_image2()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
