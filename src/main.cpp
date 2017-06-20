#include "GRT/GRT.h"
#include "grAlgorithm.h"
#include "grConnection.h"
#include "grDevice.h"

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <map>
#include <unordered_map>
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
    device_t* device;
    //device.address = "98:D3:32:10:AC:59";
    std::map<int, device_t> devices;
   conn.getAvalibleDevices();
   conn.setActiveDevice(1);
   conn.connectSocket(1);
   //
   std::unordered_map<std::string, gr_message> data;
  while(1)
    {

         data = conn.getData(1, "ITER");
         if(!data.empty())
             std::cout<<data["palm"].gyro[1]<<std::endl;
        //conn.readData(1);
       
    }
  return 0;
}
