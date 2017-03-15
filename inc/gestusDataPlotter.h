#ifndef DATAPLOTTER_H
#define DATAPLOTTER_H

#include <deque>
#include <sstream>
#include <string>

#include "externAssets/qcustomplot/qcustomplot.h"
#include "externAssets/plog/Log.h" //Lib for logging in csv format
#include "QObject"

class DataPlotter : public QObject
{
    Q_OBJECT

public:
    DataPlotter( QCustomPlot *);
    ~DataPlotter();

    bool setupPlot(std::deque<std::string>* );
    bool drawPlotFromBuffer();
    bool logger(bool);
    bool pushData(std::string);
public slots:
    void fetchData();

private:

    QCustomPlot* plot = nullptr;
    QTimer *dataTimer = nullptr;
    std::deque<std::string> *buffer = nullptr;
    bool isLoggingEnabled = false;
};

#endif // DATAPLOTTER_H
