#ifndef GR_BUFFER_MANAGER_H
#define GR_BUFFER_MANAGER_H

#include <QObject>
#include <QTimer>

#include <deque>
#include <string>

#include "plog/Log.h" //Lib for logging in csv format

#include "GRGLWidget.h"

#include <iostream>

#include "grConnection.h"
#include "grAlgorithm.h"

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

    /*void splitSensorData(std::vector<float> quat, double arr[4])
    {
        arr[0] = data[1];
       //      int i = 0;
       //      double n;
       //      std::stringstream ss(str);
       //      while(ss >> n)
       //      {
    			// arr[i] = n;
       //              i++;
       //      }
    };
    */
    //
	bool setupSource();
    QTimer* fetchTimer;

	std::deque<std::vector<float>>* sourceBuffer;

    std::deque<std::vector<float>>* firstBuffer;
    std::deque<std::vector<float>>* secondBuffer;


        bool isLoggingEnabled = false;
public slots:
    void fetchData();
private:
      GRGLWidget* widget;
	  device_t dev;
	  alg_device_t algDev;
	  GRConnection conn;
	  GRAlgorithm alg;
	   GRAlgorithm alg1;
		GRAlgorithm alg2;
		 GRAlgorithm alg3;
		  GRAlgorithm alg4;
		   GRAlgorithm alg5;
};

#endif // GR_BUFFER_MANAGER_H
