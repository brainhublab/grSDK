/********************************************************************************
** Form generated from reading UI file 'gestusVisualisation.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GESTUSVISUALISATION_H
#define UI_GESTUSVISUALISATION_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QSplitter>
#include <QtGui/QTabWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "gestusGLWidget.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_GestusVisualization
{
public:
    QAction *actionWhat;
    QAction *actionIs;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    GestusGLWidget *GLwidget;
    QTabWidget *Plots;
    QWidget *tab_all_data;
    QHBoxLayout *horizontalLayout_2;
    QCustomPlot *accelerometer;
    QCustomPlot *gyroscope;
    QCustomPlot *magnetometer;
    QWidget *tab_accelerometer;
    QVBoxLayout *verticalLayout_5;
    QCustomPlot *PlotData_X;
    QWidget *tab_gyroscope;
    QVBoxLayout *verticalLayout_4;
    QCustomPlot *PlotData_Y;
    QWidget *tab_magnetometer;
    QVBoxLayout *verticalLayout;
    QCustomPlot *PlotData_Z;

    void setupUi(QMainWindow *GestusVisualization)
    {
        if (GestusVisualization->objectName().isEmpty())
            GestusVisualization->setObjectName(QString::fromUtf8("GestusVisualization"));
        GestusVisualization->resize(800, 600);
        GestusVisualization->setMinimumSize(QSize(800, 600));
        actionWhat = new QAction(GestusVisualization);
        actionWhat->setObjectName(QString::fromUtf8("actionWhat"));
        actionIs = new QAction(GestusVisualization);
        actionIs->setObjectName(QString::fromUtf8("actionIs"));
        centralWidget = new QWidget(GestusVisualization);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Vertical);
        GLwidget = new GestusGLWidget(splitter);
        GLwidget->setObjectName(QString::fromUtf8("GLwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GLwidget->sizePolicy().hasHeightForWidth());
        GLwidget->setSizePolicy(sizePolicy);
        GLwidget->setMaximumSize(QSize(16777215, 16777215));
        splitter->addWidget(GLwidget);
        Plots = new QTabWidget(splitter);
        Plots->setObjectName(QString::fromUtf8("Plots"));
        tab_all_data = new QWidget();
        tab_all_data->setObjectName(QString::fromUtf8("tab_all_data"));
        horizontalLayout_2 = new QHBoxLayout(tab_all_data);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        accelerometer = new QCustomPlot(tab_all_data);
        accelerometer->setObjectName(QString::fromUtf8("accelerometer"));
        accelerometer->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(accelerometer);

        gyroscope = new QCustomPlot(tab_all_data);
        gyroscope->setObjectName(QString::fromUtf8("gyroscope"));

        horizontalLayout_2->addWidget(gyroscope);

        magnetometer = new QCustomPlot(tab_all_data);
        magnetometer->setObjectName(QString::fromUtf8("magnetometer"));

        horizontalLayout_2->addWidget(magnetometer);

        Plots->addTab(tab_all_data, QString());
        tab_accelerometer = new QWidget();
        tab_accelerometer->setObjectName(QString::fromUtf8("tab_accelerometer"));
        verticalLayout_5 = new QVBoxLayout(tab_accelerometer);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        PlotData_X = new QCustomPlot(tab_accelerometer);
        PlotData_X->setObjectName(QString::fromUtf8("PlotData_X"));
        PlotData_X->setMinimumSize(QSize(0, 0));
        PlotData_X->setMaximumSize(QSize(16777215, 16777215));
        PlotData_X->setBaseSize(QSize(300, 0));

        verticalLayout_5->addWidget(PlotData_X);

        Plots->addTab(tab_accelerometer, QString());
        tab_gyroscope = new QWidget();
        tab_gyroscope->setObjectName(QString::fromUtf8("tab_gyroscope"));
        verticalLayout_4 = new QVBoxLayout(tab_gyroscope);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        PlotData_Y = new QCustomPlot(tab_gyroscope);
        PlotData_Y->setObjectName(QString::fromUtf8("PlotData_Y"));
        PlotData_Y->setMinimumSize(QSize(0, 0));
        PlotData_Y->setMaximumSize(QSize(16777215, 16777215));
        PlotData_Y->setBaseSize(QSize(300, 0));

        verticalLayout_4->addWidget(PlotData_Y);

        Plots->addTab(tab_gyroscope, QString());
        tab_magnetometer = new QWidget();
        tab_magnetometer->setObjectName(QString::fromUtf8("tab_magnetometer"));
        verticalLayout = new QVBoxLayout(tab_magnetometer);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        PlotData_Z = new QCustomPlot(tab_magnetometer);
        PlotData_Z->setObjectName(QString::fromUtf8("PlotData_Z"));
        PlotData_Z->setMinimumSize(QSize(0, 0));
        PlotData_Z->setMaximumSize(QSize(16777215, 16777215));
        PlotData_Z->setBaseSize(QSize(300, 0));

        verticalLayout->addWidget(PlotData_Z);

        Plots->addTab(tab_magnetometer, QString());
        splitter->addWidget(Plots);

        horizontalLayout->addWidget(splitter);

        GestusVisualization->setCentralWidget(centralWidget);

        retranslateUi(GestusVisualization);

        Plots->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(GestusVisualization);
    } // setupUi

    void retranslateUi(QMainWindow *GestusVisualization)
    {
        GestusVisualization->setWindowTitle(QApplication::translate("GestusVisualization", "QT + OpenGL", 0, QApplication::UnicodeUTF8));
        actionWhat->setText(QApplication::translate("GestusVisualization", "&what?", 0, QApplication::UnicodeUTF8));
        actionIs->setText(QApplication::translate("GestusVisualization", "&is", 0, QApplication::UnicodeUTF8));
        Plots->setTabText(Plots->indexOf(tab_all_data), QApplication::translate("GestusVisualization", "All Sensors", 0, QApplication::UnicodeUTF8));
        Plots->setTabText(Plots->indexOf(tab_accelerometer), QApplication::translate("GestusVisualization", "Accelerometer", 0, QApplication::UnicodeUTF8));
        Plots->setTabText(Plots->indexOf(tab_gyroscope), QApplication::translate("GestusVisualization", "Gyroscope", 0, QApplication::UnicodeUTF8));
        Plots->setTabText(Plots->indexOf(tab_magnetometer), QApplication::translate("GestusVisualization", "Magnetometer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GestusVisualization: public Ui_GestusVisualization {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GESTUSVISUALISATION_H
