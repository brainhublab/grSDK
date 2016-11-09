#include <iostream>
#include <dbus/dbus.h>


#include "gestusConnection.h"
using namespace std;

int main()
{
    GestusConnection connection;
    cout<<"main OK"<<endl;
    string adaptername;
    adaptername = connection.getAdapterName();
    cout<<adaptername<<endl;
    bool flag;
    flag = connection.setAvalibleDevices();
    return 0;
}
