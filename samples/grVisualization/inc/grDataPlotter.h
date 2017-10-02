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

    bool setupPlot(std::deque<std::vector<float>>* ); // assigns buffer and makes new axis on plot
    bool runPlotting(); // runs timer for fetchData()

    bool pause = false;
public slots:
    void fetchData(); // pops front element from buffer and adds it to plot

private:
    void splitSensorData(std::string str, double arr[3]);
    int fetchFrequency = 50;
    QCustomPlot* plot = nullptr; // pointer to plot GUI element
    QTimer *dataTimer = nullptr;
    std::deque<std::vector<float>> *buffer = nullptr; // pointer to buffer of data
};

#endif // DATAPLOTTER_H
