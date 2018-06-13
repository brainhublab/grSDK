#include "GRT/GRT.h"
#include "grAlgorithm.h"
#include "grDevManager.h"
#include "grConnection.h"
#include "grDevice.h"
#include "grTrajectory.h"

#include <iomanip>
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
#include <sys/ioctl.h>
#include <cmath>
//using namespace GRT;
//using namespace std;
int buttonPressed()
{
    int i;
    ioctl(0, FIONREAD, &i);
    return i; //return a count of avalible cahracters
}

bool grPrint(std::string stringToPrint)
{
    system("stty cooked");
    std::cout<<stringToPrint<<std::endl;
    system("stty raw -echo");
}

double changePrecision(double inp)
{

    return  round(inp*10000)/10000;
}
int main (int argc, const char * argv[])
{
    GRDevManager devManager;
    GRConnection* devConn;
    GRDevice* device;
    GRMessage msg;
    GRAlgMessage alg_msg;

    std::vector<GRDevice> devices;
//    devices = devManager.getAvalibleDevices();
    int devId=-1;
    int i = 0;
/*    for(std::vector<GRDevice>::iterator it=devices.begin(); it!=devices.end(); i++, it++)
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
*/
    GRAlgorithm alg;
    alg.setupMadgwick(280, 280, 280, 280, 280, 220); //need to check

    std::unordered_map<std::string, GRMessage> data;

    int ch;

    FILE* f, *fa;
    f = fopen("firs.txt", "w");
    fa = fopen("firs_acc.txt", "w");
    std::vector<double> trajectory, acc;
    double time;
    GRTrajectory traj;
    int itr = 0;


    GRGrt grt;

    grt.setAlgorithms("MLP_C", false);
    std::vector<double> accelerations;
    std::vector<double> rotations;
    grt.setDatasetProperties("_trainingData", "first", "_trainingdata", 26);
    std::unordered_map<std::string, std::vector<double> > nodeRotationsMain;
    while(ch != 'q')
    {
        if(buttonPressed())
        {
            ch=getchar();
        }
        if(ch == 'r')
        {
            grPrint("saving");   
            while(ch != 's' && devConn->getData(&msg))
            {
                grPrint("reading");
                if(buttonPressed())
                {
                    ch = getchar();
                }

                if(!msg.empty() && itr > 10)
                {

                    system("stty cooked");
                    msg.print(); 
                    system("stty raw");

                    if( alg.madgwickUpdate(&msg, &alg_msg) && !alg_msg.empty())
                    {
                        //for(std::unordered_map<std::string, GRImu* >::iterator it=msg.imus.begin(); it!=msg.imus.end(); ++it)
                        for(auto& imu : msg.imus)
                        {
                            for(int i=0;i<3;i++) 
                            {
                                accelerations.push_back(imu.second->acc[i]);
                                system("stty cooked");
                                std::cout<<imu.second->acc[i]<<" ";
                            }  
                            std::cout<<"accs"<<std::endl;

                        }
                        nodeRotationsMain = alg.getEulerRotations(alg_msg);

                        //for(std::unordered_map<std::string, std::vector<double> >::iterator it=nodeRotationsMain.begin(); it!=nodeRotationsMain.end(); ++it)                    
                        for(auto& node : nodeRotationsMain)
                        {
                            if(node.first == "palm")
                            {
                                for(int i=0;i<3;i++)
                                {
                                    rotations.push_back((57.29578 * node.second[i]));
                                }
                            }
                            else
                            {
                                rotations.push_back((57.20578 * node.second[2]));
                            }

                            std::cout<<changePrecision(changePrecision(node.second[2]))<<" ";
                            std::cout<<"rotations"<<std::endl;

                        }
                        system("stty raw");

                        grt.addSample(&accelerations, &rotations);
                        //need new connection to continue
                        accelerations.clear();
                        rotations.clear();
                    }
                }

                nodeRotationsMain.clear();

                msg.clear();
                alg_msg.clear();
                itr++;

            }
            grPrint("pushing gasture");
            grt.pushGesture();
        }
        else if(ch == 'n')
        {
            grPrint("NEXT GESTURE");
            ch = ' ';
            grt.setNextLabel();

        }
        else if(ch == 't')
        {
            system("stty cooked echo");
            //                grPrint("training");
            std::cout<<"-----------------------------------------------------training"<<std::endl;
            grt.loadTrainingData("../trainingData/grtrainingMLP_C_trainingdata.grt");

            grt.prepare();
            grt.setTestDataFromTraining(20);

            grt.train();
            grt.saveModel("../trainingData/MLP_C_model.grt");
            grt.test();

            // system("stty raw");
            return 0;
        }
        else if(ch == t)
        {
        
            grt.loadModel("../trainingData/MLP_C_model.grt");

        }



    } 

    system("stty cooked echo");
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

    grt.saveDataset();

    return 0;

}

