/********************************************************************************
** Form generated from reading UI file 'grVisualisation.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRVISUALISATION_H
#define UI_GRVISUALISATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "GRGLWidget.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE

class Ui_GRVisualization
{
public:
    QAction *actionWhat;
    QAction *actionIs;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QSplitter *splitter;
    GRGLWidget *GLwidget;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QTreeWidget *devicesTree;
    QWidget *settings;
    QCheckBox *trajectoryCheckBox;
    QPushButton *randomData;
    QCheckBox *rightHandCheckBox;
    QCheckBox *loggingCheckBox;
    QCheckBox *hackerModeCheckBox;
    QCheckBox *leftHandCheckBox;
    QCheckBox *pausePlotCheckBox;
    QCheckBox *rotationsCheckBox;
    QTabWidget *Plots;
    QWidget *tab_all_data;
    QHBoxLayout *horizontalLayout_2;
    QCustomPlot *accelerometer;
    QCustomPlot *gyroscope;
    QCustomPlot *magnetometer;
    QWidget *tab_accelerometer;
    QVBoxLayout *verticalLayout_5;
    QCustomPlot *only_accelerometer;
    QWidget *tab_gyroscope;
    QVBoxLayout *verticalLayout_4;
    QCustomPlot *only_gyroscope;
    QWidget *tab_magnetometer;
    QVBoxLayout *verticalLayout;
    QCustomPlot *only_magnetometer;

    void setupUi(QMainWindow *GRVisualization)
    {
        if (GRVisualization->objectName().isEmpty())
            GRVisualization->setObjectName(QStringLiteral("GRVisualization"));
        GRVisualization->resize(800, 600);
        GRVisualization->setMinimumSize(QSize(800, 600));
        actionWhat = new QAction(GRVisualization);
        actionWhat->setObjectName(QStringLiteral("actionWhat"));
        actionIs = new QAction(GRVisualization);
        actionIs->setObjectName(QStringLiteral("actionIs"));
        centralWidget = new QWidget(GRVisualization);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        GLwidget = new GRGLWidget(splitter);
        GLwidget->setObjectName(QStringLiteral("GLwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GLwidget->sizePolicy().hasHeightForWidth());
        GLwidget->setSizePolicy(sizePolicy);
        GLwidget->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(GLwidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer = new QSpacerItem(431, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        devicesTree = new QTreeWidget(GLwidget);
        devicesTree->setObjectName(QStringLiteral("devicesTree"));
        devicesTree->setMaximumSize(QSize(200, 500));
        devicesTree->setStyleSheet(QStringLiteral("background-color: rgb(88, 88, 88);"));

        gridLayout->addWidget(devicesTree, 0, 2, 1, 1);

        settings = new QWidget(GLwidget);
        settings->setObjectName(QStringLiteral("settings"));
        settings->setMinimumSize(QSize(130, 200));
        settings->setMaximumSize(QSize(130, 250));
        trajectoryCheckBox = new QCheckBox(settings);
        trajectoryCheckBox->setObjectName(QStringLiteral("trajectoryCheckBox"));
        trajectoryCheckBox->setEnabled(true);
        trajectoryCheckBox->setGeometry(QRect(0, 10, 89, 22));
        trajectoryCheckBox->setChecked(false);
        randomData = new QPushButton(settings);
        randomData->setObjectName(QStringLiteral("randomData"));
        randomData->setEnabled(true);
        randomData->setGeometry(QRect(0, 150, 109, 30));
        randomData->setMaximumSize(QSize(240, 30));
        rightHandCheckBox = new QCheckBox(settings);
        rightHandCheckBox->setObjectName(QStringLiteral("rightHandCheckBox"));
        rightHandCheckBox->setGeometry(QRect(0, 50, 97, 22));
        QPalette palette;
        QBrush brush(QColor(59, 2, 51, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush);
        rightHandCheckBox->setPalette(palette);
        rightHandCheckBox->setAutoFillBackground(false);
        rightHandCheckBox->setChecked(true);
        loggingCheckBox = new QCheckBox(settings);
        loggingCheckBox->setObjectName(QStringLiteral("loggingCheckBox"));
        loggingCheckBox->setEnabled(true);
        loggingCheckBox->setGeometry(QRect(0, 130, 78, 22));
        loggingCheckBox->setChecked(false);
        hackerModeCheckBox = new QCheckBox(settings);
        hackerModeCheckBox->setObjectName(QStringLiteral("hackerModeCheckBox"));
        hackerModeCheckBox->setEnabled(true);
        hackerModeCheckBox->setGeometry(QRect(0, 110, 109, 22));
        leftHandCheckBox = new QCheckBox(settings);
        leftHandCheckBox->setObjectName(QStringLiteral("leftHandCheckBox"));
        leftHandCheckBox->setGeometry(QRect(0, 70, 88, 22));
        leftHandCheckBox->setCheckable(true);
        leftHandCheckBox->setChecked(true);
        pausePlotCheckBox = new QCheckBox(settings);
        pausePlotCheckBox->setObjectName(QStringLiteral("pausePlotCheckBox"));
        pausePlotCheckBox->setGeometry(QRect(0, 90, 89, 22));
        rotationsCheckBox = new QCheckBox(settings);
        rotationsCheckBox->setObjectName(QStringLiteral("rotationsCheckBox"));
        rotationsCheckBox->setGeometry(QRect(0, 30, 89, 22));
        rotationsCheckBox->setChecked(true);

        gridLayout->addWidget(settings, 0, 0, 1, 1);

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
        only_accelerometer = new QCustomPlot(tab_accelerometer);
        only_accelerometer->setObjectName(QStringLiteral("only_accelerometer"));
        only_accelerometer->setMinimumSize(QSize(0, 0));
        only_accelerometer->setMaximumSize(QSize(16777215, 16777215));
        only_accelerometer->setBaseSize(QSize(300, 0));

        verticalLayout_5->addWidget(only_accelerometer);

        Plots->addTab(tab_accelerometer, QString());
        tab_gyroscope = new QWidget();
        tab_gyroscope->setObjectName(QStringLiteral("tab_gyroscope"));
        verticalLayout_4 = new QVBoxLayout(tab_gyroscope);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        only_gyroscope = new QCustomPlot(tab_gyroscope);
        only_gyroscope->setObjectName(QStringLiteral("only_gyroscope"));
        only_gyroscope->setMinimumSize(QSize(0, 0));
        only_gyroscope->setMaximumSize(QSize(16777215, 16777215));
        only_gyroscope->setBaseSize(QSize(300, 0));

        verticalLayout_4->addWidget(only_gyroscope);

        Plots->addTab(tab_gyroscope, QString());
        tab_magnetometer = new QWidget();
        tab_magnetometer->setObjectName(QStringLiteral("tab_magnetometer"));
        verticalLayout = new QVBoxLayout(tab_magnetometer);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        only_magnetometer = new QCustomPlot(tab_magnetometer);
        only_magnetometer->setObjectName(QStringLiteral("only_magnetometer"));
        only_magnetometer->setMinimumSize(QSize(0, 0));
        only_magnetometer->setMaximumSize(QSize(16777215, 16777215));
        only_magnetometer->setBaseSize(QSize(300, 0));

        verticalLayout->addWidget(only_magnetometer);

        Plots->addTab(tab_magnetometer, QString());
        splitter->addWidget(Plots);

        gridLayout_2->addWidget(splitter, 0, 0, 1, 1);

        GRVisualization->setCentralWidget(centralWidget);

        retranslateUi(GRVisualization);

        Plots->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(GRVisualization);
    } // setupUi

    void retranslateUi(QMainWindow *GRVisualization)
    {
        GRVisualization->setWindowTitle(QApplication::translate("GRVisualization", "GR SDK", 0));
        actionWhat->setText(QApplication::translate("GRVisualization", "&what?", 0));
        actionIs->setText(QApplication::translate("GRVisualization", "&is", 0));
        QTreeWidgetItem *___qtreewidgetitem = devicesTree->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("GRVisualization", "Connected GR Devices", 0));
        trajectoryCheckBox->setText(QApplication::translate("GRVisualization", "Trajectory", 0));
        randomData->setText(QApplication::translate("GRVisualization", "RandomData", 0));
        rightHandCheckBox->setText(QApplication::translate("GRVisualization", "Right Hand", 0));
        loggingCheckBox->setText(QApplication::translate("GRVisualization", "More", 0));
        hackerModeCheckBox->setText(QApplication::translate("GRVisualization", "Hacker Mode", 0));
        leftHandCheckBox->setText(QApplication::translate("GRVisualization", "Left Hand", 0));
        pausePlotCheckBox->setText(QApplication::translate("GRVisualization", "Pause Plot", 0));
        rotationsCheckBox->setText(QApplication::translate("GRVisualization", "Rotations", 0));
        Plots->setTabText(Plots->indexOf(tab_all_data), QApplication::translate("GRVisualization", "All Sensors", 0));
        Plots->setTabText(Plots->indexOf(tab_accelerometer), QApplication::translate("GRVisualization", "Accelerometer", 0));
        Plots->setTabText(Plots->indexOf(tab_gyroscope), QApplication::translate("GRVisualization", "Gyroscope", 0));
        Plots->setTabText(Plots->indexOf(tab_magnetometer), QApplication::translate("GRVisualization", "Magnetometer", 0));
    } // retranslateUi

};

namespace Ui {
    class GRVisualization: public Ui_GRVisualization {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRVISUALISATION_H
