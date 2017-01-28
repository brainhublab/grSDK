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
    dataTimer->start(110);
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
        plot->graph(1)->rescaleValueAxis();
        plot->graph(2)->addData(plot->graph(2)->dataCount(), arr[2]);
        plot->graph(0)->rescaleValueAxis();
        plot->xAxis->setRange(plot->graph(0)->dataCount() - 0.1, 100, Qt::AlignRight);
        plot->replot();
    }
}
/*
bool DataPlotter::drawPlotByAxis(std::deque<std::string>* buffer, size_t axis, QString label)
{
    QVector<double> axisValues;
    if(axis == 0)
        splitDataInVectors(buffer, &axisValues, nullptr, nullptr);
    else if(axis == 1)
        splitDataInVectors(buffer, nullptr, &axisValues, nullptr);
    else if(axis == 2)
        splitDataInVectors(buffer, nullptr, nullptr, &axisValues);

    plotGyroGraph(&axisValues, axis, label);

    return true;
}

bool DataPlotter::showPlot(){
        plot->yAxis->scaleRange(1.1, plot->yAxis->range().center());
        plot->xAxis->scaleRange(1.1, plot->xAxis->range().center());
        // set blank axis lines:

        plot->xAxis->setTicks(false);
        plot->yAxis->setTicks(true);
        plot->xAxis->setTickLabels(false);
        plot->yAxis->setTickLabels(true);
        // make top right axes clones of bottom left axes:
        plot->axisRect()->setupFullAxesBox();
}

void DataPlotter::drawPlot(std::deque<std::string>* buffer)
{
    plot->legend->setVisible(true);
    plot->legend->setFont(QFont("Helvetica", 9));

    QVector<double> xs, ys, zs;

    splitDataInVectors(buffer, &xs, &ys, &zs);

    int axis = 0;
    plotGyroGraph(&xs, axis, axis == 0 ? "X" : ( axis == 1 ? "Y" : "Z"));
    plotGyroGraph(&ys, ++axis, axis == 0 ? "X" : ( axis == 1 ? "Y" : "Z"));
    plotGyroGraph(&zs, ++axis, axis == 0 ? "X" : ( axis == 1 ? "Y" : "Z"));


    plot->yAxis->scaleRange(1.1, plot->yAxis->range().center());
    plot->xAxis->scaleRange(1.1, plot->xAxis->range().center());
    // set blank axis lines:
    plot->xAxis->setTicks(false);
    plot->yAxis->setTicks(true);
    plot->xAxis->setTickLabels(false);
    plot->yAxis->setTickLabels(true);
    // make top right axes clones of bottom left axes:
    plot->axisRect()->setupFullAxesBox();
}


bool DataPlotter::splitDataInVectors(const std::deque<std::string>* d, QVector<double>* xs,QVector<double>* ys, QVector<double>* zs){
    for(auto &s : *d)
    {
        double data[3];

        splitSensorData(s, data);

        if(xs != nullptr)
            xs->push_back(data[0]);
        if(ys != nullptr)
            ys->push_back(data[1]);
        if(zs != nullptr)
            zs->push_back(data[2]);
    }

    return true;
}

bool DataPlotter::plotGyroGraph(QVector<double>* graphData, size_t axis, QString label)
{
    QPen pen;
    if(axis > 2)
        return false;
    plot->addGraph();
    pen.setColor(QColor(qSin(lines*1+1.2)*80+80, qSin(lines*0.3+0)*80+80, qSin(lines*0.3+1.5)*80+80));
    plot->graph()->setPen(pen);
    plot->graph()->setName(label);
    plot->graph()->setLineStyle((QCPGraph::LineStyle)1); // line
    plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

    QVector<double> y(graphData->size());
    for (int j=0; j<graphData->size(); ++j)
    {
        y[j] = j;
    }

    plot->graph()->setData(y, *graphData);
    plot->graph()->rescaleAxes(true);
    lines += 2;
    return true;
}*/
