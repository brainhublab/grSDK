#ifndef GR_DEVICE
#define GR_DEVICE

#include <iostream>
#include <deque>
#include <string>
#include <cstring>
#include <vector>
#include <map>

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
struct gr_message
{
    int dev_id;

    std::vector<float> gyro;
    std::vector<float> acc;
    std::vector<float> mag;

    float timestamp;

    gr_message()
    {
        dev_id = 0.0;
           
        std::memset(&gyro[0], 0, sizeof(gyro));
        std::memset(&acc[0], 0, sizeof(acc));
        std::memset(&mag[0], 0, sizeof(mag));

        timestamp = 0.0;
    }
};


/*
struct imu
{
   int id; 
    std::deque<std::vector<float> > gyro;
    std::deque<std::vector<float> > acc;
    std::deque<std::vector<float> > mag;
};
*/
struct imu
{
    int id;
    std::deque<gr_message> data;
};


struct device_t
{
    int id;
    std::string name;
    
    std::string addr;

    std::map<std::string, imu*> imus;
/*
    imu pinky;
    imu ring;
    imu middle;
    imu index;
    imu thumb;
    imu palm;
  */  //
    imu pinky;
    imu ring;
    imu middle;
    imu index;
    imu thumb;
    imu palm;

    device_t()
    {
       id = 0;

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

    void clear_attributes()
    {
           id = 0;
            name.clear();
            addr.clear(); 
      }
};

struct alg_device_t
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
