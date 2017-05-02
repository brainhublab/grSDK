#ifndef GR_DEVICE
#define GR_DEVICE

#include <iostream>
#include <deque>
#include <string>
#include <vector>
#include <map>

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
    
    std::map<std::string, imu*> imus;

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

       
       imus["pinky"] = &pinky;
       imus["ring"] = &ring;
       imus["middle"] = &middle;
       imus["index"] = &index;
       imus["thumb"] = &thumb;
       imus["palm"] = &palm;
    }
};

struct alg_device_t
{
    // q x y z
    // to store quaternions
    std::deque<std::vector<float>> pinky;
    std::deque<std::vector<float>> ring;
    std::deque<std::vector<float>> middle;
    std::deque<std::vector<float>> index;
    std::deque<std::vector<float>> thumb;
    std::deque<std::vector<float>> palm;
};



#endif
