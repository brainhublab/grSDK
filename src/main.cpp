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
//using namespace GRT;
//using namespace std;


int main (int argc, const char * argv[])
{

   GRDevManager devManager;
    GRDevice* device;
    GRMessage msg;
    GRAlgMessage alg_msg;

    std::vector<GRDevice> devices;
    devManager.getAvalibleDevices();
    devManager._getApClients();
    int devId=-1;
    int i = 0;
    /*for(std::vector<GRDevice>::iterator it=devices.begin(); it!=devices.end(); i++, it++)
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

