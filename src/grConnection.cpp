#include <grConnection.h>

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

//methods
bool GRConnection::findDevices()//TODO later maeby we will implement his funkction in lower layer 
{
   
  
}

std::vector<device_t> GRConnection::getAvalibleDevices()//TODO need to be implemented later
{

}

int GRConnection::getDeviceId(device_t dev)//TODO need to be implemented later
{

}

bool GRConnection::connect(std::string serial_dev, std::string addr, std::string  chanel) //TODO add mor rfcomm devices and need to bee imlemented later
{
    std::string command;
    command += "sudo rfcomm bind ";
    command += " ";
    command += serial_dev;
    command += " ";
    command += addr;
    command += " ";
    command += chanel;
    system(command.c_str());
    sleep(3);
    std::cout<<"succssesfuly bind device: "<<addr<<std::endl;
   //temporary
}
bool GRConnection::release(std::string serial_dev, std::string addr, std::string chanel)
{
    std::string command;
    command += "sudo rfcomm release ";
    command += " ";
    command += serial_dev;
    command += " ";
    command += addr;
    command += " ";
    command += chanel;
    system(command.c_str());
    sleep(3);
    std::cout<<"succssesfuly release device: "<<addr<<std::endl;

}
bool GRConnection::getData(device_t* device)
{
    
    std::thread thr(&GRConnection::connectAndRead, this, device);
    std::thread::id thrId;
    thrId = thr.get_id();
    //std::cout<<thrId<<std::endl;
    thr.detach();
    std::cout<<"connection thread is running"<<std::endl;

    return true;
    
   //  connectAndRead(device);
}
bool GRConnection::connectAndRead(device_t* device)
{
    int id;
    //setUpRfcomm("/dev/rfcomm0");
    std::string msg;
   // msg = getNext();
    std::cout<<"running read while"<<std::endl;
    int i=0;
    while(true)
    {
        //std::cout<<"1"<<std::endl;
        msg = getNext();
        //std::cout<<msg;
       
        std::stringstream ss(msg);
        ss >> id;
        //std::cout<<"ID :"<<id<<std::endl;
        if(id == 0)
        {
            splitData(msg, &device->pinky);
            // std::cout<<msg<<std::endl;
            //std::cout<<"front: " << device->pinky.gyro.back().front()<<std::endl;
        }
        else if(id == 1)
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
        else if(id == 4)
        {
            splitData(msg, &device->thumb);
        }
        else if(id = 5)
        {
            splitData(msg, &(device->palm));
            //std::cout<<"front: " << device->palm.gyro.back().front()<<std::endl;

        }
       msg.clear();
       i++;
    }
       //exit(1); 
       return true;
}
//private helper methods
bool GRConnection::setUpRfcomm(std::string src)
{
    portDescriptor = openPort(src);
    setTerm();
    return portDescriptor != 1;
}
bool GRConnection::setTerm()
{
    struct termios tOptions;
    cfsetispeed(&tOptions, B115200);
    cfsetospeed(&tOptions, B115200);

    //tOptions.c_cflag &= ~PARENB;
    //tOptions.c_cflag &= ~CSIZE;

    //tOptions.c_cc[VTIME] = 100;
    //tOptions.c_cc[VSTOP] = '\n';

    //tcsetattr(portDescriptor, TCSANOW, &tOptions);
    //tcflush(portDescriptor, TCIFLUSH);

}
std::string GRConnection::getNext()
{
    char buf[2];
    std::string res;

    while(true)
    {
        read(portDescriptor, buf, 1);
        if(buf[0]!= '\n')
        {
            res += buf[0];
            //std::cout<<buf[0]<<std::endl;
        }
        else
        {
            //std::cout<<res<<std::endl;
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
        std::cout<<"Error openning: "<<path<<std::endl;

    }
    else
    {
        fcntl(fd, F_SETFL, 0);
    }
    return fd;

}

bool GRConnection::splitData(std::string data, imu* sensor)
{
    int i = 0;
    float n;
    float arr[10];
    std::stringstream ss(data);
    std::vector<float> gyro, acc, mag;
    while(ss >> n)
    {
        arr[i] = n;
        i++; 
    }
    for(int i=1;i<10;i++)
    {
        if(i < 4)
        {
            gyro.push_back(arr[i]);
            //std::cout<<arr[i];
        }
        else if(i > 3 && i < 7)
        {
            acc.push_back(arr[i]);
        }
        else if(i > 6 )
        {
            mag.push_back(arr[i]);
        }
    }
    sensor->gyro.push_back(gyro);
    sensor->acc.push_back(acc);
    sensor->mag.push_back(mag);
    return true;    
}


