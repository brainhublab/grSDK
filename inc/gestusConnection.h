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

#define BLUEZ_DBUS_NAME "org.bluez"
#define BLUEZ_DBUS_PATH "/org/bluez"

#define DEVICE_LEFT_NAME "Gestus[L]"
#define DEVIDE_RIGHT_NAME "Gestus[R]"


using namespace std;
using namespace libconfig;

struct device_t
{  
   string name; 
   string path;

   map<string, string> interfaces;

   device_t()
   {
       name.clear();
       path.clear();

       interfaces.clear();


    }
   
};

class GestusConnection
{
    public:
        GestusConnection();
        ~GestusConnection();
        GestusConnection(const GestusConnection& );
        GestusConnection& operator=(const GestusConnection& );

        string getAdapterName(); //gets the pc bluetooth adapter name

        bool connectAndRead();
        
        bool setAvalibleDevices();
    private:
        //private variables
        
        device_t adapter; //name of adapter in pc
        //vector<device_t> devices; //names gesture input devices
        char* message;
        
        //private methods
        bool getReply(DBusMessage* , string*);
        bool setDeviceInfo(device_t*, string);
        bool iterDevices(DBusMessageIter*, vector<string>*, int, int);

};

#endif
