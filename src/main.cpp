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

    GRDevManager devManager;
    std::vector<std::string> str;

    std::vector<GRDevice> devs;

    devManager._getAllManagedDevicesPaths();
    str = devManager._allManagedDevicesPaths;
    /*  for(auto &path: str)
        {
        std::cout<<path<<" :-----";
        std::cout<<path.find(defaultAdapterPath)<<std::endl;
        }
        */
    /*
       for(int i=0; i< 5; i++)
       {
       devs =  devManager.getAvalibleDevices();
       std::cout<<devs.size()<<"  <-this is the size of devs"<<std::endl;
       for(int j=0;j<devs.size(); j++)
       {
       std::cout<<devs[j].id<<"]-----"<<j<<std::endl;
       }
       }
       */
    int j=0;

        devs =  devManager.getAvalibleDevices();
        for(int i=0;i<devs.size(); i++)
        {
            std::cout<<devs[i].name<<std::endl;
        }
 for(int i=0;i<devs.size();i++)
    {
        if(devs[i].name == "GR[L]")
        {
            devManager.connect(&devs[i]);
            std::cout<<devs[i].dbusObjecPath<<std::endl;
            //usleep(200);
            //devManager.disconnect(&devs[i]);
            devManager.prepareDataReading(&devs[i]);
            while(true)
            {
                devManager.getData(&devs[i]);
         //       std::cout<<"iteration------------------"<<j++;
            }
        }
    }





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

    /*
       GRDevManager devManager;
       GRConnection* devConn;
       GRDevice* device;
       GRMessage msg;
       GRAlgMessage alg_msg;

       std::vector<GRDevice> devices;
       devices = devManager.getAvalibleDevices();
       int devId=-1;
       int i = 0;
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
    devConn = devManager.setActiveDevice(devId);
    devConn->connectSocket();

    GRAlgorithm alg;
    alg.setupMadgwick(140, 140, 140, 140, 140, 220); //need to check

    std::unordered_map<std::string, GRMessage> data;

    int ch;
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);

    FILE* f, *fa;
    f = fopen("firs.txt", "w");
    fa = fopen("firs_acc.txt", "w");
    std::vector<double> trajectory, acc;
    double time;
    GRTrajectory traj;
    int itr = 0;


    GRGrt grt;

    grt.setAlgorithms("MLP_C", false);
    grt.prepare();
    std::vector<double> accelerations;
    std::vector<double> rotations;
    while(ch != 'q')
    {
    ch = getch();
    if(ch == 'r')
    {   
    clrtoeol();
    mvprintw(0, 0, "saving");
    while(ch != 's' && devConn->getData(&msg))
    {
    clrtoeol();
    mvprintw(0, 0, "reading");

    ch=getch();
    if(!msg.empty() && itr > 10)
    {

    alg.madgwickUpdate(&msg, &alg_msg);
    for(std::unordered_map<std::string, GRImu* >::iterator it=msg.imus.begin(); it!=msg.imus.end(); ++it)
    {
        for(int i=0;i<3;i++) 
        {
            accelerations.push_back(it->second->acc[i]);
        }  
    }

    for(std::unordered_map<std::string, std::vector<double>* >::iterator it=alg_msg.nodes.begin(); it!=alg_msg.nodes.end(); ++it)
    {
        for(int i=0;i<3;i++)
        {
            //TODO  //here need to fill rotations
        }     

    }

    grt.addSample(&accelerations, &rotations);
    //need new connection to continue
}
msg.clear();
alg_msg.clear();
itr++;

}
clrtoeol();
mvprintw(0, 0, "saving");
grt.pushGesture();
}
else if(ch == 'n')
{
    grt.setNextLabel();
    clrtoeol();
    mvprintw(0, 0, "next label");

}
else if(ch == 't')
{
    clrtoeol();
    mvprintw(0, 0, "training and testing");
    grt.loadTrainingData("../trainingData/MLP_C_trainingData.grt");
    grt.setTestDataFromTraining(20);

    grt.train();
    grt.test();

    grt.saveModel("../trainingData/MLP_C_model.grt");

    clrtoeol();
    //            mvprintw(0, 0, std::string(grt.getTestAccuracy()));


}


} 
*/
//std::cout << "Getting data..\n";
/*
   devConn->getData(&msg);

   if(!msg.empty() && itr > 10 )
   {
   alg.madgwickUpdate(&msg, &alg_msg, 1, "flag");
   trajectory = traj.getAccelerations(msg.palm.acc, alg_msg.palm);

//      printf( "%s %f %f %f \n","trjectory", trajectory[0], trajectory[1], trajectory[2]);
std::cout<<msg.palm.acc[0]<<" "<<msg.palm.acc[1]<<" "<<msg.palm.acc[2]<<"check conn in main"<<std::endl;
printf("writing...\n");
//fprintf(f, "%f %f %f %f %f %f\n", trajectory[0], trajectory[1], trajectory[2], msg.palm.gyro[0], msg.palm.gyro[1], msg.palm.gyro[2]);
//fprintf(fa, "%f %f %f \n", msg.palm.acc[0], msg.palm.acc[1], msg.palm.acc[2]);
}
msg.palm.gyro.clear();
msg.palm.acc.clear();
msg.palm.mag.clear();

alg_msg.clear();
itr ++;
*/



return 0;

}

