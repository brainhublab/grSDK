#ifndef DATAPLOTTER_H
#define DATAPLOTTER_H

#include <deque>
#include <sstream>
#include "externAssets/qcustomplot/qcustomplot.h"
#include "QObject"
#include <string>

class DataPlotter : public QObject
{
    Q_OBJECT

public:
    DataPlotter( QCustomPlot *);
    ~DataPlotter();

    bool setupPlot(std::deque<std::string>* );
    bool showPlot();

    bool drawPlotByAxis(std::deque<std::string>* buffer, size_t axis, QString label);
    void drawPlot(std::deque<std::string>* buffer);
    bool drawPlotFromBuffer();
public slots:
    void fetchData();

private:

    QCustomPlot* plot = nullptr;
    QTimer *dataTimer = nullptr;
    std::deque<std::string> *buffer = nullptr;
};

#endif // DATAPLOTTER_H
