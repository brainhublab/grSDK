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
    GRConnection conn;
    device_t* device;
    gr_message msg;
    gr_alg_message alg_msg;
    //device.address = "98:D3:32:10:AC:59";
    std::map<int, device_t> devices;
    conn.getAvalibleDevices();
    conn.setActiveDevice(1);
    conn.connectSocket(1);

    GRAlgorithm alg;
    alg.setupMadgwick(140, 140, 140, 140, 140, 180); //need to check
    
    acc_k_vars k_vars;
    alg.setUpKfilter(&conn, &k_vars, 1);

    //
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

        //      std::cout << "Getting data..\n";
        conn.getData(1, &msg);
      alg.kFilterStep(&msg, &k_vars);
        //        std::cout << "Got data!\n";
        if(!msg.imus["palm"]->acc.empty() && itr > 10)
        {
            //       std::cout<<"data -->";

            alg.madgwickUpdate(&msg, &alg_msg, 1, "flag");
            //        std::cout<<"QUANTERNION---->";
            /*   for(int i =0;i<4;i++)
                 {
                 std::cout<<alg_msg.palm[i];
                 }

                 std::cout<<std::endl;
                 */  
            trajectory = traj.getNewPosByIntegrating(msg.palm.acc, alg_msg.palm, msg.palm.time_stamp);

      //      printf( "%s %f %f %f \n","trjectory", trajectory[0], trajectory[1], trajectory[2]); 
            //   std::cout<<msg.palm.acc[0]<<" "<<msg.palm.acc[1]<<" "<<msg.palm.acc[2]<<std::endl;
            fprintf(f, "%f %f %f \n", trajectory[0], trajectory[1], trajectory[2]); 
            fprintf(fa, "%f %f %f \n", msg.palm.acc[0], msg.palm.acc[1], msg.palm.acc[2]); 
            //    std::cout<<std::endl;
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
        itr ++;

    }

    return 0;
}
