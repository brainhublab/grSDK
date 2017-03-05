#ifndef GESTUS_VISUALISATION_H
#define GESTUS_VISUALISATION_H


#include <QMainWindow>
#include <QKeyEvent>
#include <QObject>

#include <thread>
#include <deque>

#include "gestusDataPlotter.h"

namespace Ui{
class GestusVisualization;
}

class GestusVisualization : public QMainWindow
{
    Q_OBJECT

public:
	explicit GestusVisualization(QWidget *parent = 0);
	~GestusVisualization();
    bool setupPlotters(std::deque<std::string> *, std::deque<std::string> *, std::deque<std::string> *);
    void keyPressEvent(QKeyEvent *event);

private:
	Ui::GestusVisualization *ui;
    DataPlotter* plotter_acc;
    DataPlotter* plotter_gyro;
    DataPlotter* plotter_mag;
    DataPlotter* plotter_all_acc;
    DataPlotter* plotter_all_gyro;
    DataPlotter* plotter_all_mag;

    std::deque<std::string> buffer;
};

#endif // MAINWINDOW_H
