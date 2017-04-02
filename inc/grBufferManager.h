#ifndef GR_BUFFER_MANAGER_H
#define GR_BUFFER_MANAGER_H

#include <QObject>
#include <QTimer>

#include <deque>
#include <string>

#include "externAssets/plog/Log.h" //Lib for logging in csv format

#include "GRGLWidget.h"

class GRBufferManager : public QObject
{
  Q_OBJECT
public:
    GRBufferManager();
    ~GRBufferManager();
    bool setGLWidget(GRGLWidget* w)
    {
        widget = w;
        return true;
    };

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
    };
    //
    bool setupSource(std::deque<std::string>* buf);
    QTimer* fetchTimer;

    std::deque<std::string>* sourceBuffer = nullptr;

    std::deque<std::string>* firstBuffer;
    std::deque<std::string>* secondBuffer;


        bool isLoggingEnabled = false;
public slots:
    void fetchData();
private:
      GRGLWidget* widget;
};

#endif // GR_BUFFER_MANAGER_H
