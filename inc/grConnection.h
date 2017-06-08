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

#include <stdio.h>   /* Standard input/output definitions */
#include <stdlib.h>
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h>


#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

class GRConnection
{
    public:
        GRConnection();
        ~GRConnection();
        GRConnection(const GRConnection&);
        GRConnection& operator=(const GRConnection&);

        bool findDevices();
        std::vector<device_t> getAvalibleDevices();
        int getDeviceId(device_t);

        bool connect(std::string, std::string, std::string);
        bool release(std::string, std::string, std::string);

        bool getDataThr(device_t*);
        bool getData(device_t*);

    private:
        char buf[256];
        int portDescriptor;
        std::string rfcommPath;
        std::vector<device_t> avalibleDevices;
        bool setUpRfcomm(std::string);
        bool setTerm();
        std::string getNext();
        int openPort(std::string);
        bool splitData(std::string data, imu*);
};

#endif
