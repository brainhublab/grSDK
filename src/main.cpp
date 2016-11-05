#include <iostream>
#include <dbus/dbus.h>


#include "gestusConnection.h"
using namespace std;

int main()
{
    GestusConnection connection;
    string adptName = connection.getAvalibleDevices();
    cout<<adptName;
    return 0;
}
