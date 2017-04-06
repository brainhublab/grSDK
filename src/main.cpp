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
using namespace std;
int main(int argc, char *argv[])
{
    
    device_t dev;

	GRConnection connection;
    cout<<endl;
    connection.getData(&dev);
    cout<<"asd"<<endl;

    while(true)
    {
        //cout<<"size: " << dev.pinky.gyro.size()<<endl;

        if(dev.pinky.gyro.size() != 0 && dev.pinky.gyro.front().size() == 3)
        {
            for(int i=0; i<3; i++)
            {
                cout<<dev.pinky.gyro.front()[i];
            }
            std::cout<<std::endl;

            dev.pinky.gyro.pop_front();
        }
    }
	
    //w.realTimeDrowData(&buffer);

	return 0;
}
