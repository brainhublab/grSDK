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
    //close(portDescriptor);
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
            //device.id = i;
            
            std::cout<<name<<" "<<addr<<std::endl;
            
            device.name.assign(name, strlen(name));
            device.address.assign(addr, strlen(addr));
            
            std::cout<<"addr in class "<<device.address<<std::endl;
            std::cout<<"name in class "<<device.name<<std::endl;
            if(!deviceIsIn(device.address))
            {
                device.id = (avalibleDevices.size() + 1);
                avalibleDevices[device.id] = device;
            }
            std::cout<<"size in class "<<avalibleDevices.size()<<std::endl;
            std::cout<<"address afer store "<<avalibleDevices[1].address<<std::endl;
            std::cout<<"name after store"<< avalibleDevices[1].name<<std::endl;
            
            device.clear_attributes();
        }
    }
    //sleep(1);
       // std::cout<< "add from class"<<devices.front().addr<<std::endl;


    return avalibleDevices;

}

bool GRConnection::setActiveDevice(int id)
{
   std::map<int, device_t>::iterator it = activeDevices.begin();
   while(it != activeDevices.end()) 
   {
        if(id == it->first)
        {
            std::cout<<"ERROR: could not ad device with id: "<<id<<" because is exists"<<std::endl;
            return false;
        }
   }
   activeDevices[id] = avalibleDevices[id];
   asignDeviceWithSocket(id);
   bufferedData[id] = "";
   std::cout<<"in set active devices"<<activeDevices[id].address<<std::endl;
  
}

//TODO needs to be implemented later
int GRConnection::getDeviceId(device_t device)
{
    return device.id;
}


bool GRConnection::getData(int devId)
{
    struct sockaddr_rc saddr = { 0 }; 
    int sock, status, bytes_read;
    int id;
    char buf[1024] = {0};

    std::string rawMessage;
    rawMessage.append(bufferedData[devId]);

    //char dest TODO
    
    sock = getDeviceSocketById(devId); //= socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    saddr = deviceSockets[devId].addr;
   /* char tmp[19];
   ba2str(&saddr.rc_bdaddr, tmp);
    for(int i=0;i<19;i++)
    {
        std::cout<<tmp[i];
    } 
    std::cout<<"____check addr in get data"<<std::endl;
    */
    //addr.rc_family = AF_BLUETOOTH;
   // addr.rc_channel = (uint8_t) 1; //TODO maeby need to be changet with device id
    //str2ba(device->address.c_str(), &addr.rc_bdaddr);

    //connect to gr
   // std::cout<<"address from get data "<< activeDevices[devId].address<<std::endl;
   // std::cout<<"socket from get data " << deviceSockets[devId].sock<< sock <<std::endl;
   
   
    bool isStart = false;
    bool messageAvalible = false;
    std::string readyMessage;

    if(!rawMessage.empty())
    {
        isStart = true;
    }

   while(!messageAvalible)
   { //std::cout<<"go"<<std::endl;

        bytes_read = read(sock, buf, sizeof(buf));
        if(bytes_read >0)
        {
           for(int i=0; i<bytes_read; i++)
            {
                if(buf[i] != '\n')
                {   
                    if(isStart)
                    {                  
                        rawMessage += buf[i];
                    }
                    else if(buf[i] == '|')
                    {
                        isStart = true;
                    }
                }
                else if(rawMessage.size() != 0)
                {  

                    readyMessage = rawMessage;
                    messageAvalible = true;
                    isStart = false;

                    std::cout<<"++++++++++"<<readyMessage<<std::endl;
                    //std::cout<<sizeof(rawMessage);
                  //  std::cout<<"--->>>"<<rawMessage.length()<<"<<<---"<<std::endl;
                   rawMessage.clear();                    
                }

            }
        }
        
   }
   bufferedData[devId] = rawMessage;
   // asignMessageWithImu(rawMessage, &(activeDevices[devId]));
    //rawMessage.clear();
    //close(sock);
}
/*bool GRConnection::getDataThr(device_t* device)
{
    std::thread thr(&GRConnection::getData, this, device);
    std::thread::id thrId;

    thrId = thr.get_id();
    thr.detach();

    std::cout<<"connection thread is running"<<std::endl;

    return true;
}*/
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
  //  if(msg.gyro.size() == 3 && msg.acc.size() == 3 && msg.mag.size() == 3)
    //{
        sensor->data.push_back(msg);
        
        msg.gyro.clear();
        msg.acc.clear();
        msg.mag.clear();
     //   return true;
    //}
//    else
  //  {
    //    return false;
   // }
    
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

int GRConnection::asignDeviceWithSocket(int id)
{

    struct sockaddr_rc addr = { 0 };
    int i=1;
    int s;

    dev_socket deviceSocket;


    std::map<int, dev_socket>::iterator it = deviceSockets.begin();
    
    while(it != deviceSockets.end())
    {
        if(id == it->first)
        {
            std::cout<<"ERROR this id: "<<id<<" is already in use"<<std::endl;
            return 0; 
        }
        it++;
    }
    deviceSocket.sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    
    std::cout<<"this is address when asign"<<activeDevices[1].address<<std::endl;
    
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1; //TODO maeby need to be changet with device id
    std::cout<<"this is address when asign"<<activeDevices[id].address<<std::endl;
    str2ba(activeDevices[id].address.c_str(), &addr.rc_bdaddr);
    char tmp[19];
    ba2str(&addr.rc_bdaddr, tmp);
    for(int i=0;i<19;i++)
    {
        std::cout<<tmp[i];
    } 
    std::cout<<"<<__________"<<std::endl;
    deviceSocket.addr = addr;
    deviceSockets[id] = deviceSocket;

    connectSocket(id);
        
    return s;
}


device_t GRConnection::getDeviceById(int id)
{
    if(activeDevices.find(id)->first != 0)
    {
        return activeDevices[id];
    }
    else
    {
        std::cout<<"ERROR: No such device with id: "<<id<<std::endl;
//        return; TODO need to ad empty device or error
    }
}

int GRConnection::getDeviceSocketById(int id)
{
    int devSock = deviceSockets[id].sock;
    return devSock;
}

bool GRConnection::asignMessageWithImu(std::string rawMessage, device_t* device)
{   
    int id;
    std::cout<<"this is raw message"<<rawMessage<<std::endl;
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
        splitData(rawMessage, &(device->palm));
        device->palm.data.back().time_stamp = getTimeStamp();

    }
    //rawMessage.clear();
     std::cout<<device->palm.data.front().time_stamp;

}

bool GRConnection::connectSocket(int devId)
{
    int status;

    status = connect(deviceSockets[devId].sock, 
            (struct sockaddr *)(deviceSockets[devId].getAddrRef()), sizeof(deviceSockets[devId].addr));
    std::cout<<"status= "<<status<<std::endl;
    if(status == -1)
    {
        printf("Oh dear, something went wrong with connect! %s\n", strerror(errno));
    }

}
