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
    sensors.push_back("palm");
    sensors.push_back("pinky");
    string alg="DTW";
    utils.setDatasetProperties("testDataset", "Test info text", "test", alg);
    utils.setSensors(sensors, alg);


    int ch;
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr,TRUE);

    while(ch!='q')
    {
        ch=getch();
        if(ch == 'r')
        {
            cout<<"saving"<<endl;
            while(ch!='s')
            {
                cout<<"reading"<<endl;
                ch = getch();
                if(conn.getData(&dev))
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

    return EXIT_SUCCESS;
}
