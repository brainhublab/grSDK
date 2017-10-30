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


struct dev_socket //grDevice socket structure 
{
    int sock;
    struct sockaddr_rc addr = { 0 };

    sockaddr_rc* getAddrRef()
    {
        return &this->addr;
    }
};

/**
 * GRConnection - a class description
 */
class GRConnection
{
    public:
	    /**
	     * constructor descpription
	     */
        GRConnection();
        ~GRConnection();//destructor
        GRConnection(const GRConnection&);//copy constructor
        GRConnection& operator=(const GRConnection&);//operator = 
	/**
	 * return map of devices which are avalible for connection
	 */ 
        std::map<int, device_t> getAvalibleDevices();
 	/**
	 * add selected device to active devices and make precondition for connection
	 */
	bool setActiveDevice(int); 
        int getDeviceId(device_t);//returns Id of devise

        gr_message getMassage(int);//returns gr_message bi Id
        
        bool getData(int, gr_message*);// fill gr_message with message from device
      
        device_t* getDevice(int); //returns device by id
       
        bool connectSocket(int);//connect to socket 
    private:
        char _buf[256]; //buffer needet for reading from socket
        double _timeStamp;//timestamp local not from device
        std::chrono::time_point<std::chrono::system_clock> _start, _end;//timer
        std::map<int, device_t> _avalibleDevices;//map from avalible devices
        std::unordered_map<int, device_t> _activeDevices;//map from active devices
        std::unordered_map<int, dev_socket> _deviceSockets;//map with device sockets
        std::unordered_map<int, std::string> _bufferedData;//map with buffered data

        std::string _getNext();  //get nex message method
        bool _splitData(std::string, imu*);//splid raw data 
        double _getTimeStamp();//getting local timestamp
        bool _deviceIsIn(std::string); //chack if device is active devices
        int _asignDeviceWithSocket(int); // parameter is device ID
        device_t _getDeviceById(int);//getting device by dev Id
        int _getDeviceSocketById(int);// get device socket byd device id
        bool _asignMessageWithImu(std::string, gr_message*); //asigning of gr_message with concret imu
        bool _checkConnectedImus(std::string, gr_message*);    
        bool firstIteration;        
   // std::map<std::string, gr_message> messages;
};

#endif
