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
 * @brief sgrDevice socket structure
 *  contains socket params
 */
struct dev_socket
{
    /**
     * @brief socket descriptor
     **/
    int sock;

    /**
     * @brief addres for data recieving
     **/
    struct sockaddr_rc addr = { 0 };

    /**
     * @brief getter
     * @return reference to sockaddr_rc structure of socket
     */
    sockaddr_rc* getAddrRef()
    {
        return &this->addr;
    }
};


class GRConnection
{
    public:
        /**
         * @brief default constructor
         **/
        GRConnection();

        /**
         * @brief constructor with param
         * @param GRDevice structure for reference grDevice.h
         **/
        GRConnection(GRDevice);

        /**
         * @brief fills msg with data from device
         *  fills msg with data from device by id
         * @param id is device id geted from GRDevManager::_avalibleDevices and stored in GRDevManager::_activeDevices
         * @param msg is a pointer to gr_message object defined previosly
         * @return true if got data
         */
        bool getData(GRMessage* msg);

        /**
         * @brief gets device by id
         *   getter
         * @param id is device id
         * @return pointer to GRDevice object by device id
         */
        GRDevice* getDevice(int id);

        /**
         * @brief connects socket for selected device and store params of created socket in _deviceSocket
         * @return true if socket is assigned succssesfuly
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
         * @brief splits raw data string for imu vars imu.gyro imu.acc imu.mag imu.time_stamp
         * @return true if writing in imu* is succsessd
         * @see imu
         */
        bool _splitData(std::string, GRImu*);

        /**
         * @brief returns local timestamp
         */
        double _getTimeStamp();

        /**
         * @brief assigns new socket to device with device id
         * @return socket stored in _deviceSocket.sock
         */
        int _asignDeviceWithSocket();

        /**
         * @brief return device by device id
         * @return GRDevice structure with stored config data
         */
        GRDevice _getDeviceById(int id);

        /**
         * @brief assigns all imu vars in gr_message  pointer with concret imu data
         *  call GRConnection::_splitData for each imu
         * @param std::string input raw message
         * @param gr_message* pointer to message structure for output
         *
         */
        bool _asignMessageWithImu(std::string, GRMessage*);

        /**
         * @bref check which finger modules are connected and send data
         * write boolean imu.is_connected flags in gr_message parameter for each imu
         * @param std::string raw message as string
         * @param gr_message* poiter to message struct as output
         * return true if finish ok
         **/
        bool _checkConnectedImus(std::string, GRMessage*);

        /**
         * @brief Boolean for identifying first iteration of geting data
         */
        bool firstIteration;

        /**
         * @bref GRDevice class member for storing device params
         **/
        GRDevice _dev;

        /**
         * @bref socket information assignet with device
         **/
        dev_socket _deviceSocket;

};

#endif
