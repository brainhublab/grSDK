#ifndef GR_DEVICE
#define GR_DEVICE



#include "grDataStructs.h"
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
#include <queue>
#include "inetclientstream.hpp"
#include "exception.hpp"


#include <regex>
class GRDevice
{
    public:
        /**
         * @brief default constructor
         **/
        GRDevice();

        GRDevice(std::string, std::string, std::string, int);
/*        GRDevice(const GRDevice&);
        GRDevice& operator=(const GRDevice&);

        ~GRDevice();
*/

        /**
         * @brief constructor with param
         * @param GRDevice structure for reference grDevice.h
         **/

        /**
         * @brief fills msg with data from device
         *  fills msg with data from device by id
         * @param id is device id geted from GRDevManager::_avalibleDevices and stored in GRDevManager::_activeDevices
         * @param msg is a pointer to gr_message object defined previosly
         * @return true if got data
         */
        void subscribe(GRMessage*,std::function<void(GRMessage*)>);
        int getBatteryLevel();        
        std::string getHwAddr() const;
        std::string getName() const;
        void setHwAddr(std::string);
        void setName(std::string);

 //  private:
        void _getData(GRMessage*, std::function<void(GRMessage*)>);
        /**
          void _getData(GRMessage*, std::function<void(GRMessage*)>);
         * @brief timestamp local, not from device
         */
        double _timeStamp;

        /**
         * @brief timer
         */
        std::chrono::time_point<std::chrono::system_clock> _start, _end;
        /**
         * @brief returns local timestamp
         */
        double _getTimeStamp();
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

        /**
         * @bref socket information assignet with device
         **/
        std::string _name;
        std::string _hwAddr;
        std::string _host;
        std::string _defaultPort;
        int id;
        uint16_t _batteryLevel;

        std::vector<std::string> _splitBySlash(std::string*);
        std::vector<std::string> _splitMessage(std::string*);
        void _deserialize(std::vector<std::string>, GRMessage*);
        void _extractImuDataFromString(std::string*, GRImu*);
        int16_t _convertBytes(char, char);

        std::unordered_map<std::string, std::string> _deviceCmd;
        bool _sendCmd(std::string);
        bool pendingCmd = false;
        std::queue<std::string> _cmdQueue;
        //friend GRDevManager::getAvalibleDevices();

        //GRDevice(const GRDevice &);
        // GRDevice& operator=(const GRDevice &);


};

#endif
