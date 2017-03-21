#include "gestusVisualization.h"
#include "ui_gestusVisualisation.h"


GestusVisualization::GestusVisualization(QWidget *parent) :
    QMainWindow(parent),
	ui(new Ui::GestusVisualization)
{
    ui->setupUi(this);
    initUiProps();

    acc.setGLWidget(ui->GLwidget);
    gyro.setGLWidget(ui->GLwidget);
    mag.setGLWidget(ui->GLwidget);

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
    ui->settings->setStyleSheet("background-color: #008081;");
    ui->devicesTree->setStyleSheet("background-color: #008081;");
 // QColor backgroundColor = ui->settings->palette().light().color();
 // backgroundColor.setAlpha(100);
 // QPainter customPainter(this);
 // customPainter.fillRect(rect(),backgroundColor);

    // disable push random data button
    ui->randomData->setVisible(false);


    return true;
}

bool GestusVisualization::setupPlotters(std::deque<std::string> *acc_src, std::deque<std::string> *gyro_src, std::deque<std::string> *mag_src)
{
    // setup sources
    acc.setupSource(acc_src);
    mag.setupSource(mag_src);
    gyro.setupSource(gyro_src);

    // setup buffers for separate plotters
    plotter_acc->setupPlot(acc.firstBuffer);
    plotter_gyro->setupPlot(gyro.firstBuffer);
    plotter_mag->setupPlot(mag.firstBuffer);

    plotter_all_acc->setupPlot(acc.secondBuffer);
    plotter_all_gyro->setupPlot(gyro.secondBuffer);
    plotter_all_mag->setupPlot(mag.secondBuffer);

    // run drawing
    plotter_acc->drawPlotFromBuffer();
    plotter_gyro->drawPlotFromBuffer();
    plotter_mag->drawPlotFromBuffer();

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

    acc.isLoggingEnabled = checked;
}

void GestusVisualization::on_randomData_clicked()
{
    int max = 256, min = -256;
    std::string s;
    s.append(std::to_string(qrand() % ((max + 1) - min) + min));
    s.append(" ");
    s.append(std::to_string(0));//qrand() % ((max + 1) - min) + min));
    s.append(" ");
    s.append(std::to_string(0));//qrand() % ((max + 1) - min) + min));
    acc.sourceBuffer->push_back(s);
}

//
// BufferManager
//


BufferManager::BufferManager()
{
  ifs.open("/dev/rfcomm0", std::ifstream::in); 
  firstBuffer = new std::deque<std::string>();
  secondBuffer = new std::deque<std::string>();


  plog::init(plog::debug, "log.csv");
}

BufferManager::~BufferManager()
{
    ifs.close();
  delete fetchTimer;
  delete firstBuffer;
  delete secondBuffer;
}

bool BufferManager::setupSource(std::deque<std::string>* buf)
{
    sourceBuffer = buf;

    // start timer for fetching data from source to separate copies
    fetchTimer = new QTimer();
    QObject::connect(fetchTimer, SIGNAL(timeout()), this, SLOT(fetchData()));
    fetchTimer->start(120);

    return true;
}

void BufferManager::fetchData()
{

    std::string str;
    if(!ifs.eof())
    {
        std::getline(ifs, str);
    }

    std::cout << "True: " << str << std::endl;
    if(!str.empty())
    {
        sourceBuffer->push_back(str);
    }


    if(sourceBuffer != nullptr && !sourceBuffer->empty())
    {

        double grad2rad = 3.141592/180.0;

        double d[3] = {0, 0, 0};
    while(sourceBuffer->front().empty())
    {
        sourceBuffer->pop_front();
    }
      splitSensorData(sourceBuffer->front(), d);
    
        // move hands
        widget->leftArm.bendHand(d[0]*grad2rad, d[1]*grad2rad, d[2]*grad2rad);
        widget->rightArm.bendHand(d[0]*grad2rad, -d[1]*grad2rad, d[2]*grad2rad);
        // move front data to different buffers

        firstBuffer->push_back(sourceBuffer->front());
        secondBuffer->push_back(sourceBuffer->front());
        sourceBuffer->pop_front();
        if(isLoggingEnabled) // TODO: move it to proper place
        {
            std::cout << firstBuffer->back() << std::endl;
            int i = 0;

                // LOG(plog::info) << firstBuffer->back() << i;
                i++;
        }
    }
}

void GestusVisualization::on_pausePlotCheckBox_toggled(bool checked)
{
    plotter_acc->pause = checked;
    plotter_mag->pause = checked;
    plotter_gyro->pause = checked;

    plotter_all_acc->pause = checked;
    plotter_all_mag->pause = checked;
    plotter_all_gyro->pause = checked;
}
