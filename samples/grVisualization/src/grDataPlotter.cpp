#include <iostream>
#include <stdlib.h>

#include "grDataPlotter.h"

#include <math.h>

// Helpers

/*
 * str is string with format "double double double", arr is C-style array
 * splits a string into array
*/
void GRDataPlotter::splitSensorData(std::string str, double arr[3])
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

// Definitions

/*
 * pl is pointer to QCustomPlot object from Qt UI
*/
GRDataPlotter::GRDataPlotter( QCustomPlot *pl)
{
    plot = pl;
}

GRDataPlotter::~GRDataPlotter() {
    if(dataTimer)
    {
        delete dataTimer;
    }
}

/*
 * sets timer for calling fetchData()
*/
bool GRDataPlotter::runPlotting()
{
    dataTimer = new QTimer();
    QObject::connect(dataTimer, SIGNAL(timeout()), this, SLOT(fetchData()));
    dataTimer->start(fetchFrequency);

    return true;
}

/*
 * set buffer for plotting and config UI plot
*/
bool GRDataPlotter::setupPlot(std::deque<std::vector<float>> *buf)
{
    buffer = buf;

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    // enable horizontal and vertical drag
    plot->setInteraction(QCP::iRangeDrag, true);
    plot->axisRect()->setRangeDrag(Qt::Horizontal);
    plot->axisRect()->setRangeDrag(Qt::Vertical);
    // legend position
    plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop);

    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 9));

    plot->addGraph();  // X axis
    plot->graph(0)->setPen(QPen(QColor(255, 0, 0)));
    plot->graph(0)->setName("X axis");

    plot->addGraph();  // Y axis
    plot->graph(1)->setPen(QPen(QColor(0, 255, 0)));
    plot->graph(1)->setName("Y axis");

    plot->addGraph();  // Z axis
    plot->graph(2)->setPen(QPen(QColor(0, 0, 255)));
    plot->graph(2)->setName("Z axis");

    plot->axisRect()->setupFullAxesBox();

    return plot != nullptr;
}

/*
 * gets data from buffer and plots it
*/
void GRDataPlotter::fetchData()
{
    double arr[3] = {0, 0, 0}; // grad
    if(buffer != nullptr && !buffer->empty())
    {
        //splitSensorData(buffer->front(), arr);
        arr[0] = buffer->front()[0];
        arr[1] = buffer->front()[1];
        arr[2] = buffer->front()[2];
        buffer->pop_front();
    }


    if(pause)
    {
        return;
    }

    // plot data
    plot->graph(0)->addData(plot->graph(0)->dataCount(), arr[0]);
    plot->graph(0)->rescaleValueAxis();
    plot->graph(1)->addData(plot->graph(1)->dataCount(), arr[1]);
    plot->graph(1)->rescaleValueAxis(true);
    plot->graph(2)->addData(plot->graph(2)->dataCount(), arr[2]);
    plot->graph(2)->rescaleValueAxis(true);
    plot->xAxis->setRange(plot->graph(0)->dataCount() - 0.1, 100, Qt::AlignRight);
    plot->replot();
}
