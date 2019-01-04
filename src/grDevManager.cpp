#include "grDevManager.h"
#include <sys/socket.h>
#include <gio/gio.h>
//constructor
GRDevManager::GRDevManager()
{

    btManager = nullptr;
    try{
        btManager = tinyb::BluetoothManager::get_bluetooth_manager();  
    }
    catch(const std::runtime_error& err)
    {
        std::cerr << "ERROR: initializing libtinyb BluetoothManager"<< err.what()<<std::endl;
        exit(1);
    }


    /*
       GError* _err = NULL;
       _rootProxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
       G_DBUS_PROXY_FLAGS_DO_NOT_LOAD_PROPERTIES,
       NULL,
       bluezBus.c_str(),
       "/",
       "org.freedesktop.DBus.ObjectManager",
       NULL,
       &_err);
       g_error_free(_err);
       */
}

//destructor
GRDevManager::~GRDevManager()
{
}

//copy constructor
GRDevManager::GRDevManager(const GRDevManager& t)
{
}

//operator =
GRDevManager& GRDevManager::operator=(const GRDevManager& t)
{
}

std::unordered_map<int, GRDevice>*GRDevManager::getAvalibleGRDevices()
{
    this->_getAvalibleiGRDevices();
    /*    std::unordered_map<int, GRDevice*> outDevs;
          for(auto device : this->_avalibleGRDevices)
          {
          outDevs[device.id] = &device;
          }
          */
    return &_avalibleGRDevices;
} 

GRDevice* GRDevManager::getGRDeviceById(int devId)
{
    return &(_avalibleGRDevices[devId]);
}

bool GRDevManager::getData(int devId)
{
    // std::thread thr(&GRDevManager::subscribe, this,  devId);
    // thr.detach();
    auto& dev = _avalibleGRDevices[devId];
    if(dev.cumulativeMsg.is_complete() && dev.cumulativeMsg.id != dev.oldMsgId)
    {
        dev.oldMsgId = dev.cumulativeMsg.id;
        // dev.cumulativeMsg.print();
        //dev.cumulativeMsg;
        return true;
    }
    return false;


}

void GRDevManager::batteryCallback(BluetoothGattCharacteristic &c, std::vector<unsigned char> &data, void *userdata)
{
    //PlaceHOLDER!!!
    unsigned char* data_c;
    unsigned int dataSize = data.size();
    auto dev = static_cast<GRDevice*>(userdata);
    if(dataSize > 0)
    {
        data_c = data.data();
        int bp = (int)data[0];
        dev->battPercents = bp;
        std::cout<<bp<<"________IN CALLBACK BATT"<<std::endl;
    }
}

void GRDevManager::dataCallback(BluetoothGattCharacteristic &c, std::vector<unsigned char> &data, void *userdata)
{
    unsigned char* data_c;
    unsigned int dataSize = data.size();
    auto dev = static_cast<GRDevice*>(userdata);
    if(dataSize > 0)
    {

        data_c = data.data();
        //  uint8_t imuId = static_cast<uint8_t>(data_c[0]);
        int imuId = (int)data_c[0];


        std::vector<int16_t> msg;
        unsigned i=0;
        while(i<18)
        {
            if(i==0)
            {
                // msg.push_back(imuId);
                i++;

            }
            else
            {
                uint16_t dataPiece = (data_c[i] & 0xFF) << 8 | (data_c[i+1] & 0xFF);
                // int16_t piece = (data_c[i] << 8) | (data_c[i+1]);
                msg.push_back(dataPiece);
                //          std::cout<<dataPiece<<"["<< i << i+1<<"]"<<" ";

                i+=2;
            }
        }
        //    std::cout<<std::endl;
        //  std::cout<<msg.size()<<"MSG SIZE"<<std::endl;

        if(imuId == 0)
        {

            dev->cumulativeMsg.clear();
            dev->cumulativeMsg.id++;
            dev->cumulativeMsg.pinky.pushData(msg);
        }
        else if(imuId == 1)
        {
            dev->cumulativeMsg.ring.pushData(msg);
        }
        else if(imuId == 2)
        {
            dev->cumulativeMsg.middle.pushData(msg);
        }
        else if(imuId == 3)
        {

            dev->cumulativeMsg.index.pushData(msg);
        }
        else if(imuId == 4)
        {
            dev->cumulativeMsg.thumb.pushData(msg);
        }
        else if(imuId == 5)
        {
            // std::cout<<"PALM INDEX"<<std::endl;
            dev->cumulativeMsg.palm.pushData(msg);
            //  dev->data.push_back(dev->cumulativeMsg);
            //  dev->cumulativeMsg.clear();
        }


    }
}


std::vector<int16_t> GRDevManager::convertFromBytes(unsigned char* bytes)
{
    //TODO optimize
    std::vector<int16_t> msg;
    int i=0;
    while( i<18)
    {
        if(i!=0)
        {
            int16_t piece = (bytes[i] & 0xFF) << 8 | (bytes[i+1] & 0xFF);
            msg.push_back(piece);
            i+=2;
        }
    }
    return msg;
}



bool GRDevManager::_startDiscovery()
{   
    bool flag = this->btManager->start_discovery();
    std::cout<<"Discovery of devices is "<<(flag ? "started":"not started" )<<std::endl;
    return flag;
}

bool GRDevManager::_stopDiscovery()
{

    bool flag = this->btManager->stop_discovery();
    std::cout<<"Discovery of devices is "<<(flag? "stoped": "not stoped")<<std::endl;
    return flag;
}

bool GRDevManager::_getAvalibleiGRDevices()
{
    _startDiscovery();
    usleep(5000000);
    //    this->btManager->start_discovery();
    //std::vector<std::unique_ptr<tinyb::BluetoothDevice> > allDevices = btManager->get_devices();
    for(auto& btIter : this->btManager->get_devices())
    {
        if(btIter->get_name() == lName || btIter->get_name() == rName)
        {
            GRDevice grDev;
            grDev.id = _grDevId;
            grDev.name = btIter->get_name();
            grDev.address = btIter->get_address();
            //grDev.btTag = btManager->find<BluetoothDevice>(nullptr, &grDev.address, nullptr, std::chrono::seconds(2));
            grDev.btTag = std::move(btIter);
            //devVec.emplace_back(std::move(grDev));
            if(!_deviceIsIn(&grDev))
            {

                this->_avalibleGRDevices[_grDevId] = std::move(grDev);

                _grDevId ++;
            }
        }
    }

    for(auto& it: _avalibleGRDevices) 
    {
        // std::cout << typeid(it.btTag).name() << std::endl;
        std::cout<<it.second.name<<std::endl;
    }
    // return _avalibleGRDevices;
    //  if(this->btManager->get_discovering ())
    // {

    //    _stopDiscovery();
    // }
}

bool GRDevManager::startTransmission(int devId)
{
    _avalibleGRDevices[devId].transmissionChar->write_value(start_transmission);

}

bool GRDevManager::endTransmission(int devId)
{
    this->_avalibleGRDevices[devId].transmissionChar->write_value(end_transmission);
    //    this->_avalibleGRDevices[devId].transmissionChar->write_value(end_zerofy);
}

float GRDevManager::getBatteryState(int devId)
{
    std::vector<unsigned char> battVal;
    this->_avalibleGRDevices[devId].transmissionChar->write_value(check_bat);
    /*
       battVal = this->_avalibleGRDevices[devId].battChar->get_value();
       if(!battVal.empty())
       {
       std::cout<<battVal.size()<<"SIZEOF"<<std::endl;
       printf("BATT %x \n", battVal[0]);
       for(auto c : battVal)
       {
       std::cout<<static_cast<int>(battVal[0])<<"  Battery--------------------------------------------------------"<<std::endl;
       }

       }
       */
}

bool GRDevManager::connect(int devId)
{
    auto& dev = _avalibleGRDevices[devId];
    std::cout<<dev.address<<std::endl;
    std::string addr = dev.name;
    //auto grTag = btManager->find<BluetoothDevice>(nullptr, &addr, nullptr, std::chrono::seconds(2));
    if(dev.btTag == nullptr)
    {
        std::cerr<<"ERROR: something go wrong so cannot emit device with ID: "<<devId<<std::endl;
        return false;
    }

    dev.btTag->enable_connected_notifications([](BluetoothDevice &grDev, bool connected, void* userData)
            {if(connected) std::cout<<"Device with name: "<<grDev.get_name()<<"and address: "<<grDev.get_address()<<" is connected"<<std::endl;},
            NULL);

    if(dev.btTag != nullptr)
    {
        dev.btTag->connect();
        //dev.btTag->pair();
        std::cout<<"Waiting for IMU service:  "<<imuServiceUUID<<"to be discovered"<<std::endl;

        std::string imuService = imuServiceUUID;
        dev.imuService = dev.btTag->find(&imuService, std::chrono::seconds(2));

        std::cout<<"Waiting for Transmission service:  "<<transmissionServiceUUID<<"to be discovered"<<std::endl;
        std::string transmissionService = transmissionServiceUUID;
        dev.transmissionService = dev.btTag->find(&transmissionService, std::chrono::seconds(2));

        std::cout<<"Waiting for Battery service:  "<<transmissionServiceUUID<<"to be discovered"<<std::endl;
        std::string battService = battServiceUUID;
        dev.battService = dev.btTag->find(&battService, std::chrono::seconds(2));
        if(dev.battService == nullptr)
        {
            std::cout<<"BATT SERVICE IS NULL"<<std::endl;
        }
    }
    else
    {   std::cout<<"ERROR: something goes wrong with connecting to TAG"<<std::endl;
        return false;
        //TODO stop discovery
    }
    std::cout<<"Wainting for IMU characteristic"<<imuGattCharUUID<<std::endl;
    std::string imuChar = imuGattCharUUID;
    dev.imuChar = dev.imuService->find(&imuChar, std::chrono::seconds(2));

    std::cout<<"Waiting form Transmission characteristic"<<transmissionCharUUID<<std::endl;
    std::string transmissionChar = transmissionCharUUID;
    dev.transmissionChar = dev.transmissionService->find(&transmissionChar, std::chrono::seconds(2));

    std::cout<<"Waiting for Battery characteristic"<<battCharUUID<<std::endl;
    std::string battChar = battCharUUID;
    dev.battChar = dev.battService->find(&battChar, std::chrono::seconds(2));
    if(dev.battChar == nullptr)
    {
        std::cout<<"BATT IS NULLPT"<<std::endl;
    }

    return true;

}
bool GRDevManager::disconnect(int devId)
{
    auto& dev = _avalibleGRDevices[devId];
    dev.btTag->disconnect();
}
void GRDevManager::subscribe(int devId)
{
    this->startTransmission(devId);


    //tmp_conf->write_value(config_on);
    auto& dev = this->_avalibleGRDevices[devId];

    dev.imuChar->enable_value_notifications(&GRDevManager::dataCallback, &(dev));

    dev.battChar->enable_value_notifications(&GRDevManager::batteryCallback, &(dev));
    this->getBatteryState(devId);
    std::mutex m;
    std::unique_lock<std::mutex> lock(m);

    //  std::signal(SIGINT, signal_handler);
    //conditionVar.wait(lock);

    /*  if(this->_avalibleGRDevices[devId].btTag != nullptr)
        {
        this->_avalibleGRDevices[devId].btTag->disconnect();
        }
        */
}
void GRDevManager::unsubscribe(int devId)
{
    if(this->_avalibleGRDevices[devId].btTag != nullptr)
    {
        this->_avalibleGRDevices[devId].imuChar->disable_value_notifications();
        this->_avalibleGRDevices[devId].btTag->disconnect();
    }


}

// Check id device in avalible device /
bool GRDevManager::_deviceIsIn(GRDevice* grDev)
{
    for(auto devIt=this->_avalibleGRDevices.begin(); devIt!= this->_avalibleGRDevices.end(); devIt++)
    {
        if(grDev->address == devIt->second.address)
        {
            return true;
        }
    }
    return false;
}


