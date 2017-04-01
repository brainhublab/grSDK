#ifndef GR_CONNECTION
#define GR_CONNECTION

#include<iostream>
#include<string>
#include <vector>
#include <deque>
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

    int getDeviceId(device_t);

    bool connect(std::string , std::string);
    bool readData(int, std::string);

    private:
    bool setUpRfcomm(std::string);
    bool setTerm();
    std::vector<device_t> avalibleDevices;
    std::vector<device_t> connectedDevices;



};

#endif
