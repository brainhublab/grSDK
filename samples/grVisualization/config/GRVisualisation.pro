#-------------------------------------------------
#
# Project created by QtCreator 2015-11-13T13:00:43
#
#-------------------------------------------------
CONFIG += qt
QT += widgets core gui opengl printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GRVisualization
TEMPLATE = app
LIBS += -L/usr/lib64 -ldbus-1 -lglut -lGLU
INCLUDEPATH +=  /usr/include/dbus-1.0 /usr/lib64/dbus-1.0/include ./inc ../../externAssets/qcustomplot ../../externAssets/ ../../inc ./
QMAKE_CXXFLAGS += -g -std=c++11

OBJECTS_DIR = ./build
DESTDIR = ./bin
MOC_DIR=./build/generated_files
SOURCES += ./src/main.cpp\
    ./src/grVisualization.cpp \
    ./src/grBufferManager.cpp \
    ./src/GRGLWidget.cpp \
    ./src/grHand.cpp \
    ./src/grRender.cpp \
    ../../externAssets/qcustomplot/qcustomplot.cpp \
    ./src/grDataPlotter.cpp \
    ../../src/grConnection.cpp \
    ../../src/grAlgorithm.cpp \
	../../src/grGrt.cpp \
	../../src/grMadgwick.cpp \
        ../../src/grTrajectory.cpp \
	../../src/grUtilities.cpp \


HEADERS  += ./inc/grVisualization.h \
    ./inc/ui_grVisualization.h \
    ./inc/grBufferManager.h \
    ./inc/GRGLWidget.h \
    ./inc/grHandNode.h \
    ./inc/grHand.h \
    ./inc/renderConfig.h \
    ./inc/grRender.h \
    ../../externAssets/qcustomplot/qcustomplot.h \
    ./inc/grDataPlotter.h \
    ../../inc/grConnection.h \
    ../../inc/grAlgorithm.h \
	../../inc/grDataAttributes.h \
	../../inc/grDevice.h \
	../../inc/grGrt.h \
	../../inc/grMadgwick.h \
        ../../inc/grTrajectory.h \
	../../inc/grUtilities.h \

FORMS    += grVisualisation.ui

#LIBS
