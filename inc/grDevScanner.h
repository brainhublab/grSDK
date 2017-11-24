#ifndef GR_DEV_SCANNER
#define GR_DEV_SCANNER

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

#include <grConnection.h>
/**
 * GRDevScanner - a class description
 */
class GRDevScanner
{
    public:
        /**
         * @brief constructor
         * constructor descpription
         */
        GRDevScanner();
        /**
         * @brief Destructor
         */
        ~GRDevScanner();
        /**
         * @brief Copy constructor
         */
        GRDevScanner(const GRDevScanner&);
        /**
         * @brief Assignment operator
         */
        GRDevScanner& operator=(const GRDevScanner&); 
        /**
         * @brief returns available devices
         * @return map of id's and devices which are avalible for connection
         * @see device_t
         */ 
        std::unordered_map<int, device_t> getAvalibleDevices();
        /**
         * @brief activates device
         * add selected device to active devices and make precondition for connection
         * @param id is id of device
         * @return result of activating device
         * @see getDeviceId()
         */
        GRConnection*  setActiveDevice(int );
        /**
         * @brief returns id of device
         * a getter of id
         * @param device is device_t structure
         * @return id of device
         * @see device_t
         */
        GRConnection* getDeviceById(int);
    private:
        /**
         * @brief map of available devices
         */
        std::unordered_map<int, device_t> _avalibleDevices;
        /**
         * @brief map of activated devices
         */
        std::unordered_map<int, GRConnection> _activeDevices;
        /**
         * @brief checks if device is active
         */
        bool _deviceIsIn(std::string);

};

#endif
