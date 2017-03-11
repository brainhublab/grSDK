#ifndef DATAPLOTTER_H
#define DATAPLOTTER_H

#include <deque>
#include <sstream>
#include <string>

#include "externAssets/qcustomplot/qcustomplot.h"
#include "QObject"

class DataPlotter : public QObject
{
    Q_OBJECT

public:
    DataPlotter( QCustomPlot *);
    ~DataPlotter();

    bool setupPlot(std::deque<std::string>* );
    bool drawPlotFromBuffer();

public slots:
    void fetchData();

private:

    QCustomPlot* plot = nullptr;
    QTimer *dataTimer = nullptr;
    std::deque<std::string> *buffer = nullptr;
};

#endif // DATAPLOTTER_H
