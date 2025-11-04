/****************************************************************************
** Meta object code from reading C++ file 'NetWorkReactor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../FinalClient/NetWorkReactor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetWorkReactor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetWorkReactor_t {
    QByteArrayData data[17];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetWorkReactor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetWorkReactor_t qt_meta_stringdata_NetWorkReactor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "NetWorkReactor"
QT_MOC_LITERAL(1, 15, 11), // "decodeFrame"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 8), // "cameraId"
QT_MOC_LITERAL(4, 37, 10), // "VideoFrame"
QT_MOC_LITERAL(5, 48, 5), // "frame"
QT_MOC_LITERAL(6, 54, 17), // "handlerRegistered"
QT_MOC_LITERAL(7, 72, 8), // "TaskType"
QT_MOC_LITERAL(8, 81, 4), // "type"
QT_MOC_LITERAL(9, 86, 16), // "IMessageHandler*"
QT_MOC_LITERAL(10, 103, 7), // "handler"
QT_MOC_LITERAL(11, 111, 7), // "Refresh"
QT_MOC_LITERAL(12, 119, 11), // "onReadyRead"
QT_MOC_LITERAL(13, 131, 14), // "onDisconnected"
QT_MOC_LITERAL(14, 146, 7), // "onError"
QT_MOC_LITERAL(15, 154, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(16, 183, 3) // "err"

    },
    "NetWorkReactor\0decodeFrame\0\0cameraId\0"
    "VideoFrame\0frame\0handlerRegistered\0"
    "TaskType\0type\0IMessageHandler*\0handler\0"
    "Refresh\0onReadyRead\0onDisconnected\0"
    "onError\0QAbstractSocket::SocketError\0"
    "err"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetWorkReactor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,
       6,    3,   49,    2, 0x06 /* Public */,
      11,    1,   56,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   59,    2, 0x08 /* Private */,
      13,    0,   60,    2, 0x08 /* Private */,
      14,    1,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 4,    3,    5,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 7, 0x80000000 | 9,    3,    8,   10,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   16,

       0        // eod
};

void NetWorkReactor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetWorkReactor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->decodeFrame((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const VideoFrame(*)>(_a[2]))); break;
        case 1: _t->handlerRegistered((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< TaskType(*)>(_a[2])),(*reinterpret_cast< IMessageHandler*(*)>(_a[3]))); break;
        case 2: _t->Refresh((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->onReadyRead(); break;
        case 4: _t->onDisconnected(); break;
        case 5: _t->onError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetWorkReactor::*)(int , const VideoFrame & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetWorkReactor::decodeFrame)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetWorkReactor::*)(int , TaskType , IMessageHandler * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetWorkReactor::handlerRegistered)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetWorkReactor::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetWorkReactor::Refresh)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetWorkReactor::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NetWorkReactor.data,
    qt_meta_data_NetWorkReactor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetWorkReactor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetWorkReactor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetWorkReactor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetWorkReactor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void NetWorkReactor::decodeFrame(int _t1, const VideoFrame & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetWorkReactor::handlerRegistered(int _t1, TaskType _t2, IMessageHandler * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetWorkReactor::Refresh(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
