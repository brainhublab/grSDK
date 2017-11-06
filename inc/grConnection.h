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

/**
 * grDevice socket structure
 */
struct dev_socket 
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
	     * @brief constructor
	     * constructor descpription
	     */
        GRConnection();
        /**
	 * @brief Destructor
	 */
	~GRConnection();
	/**
	 * @brief Copy constructor
	 */
        GRConnection(const GRConnection&);
	/**
	 * @brief Assignment operator
	 */
        GRConnection& operator=(const GRConnection&); 
	/**
	 * @brief returns available devices
	 * @return map of id's and devices which are avalible for connection
	 * @see device_t
	 */ 
        std::map<int, device_t> getAvalibleDevices();
 	/**
	 * @brief activates device
	 * add selected device to active devices and make precondition for connection
	 * @param id is id of device
	 * @return result of activating device
	 * @see getDeviceId()
	 */
	bool setActiveDevice(int id);
        /**
	 * @brief returns id of device
	 * a getter of id
	 * @param device is device_t structure
	 * @return id of device
	 * @see device_t
	 */
        int getDeviceId(device_t);
	/**
	 * @brief getter
	 * @param id is device id
	 * @return gr_message from id
	 * @see gr_message
	 */
        gr_message getMassage(int id);
        /**
	 * @brief fills msg with data from device
	 * fills msg with data from device with id
	 * @param id is device id
	 * @param msg is a pointer to message object
	 * @return true if got data
	 */
        bool getData(int id, gr_message* msg);
	/**
	 * @brief gets device by id
	 * getter
	 * @param id is device id
	 * @return pointer to device object by device id
	 */
        device_t* getDevice(int id);
	/**
	 * @brief connects socket for device with identificator id
	 * Connects socket for device with identificator id
	 * @return the result of connection
	 */
        bool connectSocket(int id);
    private:
	/**
	 * @brief Buffer needed for reading from socket
	 */
        char _buf[256];
       	/**
	 * @brief timestamp local, not from device
	 */
        double _timeStamp;
	/**
	 * @brief timer
	 */
        std::chrono::time_point<std::chrono::system_clock> _start, _end;
	/**
	 * @brief map of available devices
	 */
        std::map<int, device_t> _avalibleDevices;
	/**
	 * @brief map of activated devices
	 */
        std::unordered_map<int, device_t> _activeDevices;
	/**
	 * @brief map of device sockets
	 */
        std::unordered_map<int, dev_socket> _deviceSockets;//map with device sockets
        /**
	 * @brief map of buffered data
	 */
	std::unordered_map<int, std::string> _bufferedData;
	/**
	 * @brief getter of next message
	 * @return next message
	 */
        std::string _getNext();
	/**
	 * @brief splits raw data into imu structure
	 * @return result of operation
	 * @see imu
	 */
        bool _splitData(std::string, imu*);
        /**
	 * @brief returns local timestamp
	 */	
        double _getTimeStamp();
	/**
	 * @brief checks if device is active
	 */
        bool _deviceIsIn(std::string);
	/**
	 * @brief assigns new socket to device with id device
	 * @param id is device id
	 * @return socket
	 */
        int _asignDeviceWithSocket(int id);
	/**
	 * @brief return device by device id
	 */
        device_t _getDeviceById(int id);
	/**
	 * @brief returns socket by device id
	 */
        int _getDeviceSocketById(int id);
	/**
	 * @brief assigns gr_message pointer with concret imu
	 * TODO
	 */
        bool _asignMessageWithImu(std::string, gr_message*);
        bool _checkConnectedImus(std::string, gr_message*);    
        /**
	 * @brief Boolean for identifying first iteration
	 */
	bool firstIteration;

   // std::map<std::string, gr_message> messages;
};

#endif
