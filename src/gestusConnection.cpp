#include "gestusConnection.h"


GestusConnection::GestusConnection() //default constructor
{
    adapter_name = NULL;
    devices = NULL;
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

char* GestusConnection::getAdapterName()
{

}

char* GestusConnection::getAvalibleDevices()
{

}

bool GestusConnection::connectToDevice(char* name)
{
}

bool GestusConnection::recieveMessages(char* message)
{
}

bool GestusConnection::getReply(DBusMessage* reply) 
{
    int currentType;
    DBusMessageIter  rootIter;
    dbus_message_iter_init(reply, &rootIter);
    
    
    while( dbus_message_iter_get_arg_type(&rootIter) != DBUS_TYPE_INVALID)
    {
        currentType = dbus_message_iter_get_arg_type(&rootIter);
        if(currentType == DBUS_TYPE_STRING)
        {
            char *str = NULL;
            dbus_message_iter_get_basic(&rootIter, &str);
            cout<< "value:" << str << endl; 
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
                    cout << "value:" << str << endl;
                }

                dbus_message_iter_next(&subIter);
            }
        }

        dbus_message_iter_next(&rootIter);
    }
    return TRUE;
}

bool readConfig(char* filePath)
{
    
} 
