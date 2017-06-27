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

struct dev_names
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
struct imu 
{
    std::vector<float> gyro;
    std::vector<float> acc;
    std::vector<float> mag;

    float time_stamp;

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
};


struct gr_message
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

};


struct device_t
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

struct gr_alg_message
{
    // q x y z
    // to store quaternions
    std::deque<std::vector<float> > pinky;
    std::deque<std::vector<float> > ring;
    std::deque<std::vector<float> > middle;
    std::deque<std::vector<float> > index;
    std::deque<std::vector<float> > thumb;
    std::deque<std::vector<float> > palm;
};

#endif
