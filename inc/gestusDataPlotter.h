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
};

#endif // DATAPLOTTER_H
