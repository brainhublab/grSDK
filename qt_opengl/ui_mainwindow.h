/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "glwidget.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionWhat;
    QAction *actionIs;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QSplitter *splitter;
    GLWidget *GLwidget;
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

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(800, 600);
        MainWindow->setMinimumSize(QSize(800, 600));
        actionWhat = new QAction(MainWindow);
        actionWhat->setObjectName(QStringLiteral("actionWhat"));
        actionIs = new QAction(MainWindow);
        actionIs->setObjectName(QStringLiteral("actionIs"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        GLwidget = new GLWidget(splitter);
        GLwidget->setObjectName(QStringLiteral("GLwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GLwidget->sizePolicy().hasHeightForWidth());
        GLwidget->setSizePolicy(sizePolicy);
        GLwidget->setMaximumSize(QSize(16777215, 16777215));
        splitter->addWidget(GLwidget);
        Plots = new QTabWidget(splitter);
        Plots->setObjectName(QStringLiteral("Plots"));
        tab_all_data = new QWidget();
        tab_all_data->setObjectName(QStringLiteral("tab_all_data"));
        horizontalLayout_2 = new QHBoxLayout(tab_all_data);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        accelerometer = new QCustomPlot(tab_all_data);
        accelerometer->setObjectName(QStringLiteral("accelerometer"));
        accelerometer->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(accelerometer);

        gyroscope = new QCustomPlot(tab_all_data);
        gyroscope->setObjectName(QStringLiteral("gyroscope"));

        horizontalLayout_2->addWidget(gyroscope);

        magnetometer = new QCustomPlot(tab_all_data);
        magnetometer->setObjectName(QStringLiteral("magnetometer"));

        horizontalLayout_2->addWidget(magnetometer);

        Plots->addTab(tab_all_data, QString());
        tab_accelerometer = new QWidget();
        tab_accelerometer->setObjectName(QStringLiteral("tab_accelerometer"));
        verticalLayout_5 = new QVBoxLayout(tab_accelerometer);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        PlotData_X = new QCustomPlot(tab_accelerometer);
        PlotData_X->setObjectName(QStringLiteral("PlotData_X"));
        PlotData_X->setMinimumSize(QSize(0, 0));
        PlotData_X->setMaximumSize(QSize(16777215, 16777215));
        PlotData_X->setBaseSize(QSize(300, 0));

        verticalLayout_5->addWidget(PlotData_X);

        Plots->addTab(tab_accelerometer, QString());
        tab_gyroscope = new QWidget();
        tab_gyroscope->setObjectName(QStringLiteral("tab_gyroscope"));
        verticalLayout_4 = new QVBoxLayout(tab_gyroscope);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        PlotData_Y = new QCustomPlot(tab_gyroscope);
        PlotData_Y->setObjectName(QStringLiteral("PlotData_Y"));
        PlotData_Y->setMinimumSize(QSize(0, 0));
        PlotData_Y->setMaximumSize(QSize(16777215, 16777215));
        PlotData_Y->setBaseSize(QSize(300, 0));

        verticalLayout_4->addWidget(PlotData_Y);

        Plots->addTab(tab_gyroscope, QString());
        tab_magnetometer = new QWidget();
        tab_magnetometer->setObjectName(QStringLiteral("tab_magnetometer"));
        verticalLayout = new QVBoxLayout(tab_magnetometer);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        PlotData_Z = new QCustomPlot(tab_magnetometer);
        PlotData_Z->setObjectName(QStringLiteral("PlotData_Z"));
        PlotData_Z->setMinimumSize(QSize(0, 0));
        PlotData_Z->setMaximumSize(QSize(16777215, 16777215));
        PlotData_Z->setBaseSize(QSize(300, 0));

        verticalLayout->addWidget(PlotData_Z);

        Plots->addTab(tab_magnetometer, QString());
        splitter->addWidget(Plots);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        Plots->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QT + OpenGL", 0));
        actionWhat->setText(QApplication::translate("MainWindow", "&what?", 0));
        actionIs->setText(QApplication::translate("MainWindow", "&is", 0));
        Plots->setTabText(Plots->indexOf(tab_all_data), QApplication::translate("MainWindow", "All Sensors", 0));
        Plots->setTabText(Plots->indexOf(tab_accelerometer), QApplication::translate("MainWindow", "Accelerometer", 0));
        Plots->setTabText(Plots->indexOf(tab_gyroscope), QApplication::translate("MainWindow", "Gyroscope", 0));
        Plots->setTabText(Plots->indexOf(tab_magnetometer), QApplication::translate("MainWindow", "Magnetometer", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
