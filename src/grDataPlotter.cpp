#include <iostream>
#include <stdlib.h>

#include "grDataPlotter.h"

#include <math.h>

// Helpers

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


GRDataPlotter::GRDataPlotter( QCustomPlot *pl)
{
    plot = pl;
}

GRDataPlotter::~GRDataPlotter() {
    delete dataTimer;
}

bool GRDataPlotter::drawPlotFromBuffer()
{
    dataTimer = new QTimer();
    QObject::connect(dataTimer, SIGNAL(timeout()), this, SLOT(fetchData()));
    dataTimer->start(50);

    return true;
}

bool GRDataPlotter::setupPlot(std::deque<std::string> *buf)
{
    buffer = buf;

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    plot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    plot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    plot->axisRect()->setRangeDrag(Qt::Vertical);

    plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignLeft|Qt::AlignTop); // legend position

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
void GRDataPlotter::fetchData()
{
    double arr[3] = {0, 0, 0}; // grad
    if(buffer != nullptr && !buffer->empty())
    {
        splitSensorData(buffer->front(), arr);

        buffer->pop_front();
    }


    if(pause)
    {
        return;
    }

    plot->graph(0)->addData(plot->graph(0)->dataCount(), arr[0]);
    plot->graph(0)->rescaleValueAxis();
    plot->graph(1)->addData(plot->graph(1)->dataCount(), arr[1]);
    plot->graph(1)->rescaleValueAxis(true);
    plot->graph(2)->addData(plot->graph(2)->dataCount(), arr[2]);
    plot->graph(2)->rescaleValueAxis(true);
    plot->xAxis->setRange(plot->graph(0)->dataCount() - 0.1, 100, Qt::AlignRight);
    plot->replot();
}