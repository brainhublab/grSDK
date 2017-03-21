#ifndef GESTUS_VISUALISATION_H
#define GESTUS_VISUALISATION_H


#include <QMainWindow>
#include <QKeyEvent>
#include <QObject>

#include <thread>
#include <deque>

#include "gestusDataPlotter.h"
#include "gestusGLWidget.h"
#include <iostream>
#include "externAssets/plog/Log.h" //Lib for logging in csv format

namespace Ui{
class GestusVisualization;
}
// TODO:: DataPlotter -> DataManager(in fetchData() to bend hand)

    struct BufferManager : public QObject
    {
      Q_OBJECT
    public:
        BufferManager();
        ~BufferManager();
        bool setGLWidget(GestusGLWidget* w)
        {
            widget = w;
            return true;
        };

        void splitSensorData(std::string str, double arr[3])
        {
                int i = 0;
                double n;
                std::stringstream ss(str);
                while(ss >> n)
                {
        			arr[i] = n;
                        i++;
                }
        }
        //
        bool setupSource(std::deque<std::string>* buf);
        QTimer* fetchTimer;

        std::deque<std::string>* sourceBuffer = nullptr;

        std::deque<std::string>* firstBuffer;
        std::deque<std::string>* secondBuffer;


            bool isLoggingEnabled = false;
    public slots:
        void fetchData();
    private:
          GestusGLWidget* widget;
    };

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
    void on_loggingCheckBox_toggled(bool checked);
    void on_randomData_clicked();
    void on_pausePlotCheckBox_toggled(bool checked);

private:
    bool initUiProps();
  	Ui::GestusVisualization *ui;

    BufferManager acc;
    BufferManager gyro;
    BufferManager mag;

    DataPlotter* plotter_acc;
    DataPlotter* plotter_gyro;
    DataPlotter* plotter_mag;
    DataPlotter* plotter_all_acc;
    DataPlotter* plotter_all_gyro;
    DataPlotter* plotter_all_mag;
};

#endif // MAINWINDOW_H
