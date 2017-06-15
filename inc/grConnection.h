#ifndef GR_CONNECTION
#define GR_CONNECTION

#include "grDevice.h"

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
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
        std::vector<GRConnection> getAllMessages;
        

        //bool connect(std::string, std::string, std::string);
        // bool release(std::string, std::string, std::string);
        
        bool getDataThr(device_t*);
        bool getData(device_t*);


    private:
        char buf[256];
        float timeStamp;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        std::map<int, device_t> avalibleDevices;
        std::map<int, int> deviceSockets;
        std::map<int, device_t> activeDevices;

        std::string getNext();  
        bool splitData(std::string, imu*);
        float getTimeStamp();
        bool deviceIsIn(std::string); 
        int asignDeviceWithSocket(int); // parameter is device ID
        device_t getDeviceById(int);
        

};

#endif
