#ifndef GR_DEVICE
#define GR_DEVICE

#include <iostream>
#include <deque>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <stdlib.h>

struct dev_names//gr devices names needed for easier findign of avalible devices
{
    std::string left;
    std::string right;
    std::string test;

    dev_names()
    {
        left = "GR[L]";
        right = "GR[R]";
        test = "HC-06";    
    }
};
struct imu //Innertial mesurment unit data structure
{
    std::vector<double> gyro;
    std::vector<double> acc;
    std::vector<double> mag;

    unsigned long time_stamp;

    imu()
    {
        gyro.clear();
        acc.clear();
        mag.clear();
        time_stamp = 0.0;
    }
    bool empty()
    {
        if((gyro.empty() && acc.empty() && mag.empty()) ||
                gyro.empty() || acc.empty() || mag.empty())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    bool is_complite()
    {
        if((gyro.size() == 3) && (acc.size() == 3) && (mag.size() == 3))
        {
            return true;
        }
        else 
        {
            return false;
        }
    }

    bool clear()
    {
        gyro.clear();
        acc.clear();
        mag.clear();
    }
};


struct gr_message//device message datastructure 
{
    int id;

    imu pinky;
    imu ring;
    imu middle;
    imu index;
    imu thumb;
    imu palm;

    std::unordered_map<std::string, imu*> imus;

    gr_message()
    {
        id = 0;
        
        imus["pinky"] = &pinky;
        imus["ring"] = &ring;
        imus["middle"] = &middle;
        imus["index"] = &index;
        imus["thumb"] = &thumb;
        imus["palm"] = &palm;
    }
    bool clear()
    {
        pinky.clear();
        ring.clear();
        middle.clear();
        index.clear();
        thumb.clear();
        palm.clear();
        
        return 1;
    }

};


struct device_t//g device data structure
{
    int id;
    std::string name;

    std::string address;

    device_t()
    {
        id = 0;
    }

    void clear_attributes()
    {
        id = 0;
        name.clear();
        address.clear(); 
    }
};

struct gr_alg_message//gr message for algorithms
{
    // q x y z
    // to store quaternions
    std::vector<double>  pinky;
    std::vector<double>  ring;
    std::vector<double>  middle;
    std::vector<double>  index;
    std::vector<double>  thumb;
    std::vector<double>  palm;

    std::unordered_map<std::string, std::vector<double>*> nodes;

    gr_alg_message()
    {
        nodes["pinky"] = &pinky;
        nodes["ring"] = &ring;
        nodes["middle"] = &middle;
        nodes["index"] = &index;
        nodes["thumb"] = &thumb;
        nodes["palm"] = &palm;
    }

};

#endif
