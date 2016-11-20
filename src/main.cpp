#include <iostream>
#include <dbus/dbus.h>
#include <deque>
#include <stdlib.h>
#include "gestusConnection.h"
#include <thread>
#include <time.h>
#include <unistd.h>
using namespace std;
//void threadFunction(GestusConnection)
int main()
{
    string characteristic = "magnet";

    deque<string> buffer;

    GestusConnection connection;
    connection.setAvalibleDevices();
    connection.getData(0, characteristic, &buffer);
    //thread thr(&GestusConnection::connectAndRead, &connection, 0, characteristic, &buffer);
    //thr.detach();
    do
    {
        if(!buffer.empty())
        {
            cout<<buffer.front()<<endl;
            buffer.pop_front();
        }
        //else
            //cout<<"is empty"<<endl;
    }while(TRUE);
       
      return 0;
}
