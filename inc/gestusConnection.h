#ifndef GESTUS_CONNECTION
#define GESTUS_CONNECTION

#include <dbus/dbus.h>
#include <cassert>
#include <iostream>i
#include <libconfig.h++>

using namespace std;

class GestusConnection
{
    struct adapter
    {
        //TODO paths to adapter and configs
    }
    struct device 
    {
        //TODO pths and device configs
    }
    public:
        GestusConnection();
        ~GestusConnection();
        GestusConnection(const GestusConnection& );
        GestusConnection& operator=(const GestusConnection& );

        char* getAdapterName(); //gets the pc bluetooth adapter name
        char* getAvalibleDevices(); //gets the avalible for connection gestus devices

        bool connectToDevice(char*); //make a connection with device by name

        bool recieveMessages(char* ); // method for recieving messages from gestus device the parameter is DBus uuid of service

    private:
        //private variables
        char* adapter_name; //name of adapter in pc
        char** devices; //names gesture input devices

        char* message;

        //private dbus variables
        DBusConnection* conn;
        DBusMessage* msg;
        DBusMessage* reply;
        DBusError* errori;
        


        //private methods
        bool getReply(DBusMessage* );
        bool readConfig(char*);
};

#endif
