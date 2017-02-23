#-------------------------------------------------
#
# Project created by QtCreator 2015-11-13T13:00:43
#
#-------------------------------------------------
CONFIG += qt
QT += widgets core gui opengl printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gestusVisualization
TEMPLATE = app
LIBS += -L/usr/lib64 -ldbus-1 -lglut -lGLU
INCLUDEPATH +=  /usr/include/dbus-1.0 /usr/lib64/dbus-1.0/include ./inc ./externAssets/qcustomplot
QMAKE_CXXFLAGS += -g -std=c++11

OBJECTS_DIR = ./build
DESTDIR = ./bin
MOC_DIR=generated_files
SOURCES += ./src/main.cpp\
        ./src/gestusVisualization.cpp \
    ./src/gestusGLWidget.cpp \
    ./src/gestusHand.cpp \
    ./src/gestusRender.cpp \
    ./externAssets/qcustomplot/qcustomplot.cpp \
    ./src/gestusDataPlotter.cpp \
    ./src/gestusConnection.cpp

HEADERS  += ./inc/gestusVisualization.h \
    ./inc/ui_gestusVisualization.h \
    ./inc/gestusGLWidget.h \
    ./inc/gestusHandNode.h \
    ./inc/gestusHand.h \
    ./inc/dimensions.h \
    ./inc/gestusRender.h \
    ./externAssets/qcustomplot/qcustomplot.h \
    ./inc/gestusDataPlotter.h \
    ./inc/gestusConnection.h

FORMS    += gestusVisualisation.ui

#LIBS
