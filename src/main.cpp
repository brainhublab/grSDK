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

    connection.setUpRfcomm("/dev/rfcomm0");
    std::string str;
    while(true)
    {
        
        std::cout << "Something";

        str = connection.getNext();
        std::cout<<str;
        str.clear();
    } 

    //connection.setAvalibleDevices();
    //connection.getData(0, characteristic, &buffer);
    //thread thr(&GestusConnection::connectAndRead, &connection, 0, characteristic, &buffer);
    //thr.detach();
    /*
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
      */ 
      return 0;
}
