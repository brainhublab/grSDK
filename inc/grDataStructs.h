#ifndef GR_DATA_STRUCTS
#define GR_DATA_STRUCTS

#include <iostream>
#include <deque>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <stdlib.h>
#include <memory>
#include <array>
#include <sstream>
#include <algorithm>
#include <iterator>
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
    std::array<int16_t, 3> gyro;

    /**
     * @brief accelerometer data
     */
    std::array<int16_t, 3> acc;

    /**
     * @brief magnetometr data
     */
    std::array<int16_t, 3> mag;

    /**
     * @brief constructor
     */
    std::string imuByteData;
    GRImu()
    {
        this->gyro.fill(0);
        this->acc.fill(0);
        this->mag.fill(0);
        this->timeStamp = 0.0f;
        this->isConnected = true;
        this->imuByteData.resize(117);
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
        this->gyro.fill(0);
        this->acc.fill(0);
        this->mag.fill(0);
        this->timeStamp = 0.0f;
    }
    void print()
    {
        if(!this->empty())
        {
            std::cout<<"acc ";
            for(const auto& val : acc)
            {
                std::cout<<val<<" - ";
            }

            std::cout<<"|gyro  ";
            for(const auto& val : gyro)
            {
                std::cout<<val<<" - ";
            }

            std::cout<<"|mag ";
            for(const auto& val : mag)
            {
                std::cout<<val<<" - ";
            }
            std::cout<<std::endl;
        }

    }


    std::string getAsStr()
    {
        std::stringstream res;
        std::copy(acc.begin(), acc.end(), 
                std::ostream_iterator<int16_t>(res, ","));
        res<<" ";
        std::copy(gyro.begin(), gyro.end(),
                std::ostream_iterator<int16_t>(res, ","));
        res<<" ";
        std::copy(mag.begin(), mag.end(),
                std::ostream_iterator<int16_t>(res, ","));
        return res.str();
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

    uint16_t batteryLevel;
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
        this->batteryLevel = 0;
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

    void print()
    {
        for(const auto& imu: imus)
        {
            std::cout<<imu.first<<" ";
            imu.second->print();
            std::cout<<std::endl;
        }
    }

    std::string getAsStr()
    {
        return this->pinky.getAsStr() + "," + 
            this->ring.getAsStr() + "," +
            this->middle.getAsStr() + "," +
            this->index.getAsStr() + "," +
            this->thumb.getAsStr() + "," +
            this->palm.getAsStr(); 
    }
    std::string getImuAsStr(std::string *imuKey)
    {
        if(!imus.count(*imuKey))
        {
            return "ERRO: There is no imu";
        }
        return imus[*imuKey]->getAsStr();       
    }

    // std::map<std::string, GRImu*> get_imus()
    // {
    //     return this->imus;
    // }
};


struct GRAlgMessage
{
    /**
     * @brief quaternion for pinky
     */
    std::array<double, 4>  pinky;
    /**
     * @brief quaternion for ring
     */
    std::array<double, 4>  ring;
    /**
     * @brief quaternion for middle
     */
    std::array<double, 4>  middle;
    /**
     * @brief quaternion for index
     */
    std::array<double, 4>  index;
    /**
     * @brief quaternion for thumb
     */
    std::array<double, 4>  thumb;
    /**
     * @brief quaternion for palm
     */
    std::array<double, 4>  palm;
    /**
     * @brief map for iterating through quaternions
     */
    std::unordered_map<std::string, std::array<double, 4>*> nodes;

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
        this->pinky.fill(0.0F);
        this->ring.fill(0.0f);
        this->middle.fill(0.0f);
        this->index.fill(0.0f);
        this->thumb.fill(0.0f);
        this->palm.fill(0.0f);

        return true;
    }
};

struct GRDevAttr
{
    std::string name;
    std::string id;
};

#endif
