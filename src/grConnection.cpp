#include "grConnection.h"
#include <sys/socket.h>
//constructor
GRConnection::GRConnection()
{
    this->_start = std::chrono::high_resolution_clock::now();
    /*will be useful if use rfcomm virtula descriptor
     * rfcommPath = "/dev/rfcomm0";
     setUpRfcomm(rfcommPath);
     */
}


//destructor
GRConnection::~GRConnection()
{
    for(int i=1; i <= _deviceSockets.size(); i++)
    {
        close(_deviceSockets[i].sock);
    }
}

//copy constructor
GRConnection::GRConnection(const GRConnection& t)
{
}


//operator =
GRConnection& GRConnection::operator=(const GRConnection& t)
{
}



/*Return map of avalible for connecting GR devices
 */
std::map<int, device_t> GRConnection::getAvalibleDevices()
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
bool GRConnection::setActiveDevice(int devId)
{
    std::unordered_map<int, device_t>::iterator it = this->_activeDevices.begin();
    while(it != this->_activeDevices.end()) 
    {
        if(devId == it->first)
        {
            std::cout<<"ERROR: could not ad device with id: "<<devId<<" because is exists"<<std::endl;
            return false;
        }
    }
    this->_activeDevices[devId] = this->_avalibleDevices[devId];
    _asignDeviceWithSocket(devId);
    _bufferedData[devId] = "";
    std::cout<<"Set device with address in active: "<<_activeDevices[devId].address<<std::endl;

}

//TODO needs to be implemented later
int GRConnection::getDeviceId(device_t device)
{
    return device.id;
}


device_t* GRConnection::getDevice(int devId)
{
    return &(_activeDevices[devId]);
}

/*Select device by ID and fill message with one peace of data 
 * Need to be use in loop
 */
bool GRConnection::getData(int devId, gr_message* message)
{
    int sock, status, bytes_read;
    int id;
    char buf[1] = {0};

    std::string rawMessage;

    sock = _getDeviceSocketById(devId); //= socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    bool messageAvalible = false;

    std::string readyMessage;

    int iter = 0;
    while(iter<6)
    { 

        bytes_read = read(sock, buf, sizeof(buf));
        if(bytes_read >0)
        {
                if(buf[0] != '\n')
                {   
                        rawMessage += buf[0];
                }
                else 
                {  

                    messageAvalible = true;
                    
    //               std::cout<<"message "<<rawMessage<<std::endl;
                    iter++;
                    _asignMessageWithImu(rawMessage, message);
                  /*  if(!message->palm.gyro.empty())
                    std::cout<<"in read data --->"<< message->palm.gyro[1]<<std::endl;             */
                    rawMessage.clear();

                }

        }

    }
       
    /*
       if(!this->activeDevices[devId].palm.data.empty())
       {
    // std::cout << "==" << iter << std::endl;
        for(int i=0; i<3; i++)
        {
            std::cout << this->activeDevices[devId].palm.data.front().gyro[i]<<" "; 
        }
        for(int i=0; i<3; i++)
        {
        
            std::cout << this->activeDevices[devId].palm.data.front().acc[i]<<" ";
        }
        for(int i=0;i<3;i++)
        {
        
            std::cout << this->activeDevices[devId].palm.data.front().mag[i]<<" ";
        }
        //std::cout<<std::endl;
        std::cout <<" ---- "<< this->activeDevices[devId].palm.data.front().time_stamp << std::endl;
        std::cout <<"size--:   "<< this->activeDevices[devId].palm.data.size()<<std::endl;
        this->activeDevices[devId].palm.data.pop_front();

    }
    */

   // rawMessage.clear();

}

/*Split raw device data in peaces and push them into structure
 */
bool GRConnection::_splitData(std::string data, imu* sensor)
{
    sensor->gyro.clear();
    sensor->acc.clear();
    sensor->mag.clear();
    int i = 0;
    int id;
    double n, arr[11];
    std::stringstream ss(data);
    // std::vector<double> gyro, acc, mag;

    //gr_message msg;
    while(ss >> n && i<11)
    {
        arr[i] = n;
        i++;
    }


    for(int i=1;i<11;i++)
    {
        if(i <= 3)
        {
            sensor->gyro.push_back(arr[i]);
        }
        else if(i > 3 && i <= 6)
        {
            sensor->acc.push_back(arr[i]);
        }
        else if(i > 6 && i<=9)
        {
            sensor->mag.push_back(arr[i]);
        }
        else if(i > 9)
        {
           sensor->time_stamp = arr[i];
        }

    }
    //std::cout<<"from split data "<<sensor.gyro[0]<<std::endl;
    return 1;
 }

/* Return local time stamp which starts with starting of program
 */
double GRConnection::_getTimeStamp()
{
    this->_timeStamp = 0.0;

    this->_end = std::chrono::high_resolution_clock::now();
    _timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>
        (_end-_start).count();

    _timeStamp = (_timeStamp * 0.001f);

    return _timeStamp;
}

/*Check id device in avalible device
 */
bool GRConnection::_deviceIsIn(std::string addr)
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

/* Asigning device with socke and redurn socket
 */
int GRConnection::_asignDeviceWithSocket(int devId)
{

    struct sockaddr_rc addr = { 0 };
    
    dev_socket deviceSocket;


    std::unordered_map<int, dev_socket>::iterator it = _deviceSockets.begin();

    while(it != this->_deviceSockets.end())
    {
        if(devId == it->first)
        {
            std::cout<<"ERROR this id: "<<devId<<" is already in use"<<std::endl;
            return 0; 
        }
        it++;
    }
    deviceSocket.sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1; //TODO maeby need to be changet with device id

    std::cout<<"Asigning device with addres: "<<this->_activeDevices[devId].address<<std::endl;
    
    str2ba(this->_activeDevices[devId].address.c_str(), &addr.rc_bdaddr);
    
    deviceSocket.addr = addr;
    this->_deviceSockets[devId] = deviceSocket;

    //connectSocket(devId);//TODO need to implement in another method
    
    std::cout<<"device with addres: "<<this->_activeDevices[devId].address<<"with socket"<<deviceSocket.sock<<std::endl;

    return deviceSocket.sock;
}

/*Return device by ID
 */
device_t GRConnection::_getDeviceById(int id)
{
    if(this->_activeDevices.find(id)->first != 0)
    {
        return this->_activeDevices[id];
    }
    else
    {
        std::cout<<"ERROR: No such device with id: "<<id<<std::endl;
        //        return; TODO need to ad empty device or error
    }
}

/*Return device socket
 */
int GRConnection::_getDeviceSocketById(int id)
{
    int devSock = this->_deviceSockets[id].sock;
    return devSock;
}

bool GRConnection::_asignMessageWithImu(std::string rawMessage, gr_message* message)
{   
    int id;
    bool messageAvalible = false;
    std::stringstream ss(rawMessage);

    ss >> id;
    if(id == 0)
    {
        _splitData(rawMessage, &message->pinky);
        //message->pinky.time_stamp = getTimeStamp();//TODO add if statement
    }
    else if(id ==1)
    {
        _splitData(rawMessage, &message->ring);
        //message->ring.time_stamp = getTimeStamp();
    }
    else if(id == 2)
    {
        _splitData(rawMessage, &message->middle);
        //message->middle.time_stamp = getTimeStamp();
    }
    else if(id == 3)
    {
        _splitData(rawMessage, &message->index);
        //message->index.time_stamp = getTimeStamp();
    }
    else if(id==4)
    {
        _splitData(rawMessage, &message->thumb);
        //message->thumb.time_stamp = getTimeStamp();
    }
    else if(id==5)
    {
       // std::cout<<rawMessage<<std::endl;
        _splitData(rawMessage, &message->palm);
        //message->palm.time_stamp = getTimeStamp();

    }
    //rawMessage.clear();
    //TODO need to comment
    if(!message->palm.gyro.empty())
    {   
        //std::cout<<device->palm.data.front().gyro[1]<<std::endl;
        //device->palm.data.pop_front();
    }

}

/*Make connection to socket of device
 */
bool GRConnection::connectSocket(int devId)
{
    int status;

    status = connect(this->_deviceSockets[devId].sock, 
            (struct sockaddr *)(this->_deviceSockets[devId].getAddrRef()), sizeof(this->_deviceSockets[devId].addr));

    std::cout<<"Status of connection of socket=  "<<status<<std::endl;
    if(status == -1)
    {
        printf("Oh dear, something went wrong with connect! %s\n", strerror(errno));
    }

    return true;

}
