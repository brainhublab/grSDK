#include "GRT/GRT.h"
#include "grAlgorithm.h"
#include "grDevManager.h"
#include "grDataStructs.h"
#include "grDevice.h"
#include "grTrajectory.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <map>
#include <unordered_map>
#include <ncurses.h>
#include <memory>

#include "inetclientstream.hpp"
//using namespace GRT;
//using namespace std;


int main (int argc, const char * argv[])
{

    GRDevManager devManager;
    GRDevice* device;
    GRMessage msg;
    GRAlgMessage alg_msg;

    std::unordered_map<int, GRDevice>* devices;
    int devId=-1;
    int i = 0;

    devManager._getApClients();
    devices = devManager.getAvalibleDevices();
   for(auto & dev : devManager._avalibleDevices)
   {
       std::cout<<"inMain"<<dev.second.name<<std::endl;
       dev.second.getData(&msg);

   } 
/*    libsocket::inet_stream sock ("192.168.12.4", "23", LIBSOCKET_IPv4);
    libsocket::inet_stream  sock1(std::move(sock));
    
    std::string res;
    res.resize(20);

  */  
    /*
    while(res.size()>0)
    {
        *sock1>>res;
        std::cout<<res;
    }
    for(std::vector<GRDevice>::iterator it=devices.begin(); it!=devices.end(); i++, it++)
      {
      if(it->name == "GR[R]")
      {
    //    std::cout<<it->first<<" in iteration---------------------------------------------"<<std::endl;
    devId = i;
    }
    }

    if(devId == -1)
    {
    std::cout << "Device not found" << std::endl;
    return 0;
    }

    std::cout<<"devId: "<<devId<<std::endl;
    //    devConn = devManager.setActiveDevice(devId);
    //   devConn->connectSocket();

    GRAlgorithm alg;
    alg.setupMadgwick(140, 140, 140, 140, 140, 220); //need to check

    std::unordered_map<std::string, GRMessage> data;
    */
    return 0;

}

