#include "GRT/GRT.h"
#include "grAlgorithmNew.h"
#include "grConnection.h"
#include "grDevice.h"

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

//using namespace GRT;
//using namespace std;

int main (int argc, const char * argv[])
{
    /*
    GRAlgorithm grAlg;

    grAlg.loadTrainingData("./data/grTrainingDTWtest.grt");
    grAlg.setTestDataFromTraining(20);

    //grAlg.loadModel("./data/DTWModel.grt");

    grAlg.train();
    grAlg.test();

    grAlg.saveModel("./data/DTWModel.grt");
    cout << grAlg.getTestAccuracy() << endl;
*/
    GRConnection conn;
    device_t device;
    //device.address = "98:D3:32:10:AC:59";
    std::map<int, device_t> devices;
    devices = conn.getAvalibleDevices();
    sleep(4);
    //*device = devices.front();
    std::cout<<"this is size of devices "<<devices.size()<<std::endl; 
    std::cout<<"this is the addres "<<devices[1].address<<std::endl;
    std::cout<<"this is the name "<<devices[1].name<<std::endl;
   /* 
    while(1)
    {
        conn.getData(&device);

        if(!device.pinky.data.empty())
        {
            std::cout<<device.pinky.data.front().gyro.front();
            device.pinky.data.pop_front();
        }
    }*/
    return 0;
}
