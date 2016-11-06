#include "gestusConnection.h"


GestusConnection::GestusConnection() //default constructor
{
    message = NULL;
    /*
    adapter.name.clear();//TODO fix get name function
    adapter.path = "/org/bluez/hci0";
    adapter.interfaces["adapter1"] = "org.bluez.Adapter1";
    adapter.interfaces["gattManager"] = "org.bluez.GattManager1";
    adapter.interfaces["introspectable"] = "org.freedesktop.DBus.Introspectable";
    adapter.interfaces["properties"] = "org.freedesktop.DBus.Properties";
*/
    


   

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

string GestusConnection::getAdapterName()//TODO
{/*
    DBusConnection *conn;
    DBusMessage *msg, *reply;

    DBusError* error;
    string adapterName;
    conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
    msg = dbus_message_new_method_call(
            adapter.bluezAttributes.find("name")->second.c_str(),
            adapter.adapterPath.c_str(),
            "org.freedesktop.DBus.Properties",
            "Get"
            );


    const char* interface = adapter.interfaces.find("adapter1") -> second.c_str();
    const char* property = "Name";
    dbus_message_append_args(msg,
            DBUS_TYPE_STRING, &interface,
            DBUS_TYPE_STRING, &property,
            DBUS_TYPE_INVALID);

    reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, NULL);
    
    
    adapterName.clear();
    getReply(reply, &adapterName);
    return adapterName;*/
}

bool GestusConnection::connectAndRead()
{
  // setAvalibleDevices(&devices);
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
            //need to complete with map
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
    
   // cout<<res;
    value->assign(res, strlen(res)+1);
    free(res);
    return TRUE;
}
bool GestusConnection::setAvalibleDevices()
{
    vector<string> allDevicesPaths;
    int pathArbitor = 0;
    int layerArbitor = 0;
   
    string path;
    string devName;

    DBusConnection *conn=NULL;
    DBusMessage  *msg=NULL, *reply=NULL;
    DBusError *derror=NULL;

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
    msg = dbus_message_new_method_call(
            "org.bluez",
            "/",
            "org.freedesktop.DBus.ObjectManager",
            "GetManagedObjects");
    reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, NULL);
    DBusMessageIter iterIn;
    dbus_message_iter_init(reply, &iterIn);
    iterDevices(&iterIn, &allDevicesPaths, pathArbitor, layerArbitor);
    
    const char* iface = "org.bluez.Device1";
    const char* prop = "Name";


    while(!allDevicesPaths.empty())
    {
        path = allDevicesPaths.back();
        msg = dbus_message_new_method_call(
               "org.bluez",
               path.c_str(),
               "org.freedesktop.DBus.Properties",
               "Get"
                );
        
               dbus_message_append_args(msg,
                DBUS_TYPE_STRING, &iface,
                DBUS_TYPE_STRING, &prop,
                DBUS_TYPE_INVALID);
        reply = dbus_connection_send_with_reply_and_block(conn, msg, -1, NULL);
        
        getReply(reply, &devName);
        cout<<devName<<endl;
        
        allDevicesPaths.pop_back();
        path.clear();
        

    }
cout<<"is OK" <<endl; 
    devName.clear();
cout<<"is OK" <<endl; 
 return true;
cout<<"is OK" <<endl; 

}

bool GestusConnection::setDeviceInfo(device_t* device, string devicePath)
{
    device->interfaces["device1"] = "org.bluez.Device1";   
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
                //if(pathArbitor == 2)
                //{
                    char* str = NULL;
                    dbus_message_iter_get_basic(iterIn, &str); 
                    string devicePath;
                    devicePath.assign(str, strlen(str)+1);
                    if(devicePath.length() == 38)
                    {
                        devices->push_back(devicePath);
                        //cout<<devicePath<<"----"<<devicePath.length()<<endl;
                    }
                    devicePath.clear();

                //}

            }

        }
        dbus_message_iter_next(iterIn);

    }
} 
