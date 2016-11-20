#include "gestusConnection.h"


GestusConnection::GestusConnection() //default constructor
{
    message = NULL;

    adapter.name.clear();
    adapter.path = "/org/bluez/hci0";
    adapter.interfaces["adapter1"] = "org.bluez.Adapter1";
    adapter.interfaces["gattManager"] = "org.bluez.GattManager1";
    adapter.interfaces["introspectable"] = "org.freedesktop.DBus.Introspectable";
    adapter.interfaces["properties"] = "org.freedesktop.DBus.Properties";






}

GestusConnection::~GestusConnection() //destructor
{

}

GestusConnection::GestusConnection(const GestusConnection& connection)
{

}

GestusConnection& GestusConnection::operator =(const GestusConnection& connection)
{

}

bool GestusConnection::setAdapterName()
{
    DBusConnection *conn;
    DBusMessage *msg, *reply;

    DBusError* derror;
    string adapterName;

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
    msg = dbus_message_new_method_call(
            dbusBluez.name.c_str(),
            adapter.path.c_str(),
            adapter.interfaces.find("properties")->second.c_str(),
            "Get"
            );


    const char* interface = adapter.interfaces.find("adapter1")->second.c_str();
    const char* property = "Name";
    dbus_message_append_args(msg,
            DBUS_TYPE_STRING, &interface,
            DBUS_TYPE_STRING, &property,
            DBUS_TYPE_INVALID);

    reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, derror);
    
    if(reply == NULL)
    {
        cout<<"Can't find any adapters"<<endl;
        return FALSE;
    }

    adapterName.clear();
    getReply(reply, &adapterName);
    adapter.name = adapterName;

    dbus_message_unref(msg);
    dbus_message_unref(reply);

    adapterName.clear();

    return TRUE;
}
string GestusConnection::getAdapterName()
{
    return adapter.name;
}

device_t GestusConnection::getAdapter()
{
    return adapter;
}

int GestusConnection::getDeviceId(device_t dev)
{
    return dev.id;
}

string GestusConnection::getDeviceName(device_t dev)
{
    return dev.name;
}
vector<device_t> GestusConnection::getConnectedDevices()
{
    return devices;
}
bool GestusConnection::getData(int devId, string characteristic, deque<string>* buffer)
{
    device_t dev;
    for(int i=0; i < devices.size(); i++)
    {
        if(devices[i].id == devId)
        {
            dev = devices[i];

            //cout<<dev.name;
        }
        else
        {
            cout<<"Can't find devices wit this: "<<devId<< "id"<<endl;

        }
    }

    if(characteristic == "fingers")
    {
        characteristic = dev.fingers;
    }
    else if(characteristic == "gyro")
    {
        characteristic = dev.gyro;
    }
    else if(characteristic == "accelerometer")
    {
        characteristic = dev.accelerometer;
    }
    else if(characteristic == "magnet")
    {
        characteristic = dev.magnet;
    }
   /* else
    {
        cout<<"Can't find this: "<<characteristic<<"characteristic"<<endl;
    }
*/
       
    thread thr(&GestusConnection::connectAndRead, this, dev, characteristic, buffer);
    thr.detach(); 

   return TRUE;
}
bool GestusConnection::connectAndRead(device_t dev, string characteristic, deque<string> *buffer)
{

    DBusConnection *conn=NULL;
    DBusMessage *msg=NULL, *reply=NULL;
    DBusError *derror=NULL;

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);

    string s;

    while(TRUE)
    {
        msg = dbus_message_new_method_call(
                dbusBluez.name.c_str(),
                characteristic.c_str(), 
                "org.bluez.GattCharacteristic1",
                "ReadValue"
                );

        reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, derror);
        DBusMessageIter rootIter;
        if(reply == NULL)
        {   
            cout<<"Can't read value from device "<<dev.name<<" with characteristic"<<characteristic<<endl;
            return FALSE;
        }
        dbus_message_iter_init(reply, &rootIter);
        int currentType;

        while(dbus_message_iter_get_arg_type(&rootIter) != DBUS_TYPE_INVALID)
        {
            currentType = dbus_message_iter_get_arg_type(&rootIter);
            if(currentType == DBUS_TYPE_ARRAY)
            {
                DBusMessageIter subIter;
                dbus_message_iter_recurse(&rootIter, &subIter);
                while(dbus_message_iter_get_arg_type(&subIter) != DBUS_TYPE_INVALID)
                {
                    currentType = dbus_message_iter_get_arg_type(&subIter);
                    if(currentType == DBUS_TYPE_BYTE)
                    {
                        char str, str1;
                        dbus_message_iter_get_basic(&subIter, &str);
                        s.push_back(str);

                    }
                    dbus_message_iter_next(&subIter);

                }

                buffer->push_back(s);
                s.clear();

            }
            dbus_message_iter_next(&rootIter);
        }

    }
    return TRUE;
}


bool GestusConnection::setAvalibleDevices()
{
    vector<string> allDevicesPaths;
    int pathArbitor = 0;
    int layerArbitor = 0;

    string currentPath;
    string devName;
    int devId=0;

    DBusConnection *conn=NULL;
    DBusMessage  *msg=NULL, *reply=NULL;
    DBusError *derror=NULL;

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
    msg = dbus_message_new_method_call(
            dbusBluez.name.c_str(),
            "/",
            "org.freedesktop.DBus.ObjectManager",
            "GetManagedObjects");
    reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, derror);
    DBusMessageIter iterIn;
    dbus_message_iter_init(reply, &iterIn);

    iterDevices(&iterIn, &allDevicesPaths, pathArbitor, layerArbitor);

    const char* iface = "org.bluez.Device1";
    const char* prop = "Name";

    while(!allDevicesPaths.empty())
    {
        currentPath = allDevicesPaths.back();
        msg = dbus_message_new_method_call(
                dbusBluez.name.c_str(),
                currentPath.c_str(),
                dbusBluez.properties.c_str(),
                "Get"
                );

        dbus_message_append_args(msg,
                DBUS_TYPE_STRING, &iface,
                DBUS_TYPE_STRING, &prop,
                DBUS_TYPE_INVALID);
        reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, NULL);

        getReply(reply, &devName);

        if(devName == deviceNames.left || devName == deviceNames.right)
        {
            device_t dev;
            dev.id = devId;
            dev.name = devName;
            dev.interfaces["device1"] = "org.bluez.Device1";
            dev.interfaces["introspectable"] = dbusBluez.introspectable;
            dev.interfaces["properties"] = dbusBluez.properties;

            setDeviceCharacteristics(&dev, currentPath);
            devices.push_back(dev);
        }
        /*
        else 
        {
            cout<<"Not found any Gestus Devices :("<<endl;
            return FALSE;
        }*/

        allDevicesPaths.pop_back();
        currentPath.clear();
        devId++;

    }
    devName.clear();
    //cout<<devices.front().fingers<<endl;
    return TRUE;

}

bool GestusConnection::setDeviceCharacteristics(device_t* device, string devicePath)
{

    vector<string> gattServices;
    DBusMessage *reply = NULL;

    reply = getProperty(devicePath.c_str(), "org.bluez.Device1", "GattServices");
    if(parsePropertyArray(reply, &gattServices))
    {
        for(int i = 0; i < gattServices.size(); i++)
        {
            //cout<<gattServices[i]<<endl;

            vector<string> characteristics;

            // TODO: free reply
            reply = NULL;
            reply = getProperty(gattServices[i].c_str(), "org.bluez.GattService1", "Characteristics");
            if(parsePropertyArray(reply, &characteristics))
            {
                for(int j = 0; j < characteristics.size(); j++)
                {
                    //cout<<characteristics[j]<<endl;

                    string currentUUID;

                    reply = NULL;
                    reply = getProperty(characteristics[j].c_str(), "org.bluez.GattCharacteristic1", "UUID");

                    if(parsePropertyString(reply, &currentUUID))
                    {
                        //cout<<currentUUID<<endl;
                        if(currentUUID == uuids.fingers)
                        {
                            device->fingers = characteristics[j];
                        }
                       else if(currentUUID == uuids.gyro)
                       {
                            device->gyro = characteristics[j];
                       }
                       else if(currentUUID == uuids.accelerometer)
                       {
                            device->accelerometer = characteristics[j];
                       }
                       else if(currentUUID == uuids.magnet)
                       {
                            device->magnet = characteristics[j];
                       }
                       /*else
                       {
                           cout<<"Cant find any characteristic please check yhe device configs"<<endl;
                           return FALSE;
                       }*/
                    }
                    else
                    {
                        cout<<"Can't parse property string check configs"<<endl;
                        return FALSE;
                    }
                }
            }
            else
            {
                cout<<"Cant parse parsePropertyArray check configs"<<endl;
                return FALSE;
            }
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

DBusMessage *GestusConnection::getProperty(const char *objectPath, const char *iface, const char *prop)
{
    DBusConnection *conn=NULL;
    DBusMessage  *msg=NULL;
    DBusError  *derror=NULL;

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
    msg = dbus_message_new_method_call(
            dbusBluez.name.c_str(),
            objectPath,
            dbusBluez.properties.c_str(),
            "Get");

    dbus_message_append_args(msg,
            DBUS_TYPE_STRING, &iface,
            DBUS_TYPE_STRING, &prop,
            DBUS_TYPE_INVALID);

    return dbus_connection_send_with_reply_and_block(conn, msg, -1, derror);
}

bool GestusConnection::parsePropertyArray(DBusMessage *reply, vector<string> *res)
{
    DBusMessageIter rootIter;
    dbus_message_iter_init(reply, &rootIter);

    if(dbus_message_iter_get_arg_type(&rootIter) == DBUS_TYPE_VARIANT)
    {
        DBusMessageIter arrIter;
        dbus_message_iter_recurse(&rootIter, &arrIter);

        if(dbus_message_iter_get_arg_type(&arrIter) == DBUS_TYPE_ARRAY)
        {
            DBusMessageIter objIter;
            dbus_message_iter_recurse(&arrIter, &objIter);

            while(dbus_message_iter_get_arg_type(&objIter) != DBUS_TYPE_INVALID)
            {
                if(dbus_message_iter_get_arg_type(&objIter) == DBUS_TYPE_OBJECT_PATH)
                {
                    char *str = NULL;

                    dbus_message_iter_get_basic(&objIter, &str);

                    res->push_back(string(str));
                }
                else
                {
                    return FALSE;
                }

                dbus_message_iter_next(&objIter);
            }
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }

    return TRUE;
}

bool GestusConnection::parsePropertyString(DBusMessage *reply, string *res)
{
    DBusMessageIter rootIter;
    dbus_message_iter_init(reply, &rootIter);

    if(dbus_message_iter_get_arg_type(&rootIter) == DBUS_TYPE_VARIANT)
    {
        DBusMessageIter strIter;
        dbus_message_iter_recurse(&rootIter, &strIter);

        if(dbus_message_iter_get_arg_type(&strIter) == DBUS_TYPE_STRING)
        {
            char *str = NULL;
            dbus_message_iter_get_basic(&strIter, &str);
            res->assign(str, strlen(str));
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    return FALSE;
}

bool GestusConnection::iterDevices(DBusMessageIter* iterIn, vector<string>* devices, int pathArbitor, int layerArbitor)
{
    int next = layerArbitor + 1;

    int currentType;
    while(dbus_message_iter_get_arg_type(iterIn) != DBUS_TYPE_INVALID)
    {
        currentType = dbus_message_iter_get_arg_type(iterIn);
        if(currentType == DBUS_TYPE_ARRAY ||
                currentType == DBUS_TYPE_VARIANT ||
                currentType == DBUS_TYPE_DICT_ENTRY)
        {
            DBusMessageIter subIterIn;
            dbus_message_iter_recurse(iterIn, &subIterIn);

            iterDevices(&subIterIn, devices, pathArbitor, next);

        }
        else if(currentType == DBUS_TYPE_STRING ||
                currentType == DBUS_TYPE_OBJECT_PATH)
        {

            if(currentType == DBUS_TYPE_OBJECT_PATH && layerArbitor == 2)
            {
                pathArbitor += 1;
                    char* str = NULL;
                    dbus_message_iter_get_basic(iterIn, &str);
                    string devicePath;
                    devicePath.assign(str, strlen(str)+1);
                    if(devicePath.length() == 38)
                    {
                        devices->push_back(devicePath);
                    }
                    devicePath.clear();
            }

        }
        dbus_message_iter_next(iterIn);

    }
}
bool GestusConnection::getReply(DBusMessage* reply, string* value)
{
    int currentType;
    char* res;
    DBusMessageIter  rootIter;

    dbus_message_iter_init(reply, &rootIter);


    while( dbus_message_iter_get_arg_type(&rootIter) != DBUS_TYPE_INVALID)
    {
        currentType = dbus_message_iter_get_arg_type(&rootIter);
        if(currentType == DBUS_TYPE_STRING)
        {
            char *str = NULL;
            dbus_message_iter_get_basic(&rootIter, &str);
           res = strdup(str);

        }
        else if(currentType == DBUS_TYPE_VARIANT)
        {
            DBusMessageIter subIter;
            dbus_message_iter_recurse(&rootIter, &subIter);

            while(dbus_message_iter_get_arg_type(&subIter) != DBUS_TYPE_INVALID)
            {
                currentType = dbus_message_iter_get_arg_type(&subIter);
                if(currentType == DBUS_TYPE_STRING)
                {
                    char* str = NULL;
                    dbus_message_iter_get_basic(&subIter, &str);
                    res =  strdup(str);

                }

                dbus_message_iter_next(&subIter);
            }
        }

        dbus_message_iter_next(&rootIter);
    }

    value->assign(res, strlen(res));
    free(res);
    return TRUE;
}
