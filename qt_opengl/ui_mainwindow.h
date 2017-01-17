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
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
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
    QWidget *plots_tab;
    QVBoxLayout *verticalLayout;
    QSplitter *plots_splitter;
    QCustomPlot *PlotData_X;
    QCustomPlot *PlotData_Y;
    QCustomPlot *PlotData_Z;
    QWidget *tab_4;
    QToolBar *mainToolBar;
    QMenuBar *menuBar;
    QMenu *menuOne;

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
        GLwidget->setMaximumSize(QSize(16777215, 199));
        splitter->addWidget(GLwidget);
        Plots = new QTabWidget(splitter);
        Plots->setObjectName(QStringLiteral("Plots"));
        plots_tab = new QWidget();
        plots_tab->setObjectName(QStringLiteral("plots_tab"));
        verticalLayout = new QVBoxLayout(plots_tab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        plots_splitter = new QSplitter(plots_tab);
        plots_splitter->setObjectName(QStringLiteral("plots_splitter"));
        plots_splitter->setOrientation(Qt::Horizontal);
        PlotData_X = new QCustomPlot(plots_splitter);
        PlotData_X->setObjectName(QStringLiteral("PlotData_X"));
        PlotData_X->setMinimumSize(QSize(0, 0));
        PlotData_X->setMaximumSize(QSize(16777215, 16777215));
        PlotData_X->setBaseSize(QSize(300, 0));
        plots_splitter->addWidget(PlotData_X);
        PlotData_Y = new QCustomPlot(plots_splitter);
        PlotData_Y->setObjectName(QStringLiteral("PlotData_Y"));
        PlotData_Y->setMinimumSize(QSize(0, 0));
        PlotData_Y->setMaximumSize(QSize(16777215, 16777215));
        PlotData_Y->setBaseSize(QSize(300, 0));
        plots_splitter->addWidget(PlotData_Y);
        PlotData_Z = new QCustomPlot(plots_splitter);
        PlotData_Z->setObjectName(QStringLiteral("PlotData_Z"));
        PlotData_Z->setMinimumSize(QSize(0, 0));
        PlotData_Z->setMaximumSize(QSize(16777215, 16777215));
        PlotData_Z->setBaseSize(QSize(300, 0));
        plots_splitter->addWidget(PlotData_Z);

        verticalLayout->addWidget(plots_splitter);

        Plots->addTab(plots_tab, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        Plots->addTab(tab_4, QString());
        splitter->addWidget(Plots);

        horizontalLayout->addWidget(splitter);

        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 30));
        menuOne = new QMenu(menuBar);
        menuOne->setObjectName(QStringLiteral("menuOne"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuOne->menuAction());
        menuOne->addAction(actionWhat);
        menuOne->addAction(actionIs);

        retranslateUi(MainWindow);

        Plots->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QT + OpenGL", 0));
        actionWhat->setText(QApplication::translate("MainWindow", "&what?", 0));
        actionIs->setText(QApplication::translate("MainWindow", "&is", 0));
        Plots->setTabText(Plots->indexOf(plots_tab), QApplication::translate("MainWindow", "Tab 1", 0));
        Plots->setTabText(Plots->indexOf(tab_4), QApplication::translate("MainWindow", "Tab 2", 0));
        menuOne->setTitle(QApplication::translate("MainWindow", "O&ne", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
