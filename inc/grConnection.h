#ifndef GR_CONNECTION
#define GR_CONNECTION

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

using namespace std;
using namespace libconfig;

struct dbus_bluez
{
    string name;
    string path;
    string properties;
    string introspectable;
    dbus_bluez()
    {
        name = "org.bluez";
        path = "/org/bluez";
        properties = "org.freedesktop.DBus.Properties";
        introspectable = "org.freedesktop.DBus.Introspectable";

    }

};
struct device_names
{
    string left;
    string right;
    device_names()
    {
        left = "GR[L]";
        right = "GR[R]";
    }

};
struct device_uuids
{
    string fingers;
    string gyro;
    string accelerometer;
    string magnet;

    string rx;
    string tx;

    string tmp_uuid;
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

struct device_t
{
   int id;
   string name;
   string path;

   map<string, string> interfaces;

   string fingers;
   string gyro;
   string accelerometer;
   string magnet;
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

class GRConnection
{
    public:
        GRConnection();
        ~GRConnection();
        GRConnection(const GRConnection& );
        GRConnection& operator=(const GRConnection& );


        bool setAdapterName(); //gets the pc bluetooth adapter name
        string getAdapterName();
        device_t getAdapter();

        int getDeviceId(device_t);
        string getDeviceName(device_t);
        vector<device_t> getConnectedDevices();

        bool getData(int, string, deque<string>*);
        bool connectAndRead(device_t, string, deque<string>* );

        bool setAvalibleDevices();
    private:
        //private variables

        device_t adapter; //name of adapter in pc
        vector<device_t> devices; //names gesture input devices
        char* message;

        //private methods
        bool setDeviceCharacteristics(device_t* , string);
        DBusMessage *getProperty(const char*, const char*, const char*);
        bool parsePropertyArray(DBusMessage*, vector<string>*);
        bool parsePropertyString(DBusMessage*, string*);
        bool iterDevices(DBusMessageIter*, vector<string>*, int, int);
        bool getReply(DBusMessage* , string*);

        //constants
        dbus_bluez dbusBluez;
        device_names deviceNames;
        device_uuids uuids;
};

#endif
