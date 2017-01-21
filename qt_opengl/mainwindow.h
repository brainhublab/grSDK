#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QKeyEvent>

#include "dataplotter.h"
#include <QObject>

#include <thread>
#include <deque>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool setupPlotters(std::deque<std::string> *, std::deque<std::string> *, std::deque<std::string> *);
    //void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    DataPlotter* plotter_acc;
    DataPlotter* plotter_gyro;
    DataPlotter* plotter_mag;
    DataPlotter* plotter_all_acc;
    DataPlotter* plotter_all_gyro;
    DataPlotter* plotter_all_mag;

    std::deque<std::string> buffer;
};

#endif // MAINWINDOW_H
