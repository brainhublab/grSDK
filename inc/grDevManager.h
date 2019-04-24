#ifndef GR_DEV_MANAGER
#define GR_DEV_MANAGER

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
#include "grDataStructs.h"

#include <memory>
#include <stdexcept>
#include <array>

#include <regex>
#include <algorithm>

#include <fstream>
#include "exception.hpp"
#include "inetclientstream.hpp"
/**i
 * GRDevManager - a class description
 */
class GRDevManager
{
    public:
        /**
         * @brief constructor
         */
        GRDevManager();

        /**
         * @brief Destructor
         */
        ~GRDevManager();

        /**
         * @brief Copy constructor
         */
        GRDevManager(const GRDevManager&);

        /**
         * @brief Assignment operator
         */
        GRDevManager& operator=(const GRDevManager&);

        /**
         * @brief returns available devices in range
         * @return map of id's and devices which are avalible for connection
         * @see GRDevice
         */
        std::vector<GRDevice> getAvalibleDevices();

        /**
         * @brief activates device
         * add selected device from _avalibleDevices to _activeDevices and make precondition for connection
         * @param id is id of device
         * @return pointer to GRConnection object
         * @see getDeviceId()
         * @see GRConnection
         */
        //Connection*  setActiveDevice(int);

        /**
         * @brief returns id of device
         * a getter of id
         * @param device is GRDevice structure
         * @return id of device
         * @see GRDevice
         */
        //GRConnection* getActiveDeviceById(int);

        //    private:
        /**
         * @brief map of available devices
         */
        std::vector<GRDevice> _avalibleDevices;
        /**
         * @brief map of activated devices
         */
       // std::unordered_map<int, GRConnection> _activeDevices;
        /**
         * @brief checks if device is active
         */
        bool _deviceIsIn(std::string);

        std::string _exec(const char*);

        std::string _getApIface();

        std::unordered_map<std::string, std::string> _getApClients();

        bool _requestDevAttr(std::string);

       static  void funca(std::string );

};
#endif
