#include <iostream>
#include <dbus/dbus.h>
#include <deque>
#include <stdlib.h>
#include "grConnection.h"
#include <thread>
#include <time.h>
#include <unistd.h>
using namespace std;
//void threadFunction(GestusConnection)
int main()
{
    string characteristic = "pinky_gyro";

    deque<string> buffer;

    GRConnection connection;

    connection.readData();
   return 0;
}
