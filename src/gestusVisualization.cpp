#include "gestusVisualization.h"
#include "ui_gestusVisualisation.h"


GestusVisualization::GestusVisualization(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::GestusVisualization)
{
    ui->setupUi(this);
    ui->hackerModeCheckBox->setStyleSheet("background-color: #008081;");
    ui->leftHandCheckBox->setStyleSheet("background-color: #008081;");
    ui->rightHandCheckBox->setStyleSheet("background-color: #008081;");
    ui->trajectoryCheckBox->setStyleSheet("background-color: #008081;");

    plotter_acc = new DataPlotter(ui->only_accelerometer);
    plotter_gyro = new DataPlotter(ui->only_gyroscope);
    plotter_mag = new DataPlotter(ui->only_magnetometer);

    plotter_all_acc = new DataPlotter(ui->accelerometer);
    plotter_all_gyro = new DataPlotter(ui->gyroscope);
    plotter_all_mag = new DataPlotter(ui->magnetometer);

}

GestusVisualization::~GestusVisualization()
{
    delete plotter_acc;
    delete plotter_gyro;
    delete plotter_mag;

    delete plotter_all_acc;
    delete plotter_all_gyro;
    delete plotter_all_mag;
    delete ui;
}

bool GestusVisualization::setupPlotters(std::deque<std::string> *acc, std::deque<std::string> *gyro, std::deque<std::string> *mag)
{
    //plotter_acc->setupPlot(acc);
    //plotter_gyro->setupPlot(gyro);
   //plotter_mag->setupPlot(mag);
    plotter_all_acc->setupPlot(acc);
    plotter_all_gyro->setupPlot(gyro);
    plotter_all_mag->setupPlot(mag);

    //plotter_acc->drawPlotFromBuffer();
    //plotter_gyro->drawPlotFromBuffer();
    //plotter_mag->drawPlotFromBuffer();
    plotter_all_acc->drawPlotFromBuffer();
    plotter_all_gyro->drawPlotFromBuffer();
    plotter_all_mag->drawPlotFromBuffer();

    return true;
}

void GestusVisualization::on_trajectoryCheckBox_toggled(bool checked)
{
    ui->GLwidget->renderTrajectory(checked);
}

void GestusVisualization::on_leftHandCheckBox_toggled(bool checked)
{
    ui->GLwidget->renderLeftHand(checked);
}

void GestusVisualization::on_rightHandCheckBox_toggled(bool checked)
{
    ui->GLwidget->renderRightHand(checked);
}

void GestusVisualization::on_hackerModeCheckBox_toggled(bool checked)
{
    if(checked)
    {
    ui->hackerModeCheckBox->setStyleSheet("color: white;"
                                          "background: #e67e22;"
                                          "selection-color: #34495e;"
                                          "selection-background-color: white;");
    }
    else
    {
        ui->hackerModeCheckBox->setStyleSheet("color: white;"
                                              "background: #008081;"
                                              "selection-color: white;"
                                              "selection-background-color: white;");
    }

}
