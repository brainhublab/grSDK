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


class GRConnection
{
    public:
        GRConnection();
        GRConnection(device_t);
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
        bool getData(gr_message* msg);
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
        bool connectSocket();
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
         * @brief assigns new socket to device with id device
         * @param id is device id
         * @return socket
         */
        int _asignDeviceWithSocket();
        /**
         * @brief return device by device id
         */
        device_t _getDeviceById(int id);
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
        device_t _dev;
        dev_socket _deviceSocket;

};

#endif
