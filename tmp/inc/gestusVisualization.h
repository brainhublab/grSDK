#ifndef GESTUS_VISUALISATION_H
#define GESTUS_VISUALISATION_H


#include <QMainWindow>
#include <QKeyEvent>

#include "gestusDataPlotter.h"
#include <QObject>

#include <thread>
#include <deque>

namespace Ui {
class GestusVisualisation;
}

class GestusVisualisation : public QMainWindow
{
    Q_OBJECT

public:
    explicit GestusVisualisation(QWidget *parent = 0);
    ~GestusVisualisation();
    bool setupPlotters(std::deque<std::string> *, std::deque<std::string> *, std::deque<std::string> *);
    //void keyPressEvent(QKeyEvent *event);

private:
    Ui::GestusVisualisation *ui;
    DataPlotter* plotter_acc;
    DataPlotter* plotter_gyro;
    DataPlotter* plotter_mag;
    DataPlotter* plotter_all_acc;
    DataPlotter* plotter_all_gyro;
    DataPlotter* plotter_all_mag;

    std::deque<std::string> buffer;
};

#endif // MAINWINDOW_H
