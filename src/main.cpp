#include "GRT/GRT.h"
#include "grAlgorithm.h"
#include "grConnection.h"
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
    gr_message msg;
    alg_gr_message alg_msg;
    //device.address = "98:D3:32:10:AC:59";
    std::map<int, device_t> devices;
   conn.getAvalibleDevices();
   conn.setActiveDevice(1);
   conn.connectSocket(1);

   GRAlgorithm alg;
   alg.setupMadgwick(165, 165, 165, 165, 165, 165); //need to check

   //
   std::unordered_map<std::string, gr_message> data;
   FILE* f;
   f = fopen("firs.txt", "w");
   std::vector<float> trajectory, acc;
   float time;
   GRTrajectory traj;

  while(1)
    {

         conn.getData(1, &msg);
         if(!msg.imus["palm"]->acc.empty())
         {
            /* std::cout<<"data -->";
             for(int i=0; i<3; i++)
             {
                 std::cout<<msg.imus["pinky"]->gyro[i]<<" ";
                
             }
              for(int i=0; i<3; i++)
             {
                 std::cout<<msg.imus["pinky"]->acc[i]<<" ";
                
             }
              for(int i=0; i<3; i++)
             {
                 std::cout<<msg.imus["pinky"]->mag[i]<<" ";
                
             }
              std::cout<<msg.imus["pinky"]->time_stamp;
*/              
             alg.madgwickUpdate(&msg, &alg_msg, 1, "flag");

              trajectory = traj.getNewPosByVelocity(msg.palm.acc, msg.palm.time_stamp);
              std::cout<<msg.palm.acc[0]<<" "<<msg.palm.acc[1]<<" "<<msg.palm.acc[2]<<std::endl;
              fprintf(f, "%f %f %f \n", trajectory[0], trajectory[1], trajectory[2]); 
              std::cout<<std::endl;
         }
         msg.palm.gyro.clear();
         msg.palm.acc.clear();
         msg.palm.mag.clear();

         /*
            if(!data.empty())
            {
            acc = data["palm"].acc;
            time =  data["palm"].time_stamp;
            trajectory = traj.getNewPosByVelocity(acc,time);
            std::cout<<acc[0]<<" "<<acc[1]<<" "<<acc[2]<<std::endl;
            fprintf(f, "%f %f %f \n", trajectory[0], trajectory[1], trajectory[2]); 
            }
            */

         //conn.readData(1);

    }
  return 0;
}
