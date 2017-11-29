#include "GRT/GRT.h"
#include "grAlgorithm.h"
#include "grDevManager.h"
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
#include <ncurses.h>
//using namespace GRT;
//using namespace std;


int main (int argc, const char * argv[])
{

    //Gnuplot gp;
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
    GRDevManager devManager;
    GRConnection* devConn;
    device_t* device;
    gr_message msg;
    gr_alg_message alg_msg;

    std::unordered_map<int, device_t> devices;
    devices = devManager.getAvalibleDevices();
    int devId=-1;
    for(std::unordered_map<int, device_t>::iterator it=devices.begin(); it!=devices.end(); it++)
    {
        if(it->second.name == "GR[R]")
        {
            //    std::cout<<it->first<<" in iteration---------------------------------------------"<<std::endl;
            devId = it->first;
        }
    }

    if(devId == -1)
    {
        std::cout << "Device not found" << std::endl;
        return 0;
    }

    std::cout<<"devId: "<<devId<<std::endl;
    devConn = devManager.setActiveDevice(devId);
    devConn->connectSocket();

    GRAlgorithm alg;
    alg.setupMadgwick(140, 140, 140, 140, 140, 220); //need to check

    std::unordered_map<std::string, gr_message> data;

    FILE* f, *fa;
    f = fopen("firs.txt", "w");
    fa = fopen("firs_acc.txt", "w");
    std::vector<double> trajectory, acc;
    double time;
    GRTrajectory traj;
    int itr = 0;

    while(1)
    {
        //std::cout << "Getting data..\n";
        devConn->getData(&msg);

        if(!msg.imus["palm"]->acc.empty() && itr > 10)
        {
            alg.madgwickUpdate(&msg, &alg_msg, 1, "flag");
            //        std::cout<<"QUANTERNION---->";
            /*   for(int i =0;i<4;i++)
                 {
                 std::cout<<alg_msg.palm[i];
                 }

                 std::cout<<std::endl;
                 */
            //trajectory = traj.getNewPosByRunge(msg.palm.acc, alg_msg.palm, msg.palm.time_stamp);
            trajectory = traj.getAccelerations(msg.palm.acc, alg_msg.palm);

            //      printf( "%s %f %f %f \n","trjectory", trajectory[0], trajectory[1], trajectory[2]);
            std::cout<<msg.palm.acc[0]<<" "<<msg.palm.acc[1]<<" "<<msg.palm.acc[2]<<"check conn in main"<<std::endl;
            printf("writing...\n");
            //fprintf(f, "%f %f %f %f %f %f\n", trajectory[0], trajectory[1], trajectory[2], msg.palm.gyro[0], msg.palm.gyro[1], msg.palm.gyro[2]);
            //fprintf(fa, "%f %f %f \n", msg.palm.acc[0], msg.palm.acc[1], msg.palm.acc[2]);
            //    std::cout<<std::endl;
        }
        msg.palm.gyro.clear();
        msg.palm.acc.clear();
        msg.palm.mag.clear();

        alg_msg.clear();
        itr ++;

    }

    return 0;

}

