#include <iostream>
#include <dbus/dbus.h>
#include <deque>
#include <stdlib.h>
#include "gestusConnection.h"
using namespace std;

int main()
{
    string characteristic = "gyro";

    deque<string> buffer;

    GestusConnection connection;
    connection.setAvalibleDevices();
    connection.connectAndRead(0, characteristic, &buffer);

    char str;
   // sleep(50);
    /*if(!buffer.empty())
    {
        cout<<"bufer is empty";
    }
    while(!buffer.empty())
    {
        cout<<"reading";
        cout<<buffer.front();
       // cout<<str<<endl;
        buffer.pop_front();

        } 

*/
    return 0;
}
