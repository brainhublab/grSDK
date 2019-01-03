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
    std::thread thr(&GRDevManager::subscribe, this,  devId);
    thr.detach();

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
        std::cout<<"Waiting for service:  "<<imuServiceUUID<<"to be discovered"<<std::endl;

        std::string service = imuServiceUUID;
        dev.imuService = dev.btTag->find(&service);

    }
    else
    {   std::cout<<"ERROR: something goes wrong with connecting to TAG"<<std::endl;
        return false;
        //TODO stop discovery
    }
    std::string characteristic = imuGattCharUUID;
    dev.imuChar = dev.imuService->find(&characteristic);


    std::cout<<_avalibleGRDevices[devId].name<<std::endl;
    return true;
    //if(this->btManager->get_discovering ())

    // {

    _stopDiscovery();
    //}



}
void GRDevManager::dataCallback(BluetoothGattCharacteristic &c, std::vector<unsigned char> &data, void *userdata)
{
    unsigned char* data_c;
    unsigned int dataSize = data.size();
    auto dev = static_cast<GRDevice*>(userdata);

    //std::cout<<dev->name<<"  IN CALLBACK ---------------"<<std::endl;
    /*
       if(dataSize >0)
       {
       data_c = data.data();

       std::cout<<" Raw data ---[";
       for(unsigned i=0;i<dataSize; i++)
       {
    //  std::cout<<std::hex<<static_cast<int>(data_c[i])<<" - ";

    }
    std::cout<<"]"<<std::endl;


    }
    */
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
                i++;
            }
            else
            {
                uint16_t dataPiece = (data_c[i] & 0xFF) << 8 | (data_c[i+1] & 0xFF);
                // int16_t piece = (data_c[i] << 8) | (data_c[i+1]);
                msg.push_back(dataPiece);
        //        std::cout<<dataPiece<<"["<< i << i+1<<"]"<<" ";

                i+=2;
            }
        }
//        std::cout<<std::endl;
      //  std::cout<<msg.size()<<"MSG SIZE"<<std::endl;

        if(imuId == 0)
        {
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
            dev->data.push_back(dev->cumulativeMsg);
            dev->cumulativeMsg.clear();
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
void GRDevManager::subscribe(int devId)
{
    //   auto& dev = _avalibleGRDevices[devId];
    this->_avalibleGRDevices[devId].imuChar->enable_value_notifications(&GRDevManager::dataCallback, &(this->_avalibleGRDevices[devId]));
//    std::mutex m;
  //  std::unique_lock<std::mutex> lock(m);

//    std::signal(SIGINT, signal_handler);
//    conditionVar.wait(lock);

  /*  if(this->_avalibleGRDevices[devId].btTag != nullptr)
    {
        this->_avalibleGRDevices[devId].btTag->disconnect();
    }
*/
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


