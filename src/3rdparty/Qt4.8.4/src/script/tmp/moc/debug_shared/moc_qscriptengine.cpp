/****************************************************************************
** Meta object code from reading C++ file 'qscriptengine.h'
**
** Created: Mon 26. Nov 09:54:31 2012
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../api/qscriptengine.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qscriptengine.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_QScriptEngine[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   15,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      62,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_QScriptEngine[] = {
    "QScriptEngine\0\0exception\0"
    "signalHandlerException(QScriptValue)\0"
    "_q_objectDestroyed(QObject*)\0"
};

void QScriptEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        QScriptEngine *_t = static_cast<QScriptEngine *>(_o);
        switch (_id) {
        case 0: _t->signalHandlerException((*reinterpret_cast< const QScriptValue(*)>(_a[1]))); break;
        case 1: _t->d_func()->_q_objectDestroyed((*reinterpret_cast< QObject*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData QScriptEngine::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject QScriptEngine::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QScriptEngine,
      qt_meta_data_QScriptEngine, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &QScriptEngine::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *QScriptEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *QScriptEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_QScriptEngine))
        return static_cast<void*>(const_cast< QScriptEngine*>(this));
    return QObject::qt_metacast(_clname);
}

int QScriptEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void QScriptEngine::signalHandlerException(const QScriptValue & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
