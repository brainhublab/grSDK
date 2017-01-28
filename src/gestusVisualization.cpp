#include "./inc/gestusVisualization.h"
#include "ui_gestusVisualisation.h"


GestusVisualization::GestusVisualization(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::GestusVisualization)
{
    ui->setupUi(this);

    plotter_acc = new DataPlotter(ui->PlotData_X);
    plotter_gyro = new DataPlotter(ui->PlotData_Y);
    plotter_mag = new DataPlotter(ui->PlotData_Z);

    plotter_all_acc = new DataPlotter(ui->accelerometer);
    plotter_all_gyro = new DataPlotter(ui->gyroscope);
    plotter_all_mag = new DataPlotter(ui->magnetometer);

}

GestusVisualization::~GestusVisualization()
{
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

}
/*
void GestusVisualisation::keyPressEvent(QKeyEvent *event)
{
    // move it to GVLwidget
    if(event->key() == Qt::Key_7)
    {
        this->ui->GLwidget->getArm()->bendArm( 5.f, 0.f, 0.f );
        buffer.clear();
        plotter_x->drawPlotByAxis(&buffer, 0, "buffer 1e");
        plotter_x->showPlot();
    }
    if(event->key() == Qt::Key_8)
    {
        this->ui->GLwidget->getArm()->bendArm( 0.f, 5.f, 0.f );

        buffer.push_back("23 24 45");
    }
    if(event->key() == Qt::Key_9)
    {
        this->ui->GLwidget->getArm()->bendArm( 0.f, 0.f, 5.f );
    }

}

*/

