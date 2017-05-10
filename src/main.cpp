#include <iostream>
#include <sstream>
#include <dbus/dbus.h>
#include <deque>
#include <stdlib.h>

#include <thread>
#include <time.h>
#include <unistd.h>
/*
#include <QApplication>
#include <GL/glut.h>
#include "grVisualization.h"
*/
#include "grUtilities.h"
#include "grConnection.h"
#include "grAlgorithm.h"
#include "grDevice.h"
#include "GRT/GRT.h"
#include <ncurses.h>
using namespace GRT;
using namespace std;
int main (int argc, const char * argv[])
{
    GRConnection conn;
    device_t dev;
    GRUtilities utils;

    vector<string> sensors;// ("palm", "pinky");
    sensors.push_back("pinky");
    sensors.push_back("ring");
    sensors.push_back("middle");
    sensors.push_back("index");
    sensors.push_back("thumb");
    sensors.push_back("palm");
    
    string alg="DTW";
    utils.setDatasetProperties("testDataset", "Test info text", "test", alg);
    utils.setSensors(sensors, alg);


    int ch;
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr,TRUE);

   for(int i=0; i<2; i++)
    {
        conn.getData(&dev);
        dev.pinky.gyro.pop_front();
        dev.pinky.acc.pop_front();
        dev.pinky.mag.pop_front();
        dev.ring.gyro.pop_front();
        dev.ring.acc.pop_front();
        dev.ring.mag.pop_front();
        dev.middle.gyro.pop_front();
        dev.middle.acc.pop_front();
        dev.middle.mag.pop_front();
        dev.index.gyro.pop_front();
        dev.index.acc.pop_front();
        dev.index.mag.pop_front();
        dev.thumb.gyro.pop_front();
        dev.thumb.acc.pop_front();
        dev.thumb.mag.pop_front();
        dev.palm.gyro.pop_front();
        dev.palm.acc.pop_front();
        dev.palm.mag.pop_front();
        usleep(20);

    }
    while(ch!='q')
    {
        ch=getch();
        if(ch == 'r')
        {
            cout<<"saving"<<endl;
            while(ch!='s' && conn.getData(&dev))
            {
                cout<<"reading"<<endl;
                ch = getch();
                if(true)
                {
                    usleep(20);
                    utils.pushDatasetDTW(&dev);
                    usleep(20); 
                }
            }
        }
        else if(ch == 'n')
        {              
            utils.saveDataset(alg);

            utils.setNextLabel(alg);
            cout<<"NEXT GESTURE"<<endl;
        }

    }
    utils.clearTrainingData("DTW");

    return EXIT_SUCCESS;
}
