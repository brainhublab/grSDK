#ifndef GR_CONNECTION
#define GR_CONNECTION

#include<grDevice.h>

#include<iostream>
#include<string>
#include <vector>
#include <deque>
#include <map>
#include <sstream>
#include <thread>
//bluetooth libs

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> 
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
    bool getData(device_t*);

    private:
    char buf[256];

    std::string rfcommPath;
    bool setUpRfcomm(std::string);
    bool setTerm();
    std::string getNext();
    int openPort(std::string);

    bool connectAndRead(device_t*);

    bool splitData(std::string data, imu*);

    int portDescriptor;
    std::vector<device_t> avalibleDevices;



};

#endif
