/****************************************************************************
** Meta object code from reading C++ file 'qdeclarativeenginedebugservice_p.h'
**
** Created: Mon 26. Nov 09:58:41 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../debugger/qdeclarativeenginedebugservice_p.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qdeclarativeenginedebugservice_p.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QDeclarativeEngineDebugService[] = {

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
      59,   32,   31,   31, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QDeclarativeEngineDebugService[] = {
    "QDeclarativeEngineDebugService\0\0"
    "id,objectId,property,value\0"
    "propertyChanged(int,int,QMetaProperty,QVariant)\0"
};

void QDeclarativeEngineDebugService::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QDeclarativeEngineDebugService *_t = static_cast<QDeclarativeEngineDebugService *>(_o);
        switch (_id) {
        case 0: _t->propertyChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QMetaProperty(*)>(_a[3])),(*reinterpret_cast< const QVariant(*)>(_a[4]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QDeclarativeEngineDebugService::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QDeclarativeEngineDebugService::staticMetaObject = {
    { &QDeclarativeDebugService::staticMetaObject, qt_meta_stringdata_QDeclarativeEngineDebugService,
      qt_meta_data_QDeclarativeEngineDebugService, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QDeclarativeEngineDebugService::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QDeclarativeEngineDebugService::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QDeclarativeEngineDebugService::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QDeclarativeEngineDebugService))
        return static_cast<void*>(const_cast< QDeclarativeEngineDebugService*>(this));
    return QDeclarativeDebugService::qt_metacast(_clname);
}

int QDeclarativeEngineDebugService::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDeclarativeDebugService::qt_metacall(_c, _id, _a);
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
