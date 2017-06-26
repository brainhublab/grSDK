#ifndef GR_CONNECTION
#define GR_CONNECTION

#include "grDevice.h"

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <unordered_map>
#include <sstream>
#include <thread>
#include <chrono>

#include <stdio.h>   /* Standard input/output definitions */
#include <stdlib.h>
#include <string>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */

#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>


struct dev_socket
{
    int sock;
    struct sockaddr_rc addr = { 0 };

    sockaddr_rc* getAddrRef()
    {
        return &this->addr;
    }
};


class GRConnection
{
    public:
        GRConnection();
        ~GRConnection();
        GRConnection(const GRConnection&);
        GRConnection& operator=(const GRConnection&);

        std::map<int, device_t> getAvalibleDevices();
        bool setActiveDevice(int);

        int getDeviceId(device_t);

        gr_message getMassage(int);
        
        bool getData(int, gr_message*);
      
        device_t* getDevice(int); 
       
        bool connectSocket(int);
    private:
        char buf[256];
        float timeStamp;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        std::map<int, device_t> avalibleDevices;
        std::unordered_map<int, device_t> activeDevices;
        std::unordered_map<int, dev_socket> deviceSockets;
        std::unordered_map<int, std::string> bufferedData;

        std::string getNext();  
        bool splitData(std::string, imu*);
        float getTimeStamp();
        bool deviceIsIn(std::string); 
        int asignDeviceWithSocket(int); // parameter is device ID
        device_t getDeviceById(int);
        int getDeviceSocketById(int);
        bool asignMessageWithImu(std::string, device_t*); 
        
   // std::map<std::string, gr_message> messages;
};

#endif
