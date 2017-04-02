#ifndef DATAPLOTTER_H
#define DATAPLOTTER_H

#include <deque>
#include <sstream>
#include <string>

#include "externAssets/qcustomplot/qcustomplot.h"
#include "QObject"

class GRDataPlotter : public QObject
{
    Q_OBJECT

public:
    GRDataPlotter( QCustomPlot *);
    ~GRDataPlotter();

    bool setupPlot(std::deque<std::string>* );
    bool drawPlotFromBuffer();

    void splitSensorData(std::string str, double arr[3]);
    bool pause = false;
public slots:
    void fetchData();

private:

    QCustomPlot* plot = nullptr;
    QTimer *dataTimer = nullptr;
    std::deque<std::string> *buffer = nullptr;
};

#endif // DATAPLOTTER_H
