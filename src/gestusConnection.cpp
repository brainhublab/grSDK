#include "gestusConnection.h"


GestusConnection::GestusConnection() //default constructor
{
    message = NULL;
   

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

string GestusConnection::getAdapterName()
{
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
    return adapterName;
}

string GestusConnection::getAvalibleDevices()
{
    DBusConnection *conn1=NULL;
    DBusMessage  *msg1=NULL, *reply1=NULL;
    DBusError *error1=NULL;
    string success1 = "succsess";

    conn1 = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
    msg1 = dbus_message_new_method_call(
            "org.bluez",
            "/",
            "org.freedesktop.DBus.ObjectManager",
            "GetManagedObjects");

    //dbus_message_append_args(msg1,
      //      DBUS_TYPE_INVALID);

    reply1 = dbus_connection_send_with_reply_and_block(conn1, msg1, 5, NULL);
    

    findAllDevices(reply1, &connectedDevices);
    //dbus_message_unref(msg1);
    //dbus_message_unref(reply1);
    if(connectedDevices.empty())
    {
        cout<<"is emplty"<<endl;
    }
    return success1;
    // success.clear();   

}

bool GestusConnection::connectAndRead()
{
    return true;
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
            strncpy(res, str, strlen(str)+1);
            
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
                    strncpy(res, str, strlen(str)+1);

                }

                dbus_message_iter_next(&subIter);
            }
        }

        dbus_message_iter_next(&rootIter);
    }
    
   // cout<<res;
    value->assign(res, strlen(res)+1);
    return TRUE;
}
bool GestusConnection::findAllDevices(DBusMessage* reply, map<string, map<string, map<string, vector<string> > > > *connectedDevices)
{
    int arbitor = 0;
    
    DBusMessageIter iterIn, iterOut;
    dbus_message_iter_init(reply, &iterIn);
    iterAllDevices(&iterIn, &iterOut, &arbitor, 0);
    //char* str = NULL;
   // dbus_message_iter_get_basic(&iterOut, &str);
    //cout<<"YAH"<<str<<endl;
    cout<<arbitor<<endl;


}

bool GestusConnection::iterAllDevices(DBusMessageIter* iterIn, DBusMessageIter* iterOut, int* pathArbitor, int layerArbitor)
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
          
            iterAllDevices(&subIterIn, iterOut, pathArbitor, next);
            
        }
        else if(currentType == DBUS_TYPE_STRING || 
                currentType == DBUS_TYPE_OBJECT_PATH)
        {
            
            if(currentType == DBUS_TYPE_OBJECT_PATH && layerArbitor == 2)
            {
                (*pathArbitor) += 1;
                if((*pathArbitor) == 2)
                {
                    char* str = NULL;
                    dbus_message_iter_get_basic(iterIn, &str);
                    cout<<str<<endl;
                }

            }
            iterOut = iterIn;
            char* str = NULL;
            dbus_message_iter_get_basic(iterOut, &str);
            //cout<<"VALUE!!"<<str<<endl;
            
            
        }
        dbus_message_iter_next(iterIn);

    }

}
bool GestusConnection::setVariant(DBusMessageIter* iter, vector<string>* value)
{
    DBusMessageIter subIter;
    dbus_message_iter_recurse(iter, &subIter);
    while(dbus_message_iter_get_arg_type(sub))
}
