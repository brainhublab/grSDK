#ifndef GESTUS_CONNECTION
#define GESTUS_CONNECTION

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

/*
 * structure for default bluezobject and aarameters 
 */
struct dbus_bluez
{
    std::string name;
    std::string path;
    std::string properties;
    std::string introspectable;
    dbus_bluez()
    {
        name = "org.bluez";
        path = "/org/bluez";
        properties = "org.freedesktop.DBus.Properties";
        introspectable = "org.freedesktop.DBus.Introspectable";

    }

};
/*
 * structure with default names of GR devices
 */
struct device_names
{
    std::string left;
    std::string right;
    device_names()
    {
        left = "Gestus[L]";
        right = "Gestus[R]";
    }

};
/*
 * default GR device UUID's setted inhardware 
 */
struct device_uuids
{
    std::string fingers;
    std::string gyro;
    std::string accelerometer;
    std::string magnet;

    std::string rx;
    std::string tx;

    std::string tmp_uuid;
    device_uuids()
    {
         fingers = "fced6409-c015-45ea-b50d-1de32a1c2f6d";
         gyro = "fced640a-c015-45ea-b50d-1de32a1c2f6d";
         accelerometer = "fced640b-c015-45ea-b50d-1de32a1c2f6d";
         magnet = "fced640c-c015-45ea-b50d-1de32a1c2f6d";

         rx = "";
         tx = "";

         tmp_uuid = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";
    }
};
/*
 * structure for storing of device and adapter parameters
 */
struct device_t
{
   int id;
   std::string name;
   std::string path;

   std::map<std::string, std::string> interfaces;

   std::string fingers;
   std::string gyro;
   std::string accelerometer;
   std::string magnet;
   device_t()
   {
       id = 0;
       name.clear();
       path.clear();
       fingers.clear();
       gyro.clear();
       accelerometer.clear();
       magnet.clear();

       interfaces.clear();

    }

};

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
