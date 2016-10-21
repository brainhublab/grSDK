#ifndef GESTUS_CONNECTION
#define GESTUS_CONNECTION

class GestusConnection
{
    public:
        GestusConnection();
        ~GestusConnection();
        GestusConnection(const GestusConnection& connection);
        GestusConnection& operator=(const GestusConnection& connection);

        char* getAdapterName(); //gets the pc bluetooth adapter name
        char* getAvalibleDevices(); //gets the avalible for connection gestus devices

        bool connectToDevice(char*); //make a connection with device by name

        bool recieveMessages(char* ) // method for recieving messages from gestus device the parameter is DBus uuid of service

    private:
        char* adapter_name; //name of adapter in pc
        char** devices; //names gesture input devices
        
        char* message;

}


#endif
