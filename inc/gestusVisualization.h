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

private slots:
    void on_trajectoryCheckBox_toggled(bool checked);
    void on_leftHandCheckBox_toggled(bool checked);
    void on_rightHandCheckBox_toggled(bool checked);
    void on_hackerModeCheckBox_toggled(bool checked);

private:
	Ui::GestusVisualization *ui;
  std::deque<std::string> acc;
  std::deque<std::string> gyro;
  std::deque<std::string> mag;
    DataPlotter* plotter_acc;
    DataPlotter* plotter_gyro;
    DataPlotter* plotter_mag;
    DataPlotter* plotter_all_acc;
    DataPlotter* plotter_all_gyro;
    DataPlotter* plotter_all_mag;
};

#endif // MAINWINDOW_H
