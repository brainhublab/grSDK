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

bool GRConnection::connect(std::string addr, std::string  chanel) //TODO add mor rfcomm devices and need to bee imlemented later
{
    std::string command;
    command += "sudo rfcomm bind rfcomm0";
    command += " ";
    command += addr;
    command += " ";
    command += chanel;
    system(command.c_str());
    std::cout<<"succssesfuly bind device: "<<addr<<std::endl;
   //temporary
}

bool GRConnection::readData()
{
    float id;
    setUpRfcomm("/dev/rfcomm0");
    std::string str;
    //str = getNext();
    std::cout<<"before while"<<std::endl;
    while(true)
    {
        str = getNext();
        std::stringstream ss(str);
        ss >> id;
        //std::cout<<"ID :"<<id<<std::endl;
        if(id == 0)
        {
            //std::cout<<"in IF"<<std::endl;
            splitData(str, &device.pinky);
            //std::cout<<device.pinky.gyro.front().front()<<std::endl;
        }
        else if(id == 1)
        {
            splitData(str, &device.ring);
            std::cout<<device.ring.gyro.front().front()<<std::endl;

        }
        else if(id == 2)
        {
            splitData(str, &device.middle);
        }
        else if(id == 3)
        {
            splitData(str, &device.index);
        }
        else if(id == 4)
        {
            splitData(str, &device.thumb);
        }
        else if(id = 5)
        {
            splitData(str, &device.palm);
        }
        
    }
         
       
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

    tOptions.c_cflag &= ~PARENB;
    tOptions.c_cflag &= ~CSIZE;

    tOptions.c_cc[VTIME] = 200;
    //tOptions.c_cc[VSTOP] = '\n';

    tcsetattr(portDescriptor, TCSANOW, &tOptions);
    tcflush(portDescriptor, TCIFLUSH);

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
        }
        else
        {
            //std::cout<<"\n got data";
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
    for(int i=0;i<9;i++)
    {
        if(i < 3)
        {
            gyro.push_back(arr[i]);
            //std::cout<<arr[i];
        }
        else if(i > 2 && i < 6)
        {
            acc.push_back(arr[i]);
        }
        else if(i > 5 )
        {
            mag.push_back(arr[i]);
        }
    }
    sensor->gyro.push_back(gyro);
    sensor->acc.push_back(acc);
    sensor->mag.push_back(mag);
    return true;    
}


