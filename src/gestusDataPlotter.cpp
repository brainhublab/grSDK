#include "gestusDataPlotter.h"
#include <iostream>
#include <stdlib.h>


DataPlotter::DataPlotter( QCustomPlot *pl)
{
    plot = pl;
}


bool DataPlotter::drawPlotFromBuffer()
{

    QTimer *dataTimer = new QTimer();
    QObject::connect(dataTimer, SIGNAL(timeout()), this, SLOT(fetchData()));
	dataTimer->start(120);
}

bool DataPlotter::setupPlot(std::deque<std::string> *buf)
{
    buffer = buf;

    plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                    QCP::iSelectLegend | QCP::iSelectPlottables);
    plot->setInteraction(QCP::iRangeDrag, true);  // Включаем взаимодействие перетаскивания графика
    plot->axisRect()->setRangeDrag(Qt::Horizontal);   // Включаем перетаскивание только по горизонтальной оси
    plot->axisRect()->setRangeDrag(Qt::Vertical);

    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 9));

    plot->addGraph();  // X axis
    plot->graph(0)->setPen(QPen(QColor(255, 0, 0)));

    plot->addGraph();  // Y axis
    plot->graph(1)->setPen(QPen(QColor(0, 255, 0)));

    plot->addGraph();  // Z axis
    plot->graph(2)->setPen(QPen(QColor(0, 0, 255)));

    plot->axisRect()->setupFullAxesBox();

}
void DataPlotter::fetchData()
{
    if(buffer != nullptr && !buffer->empty())
    {

        double arr[3];

        splitSensorData(buffer->front(), arr);

        buffer->pop_front();


        plot->graph(0)->addData(plot->graph(0)->dataCount(), arr[0]);
        plot->graph(0)->rescaleValueAxis();
        plot->graph(1)->addData(plot->graph(1)->dataCount(), arr[1]);
		plot->graph(1)->rescaleValueAxis(true);
        plot->graph(2)->addData(plot->graph(2)->dataCount(), arr[2]);
		plot->graph(2)->rescaleValueAxis(true);
        plot->xAxis->setRange(plot->graph(0)->dataCount() - 0.1, 100, Qt::AlignRight);
        plot->replot();
    }
}
