#-------------------------------------------------
#
# Project created by QtCreator 2015-11-13T13:00:43
#
#-------------------------------------------------

QT       += core gui opengl printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gestusVisualisation
TEMPLATE = app

DESTDIR = ../bin
SOURCES += ../src/main.cpp\
        ../src/gestusVisualisation.cpp \
    ../src/gestusGLWidget.cpp \
    ../src/gestusHand.cpp \
    ../src/gestusRenderer.cpp \
    ../externAssets/qcustomplot/qcustomplot.cpp \
    ../src/gestusDataplotter.cpp

HEADERS  += ./inc/gestusVisualisation.h \
    ../inc/gestusGLWidget.h \
    ../inc/node.h \
    ../inc/gestusHand.h \
    ../inc/dimensions.h \
    ../inc/gestusRenderer.h \
    ../inc/qcustomplot/qcustomplot.h \
    ../inc/gestusDataplotter.h

FORMS    += gestusVisualisation.ui

LIBS += -lglut -lGLU
