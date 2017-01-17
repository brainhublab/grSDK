#-------------------------------------------------
#
# Project created by QtCreator 2015-11-13T13:00:43
#
#-------------------------------------------------

QT       += core gui opengl printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT_OpenGL
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    arm.cpp \
    renderer.cpp \
    qcustomplot/qcustomplot.cpp \
    dataplotter.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    node.h \
    arm.h \
    dimensions.h \
    renderer.h \
    qcustomplot/qcustomplot.h \
    dataplotter.h

FORMS    += mainwindow.ui

LIBS += -lglut -lGLU
