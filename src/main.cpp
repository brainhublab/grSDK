#include <iostream>
#include <dbus/dbus.h>


#include "gestusConnection.h"
using namespace std;

int main()
{
   GestusConnection connection;
   connection.setAvalibleDevices();
   connection.connectAndRead(0);    

    return 0;
}
