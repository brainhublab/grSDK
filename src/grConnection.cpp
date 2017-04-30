#include "grConnection.h"

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


//TODO later maybe we will implement his function in lower layer
bool GRConnection::findDevices()
{
}


//TODO need to be implemented later
std::vector<device_t> GRConnection::getAvalibleDevices()
{
}


//TODO needs to be implemented later
int GRConnection::getDeviceId(device_t dev)
{
}


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


bool GRConnection::getData(device_t* device)
{
    std::thread thr(&GRConnection::connectAndRead, this, device);
    std::thread::id thrId;

    thrId = thr.get_id();
    thr.detach();

    std::cout<<"connection thread is running"<<std::endl;

    return true;
}


bool GRConnection::connectAndRead(device_t* device)
{
   // std::cout<<"running read while"<<std::endl;

    int id, i = 0;
    std::string msg;
    bool f0, f1, f2, f3, f4, f5;
    f0 = f1 = f2 = f3 = f4 = f5 = false;
    while(!f5)//!(f0==true && f1==true && f2==true && f3==true && f4==true && f5==true))
    {
       // std::cout<<"in read while"<<std::endl;

     

        msg = getNext();
        std::stringstream ss(msg);

        ss >> id;
//vlad        std::cout<<"ID: "<<" "<<id<<"MSG :"<<msg<<std::endl;

        switch(id)
        {
            case 0:
                if(!f0)
                {
                    splitData(msg, &device->pinky);
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

        msg.clear();
        i++;
    }
//    std::cout<<"reading is OK"<<std::endl;

    return true;
}


//private helper methods
bool GRConnection::setUpRfcomm(std::string src)
{
    portDescriptor = openPort(src);
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


bool GRConnection::splitData(std::string data, imu* sensor)
{
    int i = 0;
    float n, arr[10];
    std::stringstream ss(data);
    std::vector<float> gyro, acc, mag;

    while(ss >> n)
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

    sensor->gyro.push_back(gyro);
    sensor->acc.push_back(acc);
    sensor->mag.push_back(mag);

    return true;
}


