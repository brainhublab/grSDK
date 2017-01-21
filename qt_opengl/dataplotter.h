#ifndef DATAPLOTTER_H
#define DATAPLOTTER_H

#include <deque>
#include <sstream>
#include "qcustomplot/qcustomplot.h"
#include "QObject"
#include <string>

class DataPlotter : public QObject
{
    Q_OBJECT

public:
    DataPlotter( QCustomPlot *);
    ~DataPlotter() {}

    bool drawPlotByAxis(std::deque<std::string>* buffer, size_t axis, QString label);

    bool showPlot();

    void drawPlot(std::deque<std::string>* buffer);

    //my code
    bool drawPlotFromBuffer();
    bool setupPlot(std::deque<std::string>* );

public slots:
    void fetchData();

private:

    QCustomPlot* plot = nullptr;
    std::deque<std::string> *buffer = nullptr;
    size_t lines = 2; // refactor it!
    void splitSensorData(std::string str, double arr[3])
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


   // bool splitDataInVectors(const std::deque<std::string>* d, QVector<double>* xs,QVector<double>* ys, QVector<double>* zs);

    //bool plotGyroGraph(QVector<double>* graphData, size_t axis, QString label);
};

#endif // DATAPLOTTER_H
