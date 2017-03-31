/****************************************************************************
** Meta object code from reading C++ file 'gestusVisualization.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../inc/gestusVisualization.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gestusVisualization.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BufferManager[] = {

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
      15,   14,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BufferManager[] = {
    "BufferManager\0\0fetchData()\0"
};

void BufferManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BufferManager *_t = static_cast<BufferManager *>(_o);
        switch (_id) {
        case 0: _t->fetchData(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData BufferManager::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BufferManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BufferManager,
      qt_meta_data_BufferManager, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BufferManager::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BufferManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BufferManager::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BufferManager))
        return static_cast<void*>(const_cast< BufferManager*>(this));
    return QObject::qt_metacast(_clname);
}

int BufferManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
static const uint qt_meta_data_GestusVisualization[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      29,   21,   20,   20, 0x08,
      65,   21,   20,   20, 0x08,
      99,   21,   20,   20, 0x08,
     134,   21,   20,   20, 0x08,
     170,   21,   20,   20, 0x08,
     203,   20,   20,   20, 0x08,
     227,   21,   20,   20, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_GestusVisualization[] = {
    "GestusVisualization\0\0checked\0"
    "on_trajectoryCheckBox_toggled(bool)\0"
    "on_leftHandCheckBox_toggled(bool)\0"
    "on_rightHandCheckBox_toggled(bool)\0"
    "on_hackerModeCheckBox_toggled(bool)\0"
    "on_loggingCheckBox_toggled(bool)\0"
    "on_randomData_clicked()\0"
    "on_pausePlotCheckBox_toggled(bool)\0"
};

void GestusVisualization::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        GestusVisualization *_t = static_cast<GestusVisualization *>(_o);
        switch (_id) {
        case 0: _t->on_trajectoryCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->on_leftHandCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->on_rightHandCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->on_hackerModeCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->on_loggingCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->on_randomData_clicked(); break;
        case 6: _t->on_pausePlotCheckBox_toggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData GestusVisualization::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject GestusVisualization::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_GestusVisualization,
      qt_meta_data_GestusVisualization, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &GestusVisualization::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *GestusVisualization::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *GestusVisualization::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GestusVisualization))
        return static_cast<void*>(const_cast< GestusVisualization*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int GestusVisualization::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
