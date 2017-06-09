#include "grConnection.h"
#include <sys/socket.h>
//constructor
GRConnection::GRConnection()
{
    rfcommPath = "/dev/rfcomm0";
    setUpRfcomm(rfcommPath);
}


//destructor
GRConnection::~GRConnection()
{
    close(portDescriptor);
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

/*
//TODO add mor rfcomm devices and need to bee imlemented later
bool GRConnection::connect(std::string serial_dev, std::string addr, std::string  chanel)
{
    //temporary
    int status;
    std::string command;

    command += "sudo rfcomm bind ";
    command += " ";
    command += serial_dev;
    command += " ";
    command += addr;
    command += " ";
    command += chanel;

    status = system(command.c_str());
    if(status != 0)
    {
        std::cout << "GRConnection::connect failes to run command with status " << status << std::endl;
        return false;
    }

    sleep(3);

    std::cout << "succssesfuly bind device: " << addr << std::endl;
    return true;
}


bool GRConnection::release(std::string serial_dev, std::string addr, std::string chanel)
{
    int status;
    std::string command;

    command += "sudo rfcomm release ";
    command += " ";
    command += serial_dev;
    command += " ";
    command += addr;
    command += " ";
    command += chanel;

    status = system(command.c_str());
    if(status != 0)
    {
        std::cout << "GRConnection::release failed to execute command with status " << status << std::endl;
        return false;
    }

    sleep(3);

    std::cout<<"succssesfuly release device: "<<addr<<std::endl;
    return true;
}
//TODO
*/
bool GRConnection::getDataThrS(device_t* device)
{

}
bool GRConnection::getDataS(device_t* device)
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

                        splitDataN(rawMessage, &device->pinky);
                        device->pinky.data.back().timestamp = 1;//TODO add if statement
                    }
                    else if(id ==1)
                    {

                        splitDataN(rawMessage, &device->ring);
                        device->ring.data.back().timestamp = 1;
                    }
                    else if(id == 2)
                    {

                        splitDataN(rawMessage, &device->middle);
                        device->middle.data.back().timestamp = 1;
                    }
                    else if(id == 3)
                    {

                        splitDataN(rawMessage, &device->index);
                        device->index.data.back().timestamp = 1;
                    }
                    else if(id==4)
                    {

                        splitDataN(rawMessage, &device->thumb);
                        device->thumb.data.back().timestamp = 1;
                    }
                    else if(id==5)
                    {


                        splitDataN(rawMessage, &(device->palm));
                        device->palm.data.back().timestamp = 1;

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
    std::thread thr(&GRConnection::getDataS, this, device);
    std::thread::id thrId;

    thrId = thr.get_id();
    thr.detach();

    std::cout<<"connection thread is running"<<std::endl;

    return true;
}

/*
bool GRConnection::getData(device_t* device)
{
    int id, i = 0;
    std::string msg;
    bool f0, f1, f2, f3, f4, f5;
    f0 = f1 = f2 = f3 = f4 = f5 = false;
    while(i<6)//!(f0==true && f1==true && f2==true && f3==true && f4==true && f5==true))
    {
       // std::cout<<"in read while"<<std::endl;
        msg = getNext();
        std::stringstream ss(msg);

        ss >> id;
        if(id == 0)
        {
        
                    splitData(msg, &device->pinky);//TODO add if statement
        }
        else if(id ==1)
        {
        
                    splitData(msg, &device->ring);
        }
        else if(id == 2)
        {
        
                    splitData(msg, &device->middle);
        }
        else if(id == 3)
        {
        
                    splitData(msg, &device->index);
        }
        else if(id==4)
        {
        
                    splitData(msg, &device->thumb);
        }
        else if(id==5)
        {
        
                
                    splitData(msg, &(device->palm));
        }
*/
/*
        switch(id)
        {
            case 0:
                if(!f0)
                {
                    splitData(msg, &device->pinky);//TODO add if statement
                    f0 = true;
                }
                break;
            case 1:
                if(!f1)
                {
                    splitData(msg, &device->ring);
                    f1 = true;
                }
                break;
            case 2:
                if(!f2)
                {
                    splitData(msg, &device->middle);
                    f2 = true;
                }
                break;
            case 3:
                if(!f3)
                {
                    splitData(msg, &device->index);
                    f3 = true;
                }
                break;
            case 4:
                if(!f4)
                {
                    splitData(msg, &device->thumb);
                    f4 = true;
                }
                break;
            case 5:
                if(!f5)
                {
                    splitData(msg, &(device->palm));
                    //std::cout<<msg;
                    f5 = true;
                }
                break;
        }
*/
/*
        msg.clear();
        i++;
    }
//    std::cout<<"reading is OK"<<std::endl;

    return true;
}

*/
//private helper methods
bool GRConnection::setUpRfcomm(std::string src)
{
    this->portDescriptor = openPort(src);
    if(portDescriptor < 0)
    {
        return false;
    }

    if(!setTerm())
    {
        return false;
    }

    return true;
}


bool GRConnection::setTerm()
{
    struct termios tOptions;

    // check if fd points to TTY device
    if(!isatty(portDescriptor))
    {
        std::cout << "GRConnection::setTerm fd does not point to a TTY" << std::endl;
        return false;
    }

    // get current configuration of fd
    if(tcgetattr(portDescriptor, &tOptions) < 0)
    {
        std::cout << "GRConnection::setTerm failed to get default config of fd" << std::endl;
        return false;
    }

    // set correct baud rates for input and output
    if(cfsetispeed(&tOptions, B115200) < 0 || cfsetospeed(&tOptions, B115200) < 0)
    {
        std::cout << "GRConnection::setTerm failed to set baud rates" << std::endl;
        return false;
    }

    // apply new config to fd
    if(tcsetattr(portDescriptor, TCSAFLUSH, &tOptions) < 0)
    {
        std::cout << "GRConnection::setTerm failed to apply new config" << std::endl;
        return false;
    }

    return true;
}


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


int GRConnection::openPort(std::string path)
{
    int fd;

    fd = open(path.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd == -1)
    {
        std::cout << "GRConnection::openPort error openning " << path << std::endl;
        return -1;
    }

    if(fcntl(fd, F_SETFL, 0) < 0)
    {
        std::cout << "GRConnection::openPort fcntl failed" << std::endl;
        return -1;
    }

    return fd;
}


/*bool GRConnection::splitData(std::string data, imu* sensor)
{
    int i = 0;
    
    float n, arr[10];
    std::stringstream ss(data);
    std::vector<float> gyro, acc, mag;

    while(ss >> n && i<10)
    {
        arr[i] = n;
        i++;
    }

    for(int i=1;i<10;i++)
    {
        if(i <= 3)
        {
            gyro.push_back(arr[i]);
        }
        else if(i > 3 && i <= 6)
        {
            acc.push_back(arr[i]);
        }
        else if(i > 6 )
        {
            mag.push_back(arr[i]);
        }
    }
    if(gyro.size() == 3 && acc.size() == 3 && mag.size() == 3)
    {
        sensor->gyro.push_back(gyro);
        sensor->acc.push_back(acc);
        sensor->mag.push_back(mag);
        return true;
    }
    else
    {
        return false;
    }
    gyro.clear();
    acc.clear();
    mag.clear();
    
}
*/
bool GRConnection::splitDataN(std::string data, imu* sensor)
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


