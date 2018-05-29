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

/**
 * gr devices names needed for easier findign of avalible devices
 */
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
/**
 * Innertial mesurment unit data structure
 */
struct imu
{
	/**
	 * @brief gyroscope data
	 */
    std::vector<double> gyro;
    /**
     * @brief accelerometer data
     */
    std::vector<double> acc;
    /**
     * @brief magnetometr data
     */
    std::vector<double> mag;

    /**
     * @brief timespamp
     */
    unsigned long time_stamp;
    /**
     * @brief boolean
     */
    bool is_connected;
    /**
     * @brief constructor
     */
    imu()
    {
        gyro.clear();
        acc.clear();
        mag.clear();
        time_stamp = 0.0;
        is_connected = false;
    }
    /**
     * @brief checks if imu has no data
     */
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
    /**
     * @brief checks if imu data is complite
     */
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
    /**
     * @brief deletes data
     */
    bool clear()
    {
        gyro.clear();
        acc.clear();
        mag.clear();
    }
};

/**
 * device message datastructure 
 */
struct gr_message
{
    /**
     * @brief message id 
     */
    int id;
    /**
     * @brief imus for each finger 
     */
    imu pinky;
    imu ring;
    imu middle;
    imu index;
    imu thumb;
    imu palm;

    /**
     * @brief imu map with pairs <"imu name", imu structure pointer> 
     */
    std::unordered_map<std::string, imu*> imus;

    /**
     * @brief constructor
     */
    gr_message()
    {
        id = 0;
        
        imus["pinky"] = &pinky;
        imus["ring"] = &ring;
        imus["middle"] = &middle;
        imus["index"] = &index;
        imus["thumb"] = &thumb;
        imus["palm"] = &palm;

        palm.is_connected = true;
    }
    /**
     * @brief cleaner
     */
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

    bool empty()
    {
        if(pinky.empty() || ring.empty() || middle.empty() || index.empty() || thumb.empty() ||
         (pinky.empty() && ring.empty() && middle.empty() && index.empty() && thumb.empty()) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

};

/**
 * GR device data structure
 */
struct device_t
{
    /**
     * @brief device id
     */
    int id;
    /**
     * @brief device name
     */
    std::string name;

    /**
     * @brief device MAC address
     */
    std::string address;

    /**
     * @brief constuctor
     */
    device_t()
    {
        id = 0;
    }
    
    device_t& operator=(const device_t& t)
    {
        this->id = t.id;
        this->name = t.name;
        this->address = t.address;

        return *this;
    }
    /**
     * @brief cleaner
     */
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
    /**
     * @brief quaternion for pinky
     */
    std::vector<double>  pinky;
    /**
     * @brief quaternion for ring
     */
    std::vector<double>  ring;
    /**
     * @brief quaternion for middle
     */
    std::vector<double>  middle;
    /**
     * @brief quaternion for index
     */
    std::vector<double>  index;
    /**
     * @brief quaternion for thumb
     */
    std::vector<double>  thumb;
    /**
     * @brief quaternion for palm
     */
    std::vector<double>  palm;

    /**
     * @brief map for iterating through quaternions
     */
    std::unordered_map<std::string, std::vector<double>*> nodes;

    /**
     * @brief constructor
     */
    gr_alg_message()
    {
        nodes["pinky"] = &pinky;
        nodes["ring"] = &ring;
        nodes["middle"] = &middle;
        nodes["index"] = &index;
        nodes["thumb"] = &thumb;
        nodes["palm"] = &palm;
    }

    /**
     * @brief cleaner
     */
    bool clear()
    {

        pinky.clear();
        ring.clear();
        middle.clear();
        index.clear();
        thumb.clear();
        palm.clear();

        return true;
    }

};

#endif
