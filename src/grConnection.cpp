#include <grConnection.h>

//constructor
GRConnection::GRConnection()
{

}
//destructor
~GRConnection::GRConnection()
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

vector<device_t> GRConnection::getAvalibleDevices()//TODO need to be implemented later
{

}

int GRConnection::getDeviceId()//TODO need to be implemented later
{

}

bool connect(std::string addr, std::string  chanel) //TODO add mor rfcomm devices and need to bee imlemented later
{
    string command;
    command += "sudo rfcomm bind rfcomm0";
    command += " ";
    command += addr;
    command += " ";
    command += chanel;
    system(command);
    std::cout<<"succssesfuly bind device: "<<add<<std::endl 
}

bool readData()
{

}
