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
std::vector<device_t> GRConnection::getAvalibleDevices()
{
    dev_names deviceNames;
    device_t device;
    std::vector<device_t> devices;

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
            device.id = i;
            device.name = (std::string(name));
            device.addr = (std::string(addr));

            devices.push_back(device);

            device.clear_attributes();
        }
    }
    std::cout<< devices.size()<<std::endl;

    free(inqInfo);
    close(sock);

    return devices;

}


//TODO needs to be implemented later
int GRConnection::getDeviceId(device_t dev)
{
}

bool GRConnection::getData(device_t* device)
{
 
    struct sockaddr_rc addr = { 0 };
    int sock, status, bytes_read;
    int id;
    char buf[1024] = {0};

    std::string rawMessage;

    //char dest TODO
    
    sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1; //TODO maeby need to be changet with device id
    str2ba(device->addr.c_str(), &addr.rc_bdaddr);

    //connect to gr
    status = connect(sock, (struct sockaddr *)&addr, sizeof(addr));

    while(rawMessage.size() != 10)
    {
        bytes_read = read(sock, buf, sizeof(buf));

        if(bytes_read >0)
        {
            for(int i=0; i<bytes_read; i++)
            {
                if(buf[i] != '\n')
                {                     
                    rawMessage += buf[i];
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
                }
                else
                {
                    //rawMessage.clear()
                }

            }
        }
    }
}
bool GRConnection::getDataThr(device_t* device)
{
    std::thread thr(&GRConnection::getData, this, device);
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
    if(msg.gyro.size() == 3 && msg.acc.size() == 3 && msg.mag.size() == 3)
    {
        sensor->data.push_back(msg);
        
        msg.gyro.clear();
        msg.acc.clear();
        msg.mag.clear();
        return true;
    }
    else
    {
        return false;
    }
    
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
