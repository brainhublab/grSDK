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
 /*
  * this method iterate and set the default adapter name it's like domain name
  */
bool GestusConnection::setAdapterName()
{
    DBusConnection *conn;
    DBusMessage *msg, *reply;

    DBusError* derror;
    std::string adapterName;

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
        std::cout<<"Can't find any adapters"<<std::endl;
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
/*
 * get the adapter name
 */
std::string GestusConnection::getAdapterName()
{
    return adapter.name;
}
/*
 * retunrs object adapter with all needed parameters
 */
device_t GestusConnection::getAdapter()
{
    return adapter;
}
/*
 * return the ID of connected device "dev" from parameter
 */
int GestusConnection::getDeviceId(device_t dev)
{
    return dev.id;
}
/*
 * return name of connected device "dev" from parameter
 */
std::string GestusConnection::getDeviceName(device_t dev)
{
    return dev.name;
}
/*
 * returns vectr with objects "device_type" which are now connected
 */
std::vector<device_t> GestusConnection::getConnectedDevices()
{
    return devices;
}
/*
 * fill the bufer parameter with data from device identified with "devID" and characteristic from parameters.
 * this method execute filling of buffer in new thread.
 */
bool GestusConnection::getData(int devId, std::string characteristic, std::deque<std::string>* buffer) //TODO kill the thread
{
    device_t dev;
    for(int i=0; i < devices.size(); i++) // iterate devices vector to find needed connected device by ID
    {
        if(devices[i].id == devId)
        {
            dev = devices[i];
            //std::cout<<dev.name;
        }
        else
        {
            std::cout<<"Can't find devices wit this: "<<devId<< "id"<<std::endl;
        }
    }
    //compare and select needed characteristic form parameter of method

    if(characteristic == "pinky_gyro")
    {
        characteristic = dev.fingers[0].gyro;
    }
    else if(characteristic == "pinky_acc")
    {
        characteristic = dev.fingers[0].accelerometer;
    }
    else if(characteristic == "pinky_mag")
    {
        characteristic = dev.fingers[0].mag;
    }
     if(characteristic == "ring_gyro")
    {
        characteristic = dev.fingers[1].gyro;
    }
    else if(characteristic == "ring_acc")
    {
        characteristic = dev.fingers[1].accelerometer;
    }
    else if(characteristic == "ring_mag")
    {
        characteristic = dev.fingers[1].mag;
    }
    if(characteristic == "middle_gyro")
    {
        characteristic = dev.fingers[2].gyro;
    }
    else if(characteristic == "middle_acc")
    {
        characteristic = dev.fingers[2].accelerometer;
    }
    else if(characteristic == "middle_mag")
    {
        characteristic = dev.fingers[2].mag;
    }
    if(characteristic == "index_gyro")
    {
        characteristic = dev.fingers[3].gyro;
    }
    else if(characteristic == "index_acc")
    {
        characteristic = dev.fingers[3].accelerometer;
    }
    else if(characteristic == "index_mag")
    {
        characteristic = dev.fingers[3].mag;
    }
    if(characteristic == "thumb_gyro")
    {
        characteristic = dev.fingers[4].gyro;
    }
    else if(characteristic == "thumb_acc")
    {
        characteristic = dev.fingers[4].accelerometer;
    }
    else if(characteristic == "thumb_mag")
    {
        characteristic = dev.fingers[4].mag;
    }
    else if(characteristic == "gyro")
    {
        characteristic = dev.gyro;
    }
    else if(characteristic == "acc")
    {
        characteristic = dev.accelerometer;
    }
    else if(characteristic == "mag")
    {
        characteristic = dev.magnet;
    }
   /* else
    {
        std::cout<<"Can't find this: "<<characteristic<<"characteristic"<<std::endl;
    }
*/

    std::thread thr(&GestusConnection::connectAndRead, this, dev, characteristic, buffer); // createnew thread
    thr.detach(); // detach thread from main proccess

   return TRUE;
}
/*
 *  iterate the DBus tree find needed device connect to them and read data from characteristic
 */
bool GestusConnection::connectAndRead(device_t dev, std::string characteristic, std::deque<std::string> *buffer)
{

    DBusConnection *conn=NULL;
    DBusMessage *msg=NULL, *reply=NULL;
    DBusError *derror=NULL;

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);

    std::string s;

    while(TRUE) //loop for reading of data from characteristic path TODO set the fucking timer
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
            std::cout<<"Can't read value from device "<<dev.name<<" with characteristic"<<characteristic<<std::endl;
            return FALSE;
        }
        dbus_message_iter_init(reply, &rootIter);
        int currentType;

        while(dbus_message_iter_get_arg_type(&rootIter) != DBUS_TYPE_INVALID) //iterate DBus devic tree and execue read method
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
/*
 * connect to defaut adapter dbusBluez and iterate the DBus tree for finding of connected devices
 * select GR devices from all and set needed parameters
 */
bool GestusConnection::setAvalibleDevices()
{
    std::vector<std::string> allDevicesPaths;
    int pathArbitor = 0;
    int layerArbitor = 0;

    std::string currentPath;
    std::string devName;
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

    iterDevices(&iterIn, &allDevicesPaths, pathArbitor, layerArbitor); // method needed for iteration of each device

    const char* iface = "org.bluez.Device1";
    const char* prop = "Name";

    while(!allDevicesPaths.empty()) // iterate all devices and select GR only
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

        if(devName == deviceNames.left || devName == deviceNames.right) // select only GR devices and set parameters
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
            std::cout<<"Not found any Gestus Devices :("<<std::endl;
            return FALSE;
        }*/

        allDevicesPaths.pop_back();
        currentPath.clear();
        devId++;

    }
    devName.clear();
    //std::cout<<devices.front().fingers<<std::endl;
    return TRUE;

}
/*
 * iterate the device tree and set characteristics to device in perameter of method
 * the characteristics are UUID's which are setted in GR for each type of data
 */
bool GestusConnection::setDeviceCharacteristics(device_t* device, std::string devicePath)
{
    std::vector<std::string> gattServices;
    DBusMessage *reply = NULL;

    reply = getProperty(devicePath.c_str(), "org.bluez.Device1", "GattServices");
    if(parsePropertyArray(reply, &gattServices))
    {
        for(int i = 0; i < gattServices.size(); i++)
        {
            //std::cout<<gattServices[i]<<std::endl;

            std::vector<std::string> characteristics;

            // TODO: free reply
            reply = NULL;
            reply = getProperty(gattServices[i].c_str(), "org.bluez.GattService1", "Characteristics");
            if(parsePropertyArray(reply, &characteristics))
            {
                for(int j = 0; j < characteristics.size(); j++)
                {
                    //std::cout<<characteristics[j]<<std::endl;
                    std::string currentUUID;
                    reply = NULL;
                    reply = getProperty(characteristics[j].c_str(), "org.bluez.GattCharacteristic1", "UUID");

                    if(parsePropertyString(reply, &currentUUID))// iterate and parse needed characteristic UUID's from uuids struct
                    {
                        //std::cout<<currentUUID<<std::endl;
                        for(int k=0; k<6; k++)
                        {
                            if(currentUUID == uuids.fingers[k].gyro)
                            {
                                device->fingers[k].gyro = characteristics[j];
                            }
                            else if(currentUUID == uuids.fingers[k].accelerometer)
                            {
                                device->fingers[k].accelerometer = characteristics[j];
                            }
                            else if(currentUUID == uuids.fingers[k].mag)
                            {
                                device->fingers[k].mag = characteristics[j];
                            }
                        }
                       if(currentUUID == uuids.gyro)
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
                           std::cout<<"Cant find any characteristic please check yhe device configs"<<std::endl;
                           return FALSE;
                       }*/
                    }
                    else
                    {
                        std::cout<<"Can't parse property string check configs"<<std::endl;
                        return FALSE;
                    }
                }
            }
            else
            {
                std::cout<<"Cant parse parsePropertyArray check configs"<<std::endl;
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
/*
 * returns the DBus message with properties of device by object path in DBus tree
 * returns property of selected characteristic.
 */
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
/*
 * internal auxiliary method
 * parse property array by rely in ARRAY type
 * if property is array not a basic type
 */
bool GestusConnection::parsePropertyArray(DBusMessage *reply, std::vector<std::string> *res)
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

                    res->push_back(std::string(str));
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
/*
 * internal auxiliary method
 * parse property array by rely in STRING type
 * if it's basic type
 */
bool GestusConnection::parsePropertyString(DBusMessage *reply, std::string *res)
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
/*
 * internal auxiliary method
 * iterates all connected devices and store all paths of connected devises in "devices" vector
 */
bool GestusConnection::iterDevices(DBusMessageIter* iterIn, std::vector<std::string>* devices, int pathArbitor, int layerArbitor)
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
                    std::string devicePath;
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
/*
 * internal auxility method
 * store the reply data in "value" parameter if it is not a string is iterates reply deeper
 */
bool GestusConnection::getReply(DBusMessage* reply, std::string* value)
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
