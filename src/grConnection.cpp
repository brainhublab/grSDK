#include "grConnection.h"
#include <sys/socket.h>
//constructor
GRConnection::GRConnection()
{
    this->start = std::chrono::high_resolution_clock::now();
    /*will be useful if use rfcomm virtula descriptor
     * rfcommPath = "/dev/rfcomm0";
     setUpRfcomm(rfcommPath);
     */
}


//destructor
GRConnection::~GRConnection()
{
    for(int i=1; i <= deviceSockets.size(); i++)
    {
        close(deviceSockets[i].sock);
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



//TODO need to be implemented later
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
            if(!deviceIsIn(device.address))
            {
                device.id = (avalibleDevices.size() + 1);
                this->avalibleDevices[device.id] = device;
            }
            std::cout<<"stored in avalibleDevices device with addres "<<avalibleDevices[device.id].address;
            std::cout<<" and name "<< avalibleDevices[device.id].name<<std::endl;

            device.clear_attributes();
        }
    }

    return avalibleDevices;
}

bool GRConnection::setActiveDevice(int devId)
{
    std::unordered_map<int, device_t>::iterator it = this->activeDevices.begin();
    while(it != this->activeDevices.end()) 
    {
        if(devId == it->first)
        {
            std::cout<<"ERROR: could not ad device with id: "<<devId<<" because is exists"<<std::endl;
            return false;
        }
    }
    this->activeDevices[devId] = this->avalibleDevices[devId];
    asignDeviceWithSocket(devId);
    bufferedData[devId] = "";
    std::cout<<"Set device with address in active: "<<activeDevices[devId].address<<std::endl;

}

//TODO needs to be implemented later
int GRConnection::getDeviceId(device_t device)
{
    return device.id;
}

std::unordered_map<std::string, gr_message> GRConnection::getData(int devId, std::string type)
{   
    //TODO implement sesor spliting
    //std::map<std::string, gr_message> messages;
    std::unordered_map<std::string, gr_message> messages;
    if(type == "ITER")
    {
        readData(devId);
    }
    else if(type == "THREAD")
    {
        readDataThr(devId); 
    }
    else
    {
        std::cout<<"ERROR: no such type= "<<type<<" for recieving data please select ITER or THREAD";
    }

    if(!(activeDevices[devId].pinky.data.empty() ||
                activeDevices[devId].ring.data.empty() ||
                activeDevices[devId].middle.data.empty() ||
                activeDevices[devId].index.data.empty() ||
                activeDevices[devId].thumb.data.empty() ||
                activeDevices[devId].palm.data.empty() ))
    {
        messages["pinky"] = activeDevices[devId].pinky.data.front();
        activeDevices[devId].pinky.data.pop_front();
        messages["ring"] = activeDevices[devId].ring.data.front();
        activeDevices[devId].ring.data.pop_front();
        messages["middle"] = activeDevices[devId].middle.data.front();
        activeDevices[devId].middle.data.pop_front();
        messages["index"] = activeDevices[devId].index.data.front();
        activeDevices[devId].index.data.pop_front();
        messages["thumb"] = activeDevices[devId].thumb.data.front();
        activeDevices[devId].thumb.data.pop_front();
        messages["palm"] = activeDevices[devId].palm.data.front();
        activeDevices[devId].palm.data.pop_front();
    }


    return messages;  
}

device_t* GRConnection::getDevice(int devId)
{
    return &(activeDevices[devId]);
}

bool GRConnection::readData(int devId)
{
    int sock, status, bytes_read;
    int id;
    char buf[1] = {0};

    std::string rawMessage;

    sock = getDeviceSocketById(devId); //= socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    bool messageAvalible = false;

    std::string readyMessage;

    int iter = 0;
    while(iter<6)
    { 

        bytes_read = read(sock, buf, sizeof(buf));
    //    std::cout<<"========"<<std::endl;
        if(bytes_read >0)
        {
                if(buf[0] != '\n')
                {   
                        rawMessage += buf[0];
                }
                else 
                {  

                    messageAvalible = true;
                    
             //       std::cout<<"message "<<rawMessage<<std::endl;
                    iter++;
                    asignMessageWithImu(rawMessage, &(activeDevices[devId]));             
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

bool GRConnection::readDataThr(int devId)
{
    std::thread thr(&GRConnection::readData, this, devId);
    std::thread::id thrId;

    thrId = thr.get_id();
    thr.detach();

    std::cout<<"connection thread is running"<<std::endl;

    return true;
}

/*
   std::string GRConnection::getNext()
   {
   char buf[2];
   ssize_t n;
   std::string res;

   while(true)
   {
   n = read(portDescriptor, buf, 1);
   if(n < 0)
   {
   std::cout << "GRConnection::getNext read failed" << std::endl;
   return std::string("");
   }
   if(buf[0]!= '\n')
   {
   res += buf[0];
   }
   else
   {
   return res;
   }
   }
   }
   */
bool GRConnection::splitData(std::string data, imu* sensor)
{
    int i = 0;
    int id;
    float n, arr[10];
    std::stringstream ss(data);
    // std::vector<float> gyro, acc, mag;

    gr_message msg;
    while(ss >> n && i<10)
    {
        arr[i] = n;
        i++;
    }

    for(int i=1;i<10;i++)
    {
        if(i <= 3)
        {
            msg.gyro.push_back(arr[i]);
        }
        else if(i > 3 && i <= 6)
        {
            msg.acc.push_back(arr[i]);
        }
        else if(i > 6 )
        {
            msg.mag.push_back(arr[i]);
        }
    }
    sensor->data.push_back(msg);

    msg.gyro.clear();
    msg.acc.clear();
    msg.mag.clear();

}

float GRConnection::getTimeStamp()
{
    this->timeStamp = 0.0;

    this->end = std::chrono::high_resolution_clock::now();
    timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>
        (end-start).count();

    timeStamp = (timeStamp * 0.001f);

    return timeStamp;
}

bool GRConnection::deviceIsIn(std::string addr)
{
    int i=1;

    while(i <= avalibleDevices.size())
    {
        if(addr != avalibleDevices[i].address)
        {
            i++; 
        }
        else if(addr == avalibleDevices[i].address) 
        {

            return true;
        }
    }

    return false;
}

int GRConnection::asignDeviceWithSocket(int devId)
{

    struct sockaddr_rc addr = { 0 };
    
    dev_socket deviceSocket;


    std::unordered_map<int, dev_socket>::iterator it = deviceSockets.begin();

    while(it != this->deviceSockets.end())
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

    std::cout<<"Asigning device with addres: "<<this->activeDevices[devId].address<<std::endl;
    
    str2ba(this->activeDevices[devId].address.c_str(), &addr.rc_bdaddr);
    
    deviceSocket.addr = addr;
    this->deviceSockets[devId] = deviceSocket;

    //connectSocket(devId);//TODO need to implement in another method
    
    std::cout<<"device with addres: "<<this->activeDevices[devId].address<<"with socket"<<deviceSocket.sock<<std::endl;

    return deviceSocket.sock;
}


device_t GRConnection::getDeviceById(int id)
{
    if(this->activeDevices.find(id)->first != 0)
    {
        return this->activeDevices[id];
    }
    else
    {
        std::cout<<"ERROR: No such device with id: "<<id<<std::endl;
        //        return; TODO need to ad empty device or error
    }
}

int GRConnection::getDeviceSocketById(int id)
{
    int devSock = this->deviceSockets[id].sock;
    return devSock;
}

bool GRConnection::asignMessageWithImu(std::string rawMessage, device_t* device)
{   
    int id;
    bool messageAvalible = false;
    std::stringstream ss(rawMessage);

    ss >> id;
    if(id == 0)
    {
        splitData(rawMessage, &device->pinky);
        device->pinky.data.back().time_stamp = getTimeStamp();//TODO add if statement
    }
    else if(id ==1)
    {
        splitData(rawMessage, &device->ring);
        device->ring.data.back().time_stamp = getTimeStamp();
    }
    else if(id == 2)
    {
        splitData(rawMessage, &device->middle);
        device->middle.data.back().time_stamp = getTimeStamp();
    }
    else if(id == 3)
    {
        splitData(rawMessage, &device->index);
        device->index.data.back().time_stamp = getTimeStamp();
    }
    else if(id==4)
    {
        splitData(rawMessage, &device->thumb);
        device->thumb.data.back().time_stamp = getTimeStamp();
    }
    else if(id==5)
    {
       // std::cout<<rawMessage<<std::endl;
        splitData(rawMessage, &(device->palm));
        device->palm.data.back().time_stamp = getTimeStamp();

    }
    //rawMessage.clear();
    //TODO need to comment
    if(!device->palm.data.empty())
    {   
        //std::cout<<device->palm.data.front().gyro[1]<<std::endl;
        //device->palm.data.pop_front();
    }

}

bool GRConnection::connectSocket(int devId)
{
    int status;

    status = connect(this->deviceSockets[devId].sock, 
            (struct sockaddr *)(this->deviceSockets[devId].getAddrRef()), sizeof(this->deviceSockets[devId].addr));

    std::cout<<"Status of connection of socket=  "<<status<<std::endl;
    if(status == -1)
    {
        printf("Oh dear, something went wrong with connect! %s\n", strerror(errno));
    }

    return true;

}
