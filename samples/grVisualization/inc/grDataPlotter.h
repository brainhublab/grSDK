#ifndef DATAPLOTTER_H
#define DATAPLOTTER_H

#include <deque>
#include <sstream>
#include <string>

#include "qcustomplot.h"
#include "QObject"

class GRDataPlotter : public QObject
{
    Q_OBJECT

public:
    GRDataPlotter( QCustomPlot *);
    ~GRDataPlotter();

    bool setupPlot(std::deque<std::vector<float>>* );
    bool runPlotting();

    bool pause = false;
public slots:
    void fetchData();

private:

	void splitSensorData(std::string str, double arr[3]);

    QCustomPlot* plot = nullptr;
    QTimer *dataTimer = nullptr;
    std::deque<std::vector<float>> *buffer = nullptr;
};

#endif // DATAPLOTTER_H
