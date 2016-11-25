#include <iostream>
#include <sstream>
#include <dbus/dbus.h>
#include <deque>
#include <stdlib.h>
#include "gestusConnection.h"
#include <thread>
#include <time.h>
#include <unistd.h>
#include <attitude_estimator.h>
using namespace std;
//void threadFunction(GestusConnection)

void getSensorData(string str, double arr[3])
{
    int i = 0;
    double n;
    stringstream ss(str);
    while(ss >> n)
    {
        arr[i] = n;
        i++;
    }
}
int main()
{
    string accelerometer_char = "accelerometer";
    string gyro_char = "gyro";
    deque<string> accelerometer;
    deque<string> gyro;

    string a;
    string g;

    GestusConnection connection;
    connection.setAvalibleDevices();
    connection.getData(0, accelerometer_char, &accelerometer);
    connection.getData(0, gyro_char, &gyro);
    
    
    do
    {
        if(!accelerometer.empty() && !gyro.empty())
        {
            // cout<<accelerometer.front()<<endl;
            // cout<<gyro.front()<<endl;

            // accelerometer.pop_front();
            // gyro.pop_front();
            double arr[3];
            getSensorData(accelerometer.front(), arr);
            accelerometer.pop_front();

            for(int i = 0; i < 3; i++)
            {
                cout<<arr[i]<<endl;
            }
            cout<<endl;
        }
        //else
            //cout<<"is empty"<<endl;
    }while(TRUE);
       
      return 0;
}
