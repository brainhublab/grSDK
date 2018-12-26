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

#include <gio/gio.h>

/**
 * gr devices names needed for easier findign of avalible devices
 */
const std::string bluezBus = "org.bluez";
const std::string defaultAdapterPath = "/org/bluez/hci";
const std::string deviceIface = "org.bluez.Device1";
const std::string propIface = "org.freedesktop.DBus.Properties";
const std::string gattCharIface = "org.bluez.GattCharacteristic1";
const std::string dataGattCharPath = "/service001f/char0020";

const std::string lName = "GR[L]";
const std::string rName = "GR[R]";

struct GRImu
{
    /**
     * @brief timespamp
     */
    unsigned long time_stamp;

    /**
     * @brief boolean
     */
    bool is_connected;

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
        this->time_stamp = 0.0;
        this->is_connected = false;
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
    bool is_complete()
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
        this->time_stamp = 0.0;
        this->is_connected = false;
        return true;
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

        this->palm.is_connected = true;
    }
    bool clear()
    {
        this->pinky.clear();
        this->ring.clear();
        this->middle.clear();
        this->index.clear();
        this->thumb.clear();
        this->palm.clear();
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

struct GRDevice
{
    int id = 0;
    std::string name = "";
    std::string address = "";
    std::string dbusObjecPath = "";

    bool connected = false;

    GDBusProxy *propProxy;
    GDBusProxy *devMethodProxy;
    GDBusProxy *gattCharProxy;
    GDBusProxy *gattPropProxy;


    GRDevice()
    {
        this->id = 0;

    }
    GRDevice& operator=(const GRDevice& dev)
    {
        this->id = dev.id;
        this->name = dev.name;
        this->address = dev.address;

        return *this;
    }

    void clear_attributes()
    {
        this->id = 0;
        this->name.clear();
        this->address.clear();
    }
    void clear()
    {
        this->id = 0;
        this->name.clear();
        this->address.clear();
        this->dbusObjecPath.clear();

        this->connected = false;

        //TODO clear proxies;
        //g_object_unref(propProxy);
        //g_object_unref(methodProxy);
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



    std::vector<double>* get_node(std::string key)
    {
        return this->nodes[key];

    }
};

#endif
