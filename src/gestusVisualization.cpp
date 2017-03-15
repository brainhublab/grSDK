#include "gestusVisualization.h"
#include "ui_gestusVisualisation.h"


GestusVisualization::GestusVisualization(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::GestusVisualization)
{
    ui->setupUi(this);
    initUiProps();

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

bool GestusVisualization::initUiProps()
{
    // splitter init
    QList<int> sizes;
    sizes.push_back(300);
    sizes.push_back(100);
    ui->splitter->setSizes(sizes);
    // checkboxes style init
    ui->hackerModeCheckBox->setStyleSheet("background-color: #008081;");
    ui->leftHandCheckBox->setStyleSheet("background-color: #008081;");
    ui->rightHandCheckBox->setStyleSheet("background-color: #008081;");
    ui->trajectoryCheckBox->setStyleSheet("background-color: #008081;");
    ui->loggingCheckBox->setStyleSheet("background-color: #008081;");

    // disable push random data button
    ui->randomData->setVisible(false);
    return true;
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

//
// Qt Slots
//

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
        ui->GLwidget->getRenderer()->setPlaneColor(0, 255, 0, 0);
        // ui->GLwidget->getRenderer()->setLinesColor(255,193,7, 255);
        ui->GLwidget->getRenderer()->setLinesColor(0,255,0, 255);
    ui->hackerModeCheckBox->setStyleSheet("color: white;"
                                          "background: #e67e22;"
                                          "selection-color: #34495e;"
                                          "selection-background-color: white;");
    }
    else
    {
        // go to default
        ui->GLwidget->getRenderer()->setLinesColor(255, 0, 0, 255);
        ui->GLwidget->getRenderer()->setPlaneColor(255, 255, 255, 255);
        ui->hackerModeCheckBox->setStyleSheet("color: white;"
                                              "background: #008081;"
                                              "selection-color: white;"
                                              "selection-background-color: white;");
    }

}

void GestusVisualization::on_loggingCheckBox_toggled(bool checked)
{
    ui->randomData->setVisible(checked);

    /*
    plotter_acc->logger(checked);
    plotter_gyro->logger(checked);
    plotter_mag->logger(checked);*/

    plotter_all_acc->logger(checked);
    plotter_all_gyro->logger(checked);
    plotter_all_mag->logger(checked);
}

void GestusVisualization::on_randomData_clicked()
{
    std::string s;
    s.append(std::to_string(qrand() % ((377 + 1) - 0) + 0));
    s.append(" ");
    s.append(std::to_string(qrand() % ((377 + 1) - 0) + 0));
    s.append(" ");
    s.append(std::to_string(qrand() % ((377 + 1) - 0) + 0));
    plotter_all_acc->pushData(s);
}
