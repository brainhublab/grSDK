#include <iostream>
#include <sstream>
#include <dbus/dbus.h>
#include <deque>
#include <stdlib.h>
#include "grConnection.h"
#include <thread>
#include <time.h>
#include <unistd.h>
//#include <QApplication>
//#include <GL/glut.h>

//#include "grVisualization.h"
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

    while(true)
    {
        //cout<<"size: " << dev.pinky.gyro.size()<<endl;
        if(dev.pinky.gyro.front().size() != 0 && dev.pinky.gyro.front().size() == 3)
        {
            if(algDev.pinky.size() != 0 && algDev.pinky.front().size() == 4)
            {
                for(int i=0; i<4; i++)
                {
                    cout<<algDev.pinky.front()[i]<<" ";
                }
                std::cout<<std::endl;

                algDev.pinky.pop_front();
            }
            dev.pinky.gyro.pop_front();

        }

    }

    //w.realTimeDrowData(&buffer);

	return 0;
}
