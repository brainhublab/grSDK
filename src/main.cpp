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
using namespace stateestimation;
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
    
   double G[3], A[3], M[3];
// Declare an instance of the class (e.g. as a member of another class)
 AttitudeEstimator Est;
 // Initialise the estimator (e.g. in the class constructor, none of these are actually strictly required for the estimator to work, and can be set at any time)
 Est.setMagCalib(0.68, -1.32, 0.0);         // Recommended: Use if you want absolute yaw information as opposed to just relative yaw (Default: (1.0, 0.0, 0.0))
 Est.setPIGains(2.2, 2.65, 10, 1.25);       // Recommended: Use if the default gains (shown) do not provide optimal estimator performance (Note: Ki = Kp/Ti)
 Est.setQLTime(2.5);                        // Optional: Use if the default quick learning time is too fast or too slow for your application (Default: 3.0)
 Est.setAttitude(0.5, 0.5, 0.5, 0.5);       // Optional: Use if you have prior knowledge about the orientation of the robot (Default: Identity orientation)
 Est.setAttitudeEuler(3.14, 0.0, 0.0);      // Optional: Use if you have prior knowledge about the orientation of the robot (Default: Identity orientation)
 Est.setAttitudeFused(3.14, 0.0, 0.0, 1.0); // Optional: Use if you have prior knowledge about the orientation of the robot (Default: Identity orientation)
 Est.setGyroBias(0.152, 0.041, -0.079);     // Optional: Use if you have prior knowledge about the gyroscope bias (Default: (0.0, 0.0, 0.0))
 Est.setAccMethod(Est.ME_FUSED_YAW);        // Optional: Use if you wish to experiment with varying acc-only resolution methods
 
    do
    {
        if(!gyro.empty() )
        {
            getSensorData(gyro.front(), G);
            gyro.pop_front();

        }
        if(!accelerometer.empty())
        {
            getSensorData(accelerometer.front(), A);
            accelerometer.pop_front();
        }

        Est.update(0.20, G[0], G[1], G[2], A[0], A[1], A[2], 0, 0, 0);

        double q[4];
        Est.getAttitude(q);
        cout << "My attitude is (quaternion): (" << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << ")" << endl;
        cout << "My attitude is (ZYX Euler): (" << Est.eulerYaw() << "," << Est.eulerPitch() << "," << Est.eulerRoll() << ")" << endl;
        cout << "My attitude is (fused): (" << Est.fusedYaw() << "," << Est.fusedPitch() << "," << Est.fusedRoll() << "," << (Est.fusedHemi() ? 1 : -1) << ")" << endl;
    }while(TRUE);
       
      return 0;
}
