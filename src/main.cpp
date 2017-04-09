#include <iostream>
#include <sstream>
#include <dbus/dbus.h>
#include <deque>
#include <stdlib.h>

#include <thread>
#include <time.h>
#include <unistd.h>

#include <QApplication>
#include <GL/glut.h>
#include "grVisualization.h"

#include "grConnection.h"
#include "grAlgorithm.h"
using namespace std;
int main(int argc, char *argv[])
{
  
    device_t dev;

	GRConnection connection;
    //connection.connect("rfcomm0", "98:D3:32:10:AC:59", "1" );
     sleep(2); 
    cout<<endl;
    connection.getData(&dev);
    cout<<"asd"<<endl;
    sleep(5);

    alg_device_t algDev;
    GRAlgorithm algr;
   // while(dev.pinky.gyro)
    algr.madgwickAHRS(&dev, &algDev);
    std::cout<<"MadgwickAHRS is started"<<std::endl;
   sleep(10); 
/*
    while(true)
    {
       // cout<<"while"<<endl;
            if(algDev.palm.size() != 0 && algDev.palm.front().size() == 4)
            {
               // cout<<algDev.palm.size()<<"palm size"<<endl;
                cout<<algDev.palm.front().size()<<"front size"<<endl;

                for(int i=0; i<4; i++)
                {
                   cout<<algDev.palm.front()[i]<<" ";
                }
                std::cout<<std::endl;

                algDev.palm.pop_front();
            }

    }

*/
    sleep(5);
    glutInit(&argc, argv);
    QApplication a(argc, argv);
    GRVisualization w;
    w.setupPlotters(&algDev.palm);
    w.show();
    return a.exec();
        
    //w.realTimeDrowData(&buffer);

//return 0;
}
