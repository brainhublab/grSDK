#include "grDevManager.h"
//constructor
GRDevManager::GRDevManager()
{
    _reqAttrStr = "ga\n";
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
std::unordered_map<int, GRDevice>* GRDevManager::getAvalibleDevices()
{
    std::vector<std::string> attr;
    std::string ans;
    ans.resize(32);
    for(auto const& client : this->_getApClients())
    {
        try{

            libsocket::inet_stream sk(client.first, "23", LIBSOCKET_IPv4);
            sk<<_reqAttrStr;
            sk>>ans;
            attr = _requestDevAttr(&ans);
            if((attr[0]!="GR[L]") || (attr[0] != "GR[R]"))
            {
                std::cout<<attr[0];
                std::cerr<<"ERROR: Name of the device is wrong"<<std::endl;
                //TODO return something 
            }
            int devId=0;
            if((devId=_deviceIsIn(client.second))!= -1)
            {
                std::cout<<attr[0]<<std::endl;
               // GRDevice newDev(attr[0], client.second, client.first, devId);
                this->_avalibleDevices[devId]= std::move(GRDevice(attr[0], client.second, client.first, devId));

            }
            sk.destroy();


        }catch (const libsocket::socket_exception& exc)
        {
            std::cerr<<exc.mesg;
        }
    }
    std::cout<<"DONEDONE"<<std::endl;

    std::string asd = _exec("create_ap --list-running");
    std::cout<<asd;
    return &_avalibleDevices;
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
int GRDevManager::_deviceIsIn(std::string hwAddr)
{
    for(auto const& dev : _avalibleDevices )
    {
        if(dev.second.hwAddr == hwAddr)
        {
            return -1;
        }
        else
        {
            return this->_avalibleDevices.size() + 1;
        }
    }
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
    return clientsOut;
}


std::vector<std::string> GRDevManager::_requestDevAttr(std::string* resp)
{
    const std::regex re("\\s+");
    return std::vector<std::string> {
        std::regex_token_iterator<std::string::iterator>(resp->begin(),
                resp->end(),
                re,
                -1),{}
    };
}


