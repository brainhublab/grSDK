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
#include <memory>

//#include "grConnection.h"
/**
 * gr devices names needed for easier findign of avalible devices
 */
struct GRDevNames
{
    std::string left = "GR[L]";
    std::string right = "GR[R]";
    std::string test = "asd";

};

struct GRImu
{
    /**
     * @brief timespamp
     */
    unsigned long timeStamp;

    /**
     * @brief boolean
     */
    bool isConnected;

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
     * @brief constructor
     */
    GRImu()
    {
        this->gyro.clear();
        this->acc.clear();
        this->mag.clear();
        this->timeStamp = 0.0f;
        this->isConnected = false;


    }

    /**
     * @brief checks if imu has no data
     */
    bool empty()
    {
        return (this->gyro.empty() && this->acc.empty() && this->mag.empty()) ||
            this->gyro.empty() || this->acc.empty() || this->mag.empty();
    }
    /**
     * @brief checks if imu data is complite
     */


    bool isComplete()
    {
        return (this->gyro.size() == 3) && (this->acc.size() == 3) && (this->mag.size() == 3);
    }

    /**
     * @brief deletes data
     */

    bool clear()
    {
        this->gyro.clear();
        this->acc.clear();
        this->mag.clear();
        this->timeStamp = 0.0f;
    }


};

struct GRMessage
{
    int id;
    GRImu pinky;
    GRImu ring;
    GRImu middle;
    GRImu index;
    GRImu thumb;
    GRImu palm;

    std::unordered_map<std::string, GRImu*> imus;

    GRMessage()
    {
        this->id = 0;
        this->imus["pinky"] = &(this->pinky);
        this->imus["ring"] = &(this->ring);
        this->imus["middle"] = &(this->middle);
        this->imus["index"] = &(this->index);
        this->imus["thumb"] = &(this->thumb);
        this->imus["palm"] = &(this->palm);

        this->palm.isConnected = true;
    }

    bool clear()
    {
        this->pinky.clear();
        this->ring.clear();
        this->middle.clear();
        this->index.clear();
        this->thumb.clear();
        this->palm.clear();

        return true;
    }

    int get_id()
    {
        return this->id;
    }
    void set_id(int nId)
    {
        this->id = nId;
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

    std::unordered_map<std::string, GRImu*> get_imus()
    {
        return this->imus;
    }
};


struct GRAlgMessage
{
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
    GRAlgMessage()
    {
        this->nodes["pinky"] = &(this->pinky);
        this->nodes["ring"] = &(this->ring);
        this->nodes["middle"] = &(this->middle);
        this->nodes["index"] = &(this->index);
        this->nodes["thumb"] = &(this->thumb);
        this->nodes["palm"] = &(this->palm);
    }

    /**
     * @brief cleaner
     */
    bool clear()
    {
        this->pinky.clear();
        this->ring.clear();
        this->middle.clear();
        this->index.clear();
        this->thumb.clear();
        this->palm.clear();

        return true;
    }
};

#endif
