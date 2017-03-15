#ifndef GESTUS_CONNECTION
#define GESTUS_CONNECTION

#include <grBluezAttributes.h>

#include <dbus/dbus.h>
#include <cassert>
#include <iostream>
#include <libconfig.h++>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <map>
#include <deque>
#include <typeinfo>
#include <thread>

class GestusConnection
{
    public:
        GestusConnection(); // constructor
        ~GestusConnection(); // destructor
        GestusConnection(const GestusConnection& ); // copy construcor
        GestusConnection& operator=(const GestusConnection& ); // operator =


        bool setAdapterName(); //set the nternal pc adapter parameters and stor them in object
        std::string getAdapterName(); // return the name of internal adapter
        device_t getAdapter(); // return the object adapter 

        int getDeviceId(device_t); // return the ID of device setted inparameter
        std::string getDeviceName(device_t); // return the device name setted in parameter
        std::vector<device_t> getConnectedDevices(); // returns vector filled with connected device structures

        bool getData(int, std::string, std::deque<std::string>*); // fiil buffer with data taked from device wit ID from parameter and their characteristc 
        bool connectAndRead(device_t, std::string, std::deque<std::string>* ); // find device in DBUS tree, connect and read data from needed characteristic 

        bool setAvalibleDevices(); // iterate all connected devices select the GR devices and store them 
    private:
        //private variables
        device_t adapter; //name of adapter in pc
        std::vector<device_t> devices; //names gesture input devices
        char* message;

        //private methods
        bool setDeviceCharacteristics(device_t* , std::string); // sed default device characteristics
        DBusMessage *getProperty(const char*, const char*, const char*); // return properties of device
        bool parsePropertyArray(DBusMessage*, std::vector<std::string>*); // parse properties with needed from aray 
        bool parsePropertyString(DBusMessage*, std::string*); // parse proerties with needed from string
        bool iterDevices(DBusMessageIter*, std::vector<std::string>*, int, int); // iterate DBUS tree with devices
        bool getReply(DBusMessage* , std::string*); // store reply as string

        //constants
        dbus_bluez dbusBluez; // DBUS bluez object
        device_names deviceNames; // names neede for parsing
        device_uuids uuids; // uuids of sensors
};

#endif
