#include "grDevManager.h"
//constructor
GRDevManager::GRDevManager()
{
}

//destructor
GRDevManager::~GRDevManager()
{
}

//copy constructor
GRDevManager::GRDevManager(const GRDevManager& t)
{
}

//operator =
GRDevManager& GRDevManager::operator=(const GRDevManager& t)
{
}

/* Return map of avalible for connecting GR devices */
std::vector<GRDevice> GRDevManager::getAvalibleDevices()
{

    std::string asd = _exec("create_ap --list-running");
    std::cout<<asd;
    //return _avalibleDevices;
}

/* Set GR device from avalible to active and make it ready for connection */
/*GRConnection* GRDevManager::setActiveDevice(int devId)
{
    return &this->_activeDevices[devId];
}
*/
//TODO needs to be implemented later
/* Return device by ID */
/*GRConnection* GRDevManager::getActiveDeviceById(int id)
{
}
*/
/* Check id device in avalible device */
bool GRDevManager::_deviceIsIn(std::string addr)
{
 /*   int i=1;

    while(i <= _avalibleDevices.size())
    {
        if(addr != _avalibleDevices[i].hwAddress)
        {
            i++;
        }
        else if(addr == _avalibleDevices[i].hwAddress)
        {
            return true;
        }
    }
*/
    return false;
}

std::string GRDevManager::_exec(const char* cmd)
{
    std::array<char, 128> buffer;
    std::string res;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);

    if(!pipe)
    {
        throw std::runtime_error("ERROR: popen() failed");
    }
    while(fgets(buffer.data(), buffer.size(), pipe.get()) != NULL)
    {
        res += buffer.data();
    }
    return res;
}

std::string GRDevManager::_getApIface()
{

    std::string apIface;
    std::string cmdOut = _exec("create_ap --list-running");
    const std::regex re("\\(gr-ap[^]+\\)"); //TODO make regex with iface recognition \(id[^]+\)
    std::smatch m;
    if(std::regex_search(cmdOut, m, re))
    {
        for(auto iface : m)
        {
            apIface = iface;
        }
        apIface.erase( std::remove_if(apIface.begin(), apIface.end(),
                    [](char ch){ return (ch=='(' || ch==')') ? true: false; }),
                apIface.end() );
    }
    else
        apIface = "-1";
    return apIface;

}

std::unordered_map<std::string, std::string> GRDevManager::_getApClients()
{

    std::unordered_map<std::string, std::string> clientsOut;
    std::string clients = this->_exec(("create_ap --list-clients " + this->_getApIface()).c_str());
    std::regex re("\\s+");
    std::regex_token_iterator<std::string::iterator> iter(clients.begin(),
            clients.end(),
            re,
            -1);
    std::regex_token_iterator<std::string::iterator> end;
    while(iter != end)
    {
        clientsOut[*iter] = *iter++;
        std::advance(iter, 2);
    }

    std::string host = "192.168.12.4";
    std::string port = "23";
    std::string answer;

    answer.resize(20);
    
    std::stringstream ss;
    char c;
    char* buf = new char[20];
    try
    {
        libsocket::inet_stream sock(host, port, LIBSOCKET_IPv4);
        long int i=0;
        while(answer.size() > 0)
        {
          sock>>answer;
         std::cout<<answer; 
           // std::cout<<answer<<"";
        }
        std::cout<<"afterqhile"<<std::endl;
        //readInCallback(sock, funca);
        
    }
    catch (const libsocket::socket_exception& exc)
    {
        std::cerr<<exc.mesg;
    }
    return clientsOut;
}

void GRDevManager::funca(std::string s)
{
    std::cout<<s;
}
bool GRDevManager::_requestDevAttr(std::string)
{

}


