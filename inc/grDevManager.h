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
#include <thread>
#include <condition_variable>
#include <typeinfo>

#include <csignal>

#include <gio/gio.h>

#include "tinyb.hpp"
//#include "grConnection.h"
/**
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
        std::unordered_map<int, GRDevice>* getAvalibleGRDevices();

        bool _deviceIsIn(GRDevice*);
        
        GRDevice* getGRDeviceById(int);

        bool getData(int);

        static void dataCallback(BluetoothGattCharacteristic &c, std::vector<unsigned char> &data, void *userdata);

        std::vector<int16_t> convertFromBytes(unsigned char*);

        std::vector<uint8_t> dataHolder; 

        int _grDevId = 0;

        bool _getAvalibleiGRDevices();

        bool _startDiscovery();
        bool _stopDiscovery();

        bool connect(int);

        void subscribe(int);

        tinyb::BluetoothManager* btManager;
        std::unordered_map<int, GRDevice> _avalibleGRDevices; 

        

    private:
};

#endif
