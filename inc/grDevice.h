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

#include <functional>

#include "tinyb.hpp"
#include <gio/gio.h>

/**
 * gr devices names needed for easier findign of avalible devices
 */
/*
   const std::string bluezBus = "org.bluez";
   const std::string defaultAdapterPath = "/org/bluez/hci";
   const std::string deviceIface = "org.bluez.Device1";
   const std::string propIface = "org.freedesktop.DBus.Properties";
   const std::string gattCharIface = "org.bluez.GattCharacteristic1";
//const std::string dataGattCharPath = "/service001f/char0020";
const std::string dataGattCharPath = "/service002b/char002c";
*/
const std::string lName = "GR[L]";
const std::string rName = "GR[R]";

//const std::string imuServiceUUID ="6e400001-b5a3-f393-e0a9-e50e24dcca9e";// "fced6408-c015-45ea-b50d-1de32a1c2f6d"; //TODO later change to const
//const std::string imuGattCharUUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";// "fced6409-c015-45ea-b50d-1de32a1c2f6d";

const std::string imuServiceUUID = "fced6408-c015-45ea-b50d-1de32a1c2f6d"; //TODO later change to const
const std::string imuGattCharUUID = "fced6409-c015-45ea-b50d-1de32a1c2f6d";
const std::string transmissionServiceUUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
const std::string transmissionCharUUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";
const std::string battServiceUUID = "0000180f-0000-1000-8000-00805f9b34fb";
const std::string battCharUUID = "00002a1b-0000-1000-8000-00805f9b34fb";

const std::vector<unsigned char> start_transmission {0x01};
const std::vector<unsigned char> end_transmission {0x04};
const std::vector<unsigned char> check_bat {0x06};

static std::condition_variable conditionVar;
/*
   static void data_callback(BluetoothGattCharacteristic &c, std::vector<unsigned char> &data, void *userdata)
   {
   unsigned char* data_c;
   unsigned int dataSize = data.size();

   if(dataSize >0)
   {
   data_c = data.data();

   std::cout<<" Raw data [";
   for(unsigned i=0;i<dataSize; i++)
   {
   std::cout<<std::hex<<static_cast<int>(data_c[i])<<" - ";

   }
   std::cout<<"]"<<std::endl;
   }
   }
   */

static void signal_handler(int sigNum)
{
    if(sigNum == SIGINT)
    {

        conditionVar.notify_all();
    }
}



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
    std::vector<int16_t> gyro;

    /**
     * @brief accelerometer data
     */
    std::vector<int16_t> acc;

    /**
     * @brief magnetometr data
     */
    std::vector<int16_t> mag;

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
    bool pushData(std::vector<int16_t> inp)
    {
        for(int i=0; i<3; i++)
        {
            gyro.push_back(inp[i+1]);
            acc.push_back(inp[i+4]);
            mag.push_back(inp[i+7]);
        }
    }
    bool print()
    {
        if(is_complete())
        {
            for(int i=0;i<3;i++)
            {
                std::cout<<"GYRO:"<<this->gyro[i];
            }
            // std::cout<<std::endl;
            for(int i=0;i<3;i++)
            {
                std::cout<<"ACC:"<<this->acc[i];
            }
            // std::cout<<std::endl;
            for(int i=0;i<3;i++)
            {
                std::cout<<"MAG:"<<this->mag[i];
            }
            std::cout<<std::endl;

        }


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

    bool is_complete()
    {
        return (pinky.is_complete() && ring.is_complete() && middle.is_complete()
                && index.is_complete() && thumb.is_complete() && palm.is_complete());
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
    bool print()
    {

        this->pinky.print();
        this->ring.print();
        this->middle.print();
        this->index.print();
        this->thumb.print();
        this->palm.print();
        std::cout<<std::endl;
    }

};

struct GRDevice
{
    int id = 0;
    int oldMsgId = 0;
    std::string name = "";
    std::string address = "";
    bool connected = false;
    int battPercents = 0;

    std::unique_ptr< tinyb::BluetoothDevice >  btTag;
    std::unique_ptr<tinyb::BluetoothGattService> imuService;
    std::unique_ptr< BluetoothGattCharacteristic > imuChar;

    std::unique_ptr<tinyb::BluetoothGattService> transmissionService;
    std::unique_ptr< tinyb::BluetoothGattCharacteristic > transmissionChar;

    std::unique_ptr<tinyb::BluetoothGattService> battService;
    std::unique_ptr<tinyb::BluetoothGattCharacteristic> battChar;

    std::deque<GRMessage> msgDeque;

    GRMessage cumulativeMsg;

    std::deque<std::vector<int16_t> > rawData;

        bool msgStart = false ;

    std::function<void ()> callback;
    void setDataCallback(std::function<void()> cb) {
      this->callback = cb;
    }
    GRDevice()
    {
        this->id = 0;

    }
    GRDevice(GRDevice &&dev) : btTag(std::move(dev.btTag)), imuService(std::move(dev.imuService)),
    imuChar(std::move(dev.imuChar)), transmissionService(std::move(dev.transmissionService)),
    transmissionChar(std::move(dev.transmissionChar)), battService(std::move(dev.battService)),
    battChar(std::move(dev.battChar))
    {
        this->id = dev.id;
        this->name = dev.name;
        this->address = dev.address;

    }
    //   GRDevice& operator=(GRDevice const& );


    GRDevice& operator=(GRDevice&& dev)
    {
        if(this != &dev)
        {
            this->id = dev.id;
            this->name = dev.name;
            this->address = dev.address;
            this->btTag = std::move(dev.btTag);
            this->imuService = std::move(dev.imuService);
            this->imuChar = std::move(dev.imuChar);
            this->transmissionService = std::move(dev.transmissionService);
            this->transmissionChar = std::move(dev.transmissionChar);
            this->battService = std::move(dev.battService);
            this->battChar = std::move(dev.battChar);

        }
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

        this->connected = false;

        //TODO clear proxies;
        //g_object_unref(propProxy);
        //g_object_unref(methodProxy);
    }
    private:
    GRDevice(const GRDevice &);
    GRDevice& operator=(const GRDevice &);
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
