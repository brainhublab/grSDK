#ifndef GR_CONNECTION
#define GR_CONNECTION

#include<iostream>
#include<string>
#include <vector>
#include <deque>
#include <map>
#include <sstream>
//bluetooth libs
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> 
struct dev_names
{
    std::string left;
    std::string right;
    std::string test;

    dev_names()
    {
        left = "GR[L]";
        right = "GR[R]";
        test = "hc-06";    
    }
};
struct imu
{
    int id; 
    std::deque<std::vector<float> > gyro;
    std::deque<std::vector<float> > acc;
    std::deque<std::vector<float> > mag;
};

struct device_t
{
    int id;
    std::string name;

    imu pinky;
    imu ring;
    imu middle;
    imu index;
    imu thumb;
    imu palm;
    device_t()
    {
        pinky.id = 0;
        ring.id = 1;
        middle.id = 2;
        index.id = 3;
        thumb.id = 4;
        palm.id = 5;

    }
};
class GRConnection
{
    public:
    GRConnection();
    ~GRConnection();
    GRConnection(const GRConnection&);
    GRConnection& operator=(const GRConnection&);

    bool findDevices();
    std::vector<device_t> getAvalibleDevices();
    std::vector<device_t> getConnectedDevices();

    int getDeviceId(device_t);

    bool connect(std::string , std::string);
    bool readData();

    private:
    char buf[256];

    std::string rfcommPath;
    bool setUpRfcomm(std::string);
    bool setTerm();
    std::string getNext();
    int openPort(std::string);

    bool splitData(std::string data, imu*);

    int portDescriptor;
    std::vector<device_t> avalibleDevices;
    device_t device;



};

#endif
