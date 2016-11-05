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
using namespace std;
using namespace libconfig;
struct adapter_t
{
    map<string, string> bluezAttributes;
    string adapterPath;
    map<string, string> interfaces;

    adapter_t()
    {
        Config cfg;
        try
        {
            cfg.readFile("./config/adapter.cfg");                
        }
        catch(const FileIOException &fieox)
        {
            cerr << "I/O error while reading adapter config file" << endl;
            //return(EXIT_FAILURE);
        }
        catch(const ParseException &pex)
        {
            cerr << "Parse error at" << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError()
                << endl;
            //return(EXIT_FAILURE);

        }

        const Setting& root = cfg.getRoot();
        try
        {
            const Setting  &bluez = root["bluez"];
            int bluezCount = bluez.getLength();

            for(int i=0; i<bluezCount; i++)
            {
                string sett, val;
                sett = bluez[i].getName();
                bluez.lookupValue(sett, val);
                bluezAttributes[sett]=val;
                sett.clear();
                val.clear();

            }
            //bluez.lookupValue("path", bluezPath);
        }
        catch(const SettingNotFoundException &nfex)
        {
            //ignore
        }
        try
        {
            const Setting &adapter = root["adapter"];
            adapter.lookupValue("path", adapterPath);

            const Setting &iface = root["adapter"]["interfaces"];
            int interfacesCount = iface.getLength();

            for(int i=0; i<interfacesCount; i++)
            {
                string sett, val;
                sett = iface[i].getName();
                iface.lookupValue(sett, val);
                interfaces[sett.c_str()] = val.c_str();

                sett.clear();
                val.clear();   
            }
        }
        catch(const SettingNotFoundException &nfex)
        {
            //Ignore
        }
    }

};
struct device_t
{   
    //TODO mor devices implement
    map<string, string> deviceUuids;

    device_t()
    {
        Config cfg;
        try
        {
            cfg.readFile("./config/device.cfg");

        }
        catch(const FileIOException &fioex)
        {
            cerr << "I/O error while reading device config file" << endl;
            //return(EXIT_FAILURE);
        }
        catch(const ParseException &pex)
        {
            cerr << "Parse error at" << pex.getFile() << ":"<<pex.getLine() << " - " << pex.getError() <<
                endl; 
        }

        const Setting& root = cfg.getRoot();

        try
        {
            const Setting& device = root["device"];
            //implement more settings if needed
            const Setting& uuids = root["device"]["uuids"];
            int uuidCount = uuids.getLength();

            for(int i=0; i<uuidCount; i++)
            {
                string key, val;
                key = uuids[i].getName();
                uuids.lookupValue(key, val);
                deviceUuids[key.c_str()] = val.c_str();
                key.clear();
                val.clear();
            }
        }
        catch(const SettingNotFoundException &nfex)
        {
            //Ignore
        }

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
        string getAvalibleDevices(); //gets the avalible for connection gestus devices

        bool connectAndRead();
        
    private:
        //private variables
        
        adapter_t adapter; //name of adapter in pc
        device_t devices; //names gesture input devices
        map<string, map<string, map<string, vector<string> > > > connectedDevices;   
        char* message;
        //Dbus variables
        DBusConnection *conn;
        DBusMessage *msg, *reply;

        //private methods
        bool getReply(DBusMessage* , string*);
        bool findAllDevices(DBusMessage* ,map<string, map<string, map<string, vector<string> > > >* );
        bool iterAllDevices(DBusMessageIter* , DBusMessageIter*, int*, int);
};

#endif
