#include "/home/h3lgi/code/grSDK/inc/grDevManager.h"
#include <sys/socket.h>
//constructor
GRDevManager::GRDevManager()
{
}


//destructor
GRDevManager::~GRDevManager()
{
    //  for(int i=1; i <= _deviceSockets.size(); i++)
    //  {
    //      close(_deviceSockets[i].sock);
    //  }
}

//copy constructor
GRDevManager::GRDevManager(const GRDevManager& t)
{
}


//operator =
GRDevManager& GRDevManager::operator=(const GRDevManager& t)
{
}



/*Return map of avalible for connecting GR devices
*/
std::unordered_map<int, device_t> GRDevManager::getAvalibleDevices()
{
    dev_names deviceNames;
    device_t device;
    //std::map<int, device_t> devices;

    inquiry_info *inqInfo = NULL;
    int maxRsp, numRsp;
    int devId, sock, len, flags;

    char addr[19] = {0};
    char name[248] = {0};

    devId = hci_get_route(NULL);
    sock = hci_open_dev(devId);
    if(devId < 0 || sock < 0)
    {
        perror("opening socket while scan devices");
        exit(1);
    }

    len = 8;
    maxRsp = 255;
    flags = IREQ_CACHE_FLUSH;
    inqInfo = (inquiry_info*)malloc(maxRsp * sizeof(inquiry_info));

    numRsp = hci_inquiry(devId, len, maxRsp, NULL, &inqInfo, flags);
    if(numRsp < 0)
    {
        perror("hci_inquiry error while scanning devices");
    }

    for(int i =0; i < numRsp; i++)
    {
        ba2str(&(inqInfo + i)->bdaddr, addr);
        memset(name, 0, sizeof(name));
        if(hci_read_remote_name(sock, &(inqInfo + i)->bdaddr, sizeof(name), name, 0) < 0)
        {
            strcpy(name, "[unknown]");
        }
        else if(deviceNames.left == name || deviceNames.right == name || deviceNames.test == name )
        {
            //  device.id = i;

            std::cout<<name<<" "<<addr<<std::endl;

            device.name.assign(name, strlen(name));
            device.address.assign(addr, strlen(addr));

            std::cout<<"Finded device with address: "<<device.address;
            std::cout<<" and name "<<device.name<<std::endl;
            if(!_deviceIsIn(device.address))
            {
                device.id = (_avalibleDevices.size() + 1);
                this->_avalibleDevices[device.id] = device;
            }
            std::cout<<"stored in avalibleDevices device with addres "<<_avalibleDevices[device.id].address;
            std::cout<<" and name "<< _avalibleDevices[device.id].name<<std::endl;

            device.clear_attributes();
        }
    }

    return _avalibleDevices;
}

/*Set GR device from avalible to active and make it ready for connection
*/
GRConnection* GRDevManager::setActiveDevice(int devId)
{
    std::unordered_map<int, GRConnection>::iterator it = this->_activeDevices.begin();
    while(it != this->_activeDevices.end())
    {
        if(devId == it->first)
        {
            std::cout<<"ERROR: could not ad device with id: "<<devId<<" because is exists"<<std::endl;
            return &it->second;
        }
        it++;
    }

    this->_activeDevices[devId] = GRConnection(this->_avalibleDevices[devId]);
    std::cout<<"Set device with address in active: "<<_avalibleDevices[devId].address<<std::endl;
    return &this->_activeDevices[devId];
}

//TODO needs to be implemented later
/*Return device by ID
*/
GRConnection* GRDevManager::getActiveDeviceById(int id)
{
    if(this->_activeDevices.find(id)->first != 0)
    {
        return &this->_activeDevices[id];
    }
    else
    {
        std::cout<<"ERROR: No such device with id: "<<id<<std::endl;
        return NULL;
    }
}

/*Check id device in avalible device
*/
bool GRDevManager::_deviceIsIn(std::string addr)
{
    int i=1;

    while(i <= _avalibleDevices.size())
    {
        if(addr != _avalibleDevices[i].address)
        {
            i++;
        }
        else if(addr == _avalibleDevices[i].address)
        {

            return true;
        }
    }

    return false;
}




