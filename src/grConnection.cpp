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
bool findDevices()//TODO later maeby we will implement his funkction in lower layer 
{
   
  
}

std::vector<device_t> GRConnection::getAvalibleDevices()//TODO need to be implemented later
{

}

int GRConnection::getDeviceId(device_t dev)//TODO need to be implemented later
{

}

bool connect(std::string addr, std::string  chanel) //TODO add mor rfcomm devices and need to bee imlemented later
{
    std::string command;
    command += "sudo rfcomm bind rfcomm0";
    command += " ";
    command += addr;
    command += " ";
    command += chanel;
    system(command.c_str());
    std::cout<<"succssesfuly bind device: "<<addr<<std::endl; 
}

bool readData()
{

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
            std::cout<<"\n got data";
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



