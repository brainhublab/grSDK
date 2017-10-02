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

#include <thread>
#include <atomic>

// gnuplot
#include <GNUPlot.h>
// 

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
    gr_alg_message alg_msg;
    //device.address = "98:D3:32:10:AC:59";
    std::map<int, device_t> devices;
   conn.getAvalibleDevices();
   conn.setActiveDevice(1);
   conn.connectSocket(1);

   GRAlgorithm alg;
   alg.setupMadgwick(140, 140, 140, 140, 140, 180); //need to check

   //
   std::unordered_map<std::string, gr_message> data;
   FILE* f, *fa;
   f = fopen("firs.txt", "w");
   fa = fopen("firs_acc.txt", "w");
   std::vector<double> trajectory, acc;
   double time;
   GRTrajectory traj;
  int itr = 0;

/*std::vector<std::string> axes;
axes.push_back("Z");
axes.push_back("-Z");
axes.push_back("X");
axes.push_back("Y");
std::string line;


  conn.getData(1, &msg);
  
  std::cout << "Got data!\n";
for( int j = 0; j < 4; j++)
{
    std::atomic<bool> interrupted;


    interrupted.store(false);
    conn.getData(1, &msg);
    //``std::cout << "Giot data" << std::endl;
    std::thread th([&]() {
        while( !interrupted )
        {
         //   std::cout << "reading data\n";
            conn.getData(1, &msg);
          //  std::cout << "got data!\n";
        }
    });
     
    std::cout << "Set device for " << axes[j] << ", press ENTER when you are ready."<<std::endl;
    while( cin.get() != '\n' );
    std::cout << "Ok, let`s break thread!\n";
    interrupted.store(true);
    th.join(); // wait for ending of thread
    
    std::cout << "Ok, let`s read data!" << std::endl; 
    conn.getData(1, &msg);
      
// convert to m/s/s and push
    traj.calibrateGravityMatrix(msg.palm.acc, j);
    std::cout<<"The - "<<axes[j]<<" is stored in calibration matrix"<<std::endl;
    

    msg.clear();
}
*/

/*    initscr();
    cbreak();
    noecho();
    nodelay(stdscr,TRUE);
//int j;
std::vector<std::string> axes;
axes.push_back("Z");
axes.push_back("-Z");
axes.push_back("X");
axes.push_back("Y");
std::string line;

int j=0;
//for( int j = 0; j < 4; j++)
//{
 
    std::cout << "Set device for " << axes[j] << ", press ENTER when you are ready."<<std::endl;
    while(ch != 's' && j<4)
    {
      ch=getch();
      if(ch == 'n')
      {
          conn.getData(1, &msg);
  // convert to m/s/s and push
        traj.calibrateGravityMatrix(msg.palm.acc, j);
        std::cout<<"The - "<<axes[j]<<" is stored in calibration matrix"<<std::endl;
        j++;
      }
      else
      {
       conn.getData(1, &msg);
        //msg.clear();
      }

    }
//  }

//}*/
//traj.calculateCorrectionMatrix();


// gnuplot
//
//
//
std::vector<std::string> plot_script;
plot_script.push_back("set terminal x11");
plot_script.push_back("reset");
plot_script.push_back("plot sin(x)");

GNUPlot plotter;
plotter.open();
plotter.execute(plot_script);

//getchar();
//
//
//


    while(1)
    {
        
        std::cout << "Getting data..\n";
         conn.getData(1, &msg);
 //        std::cout << "Got data!\n";
         if(!msg.imus["palm"]->acc.empty() && itr > 10)
         {
             std::cout<<"data -->";
            /* for(int i=0; i<3; i++)
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
            std::cout<<"QUANTERNION---->";
            for(int i =0;i<4;i++)
            {
                std::cout<<alg_msg.palm[i];
            }
            std::cout<<std::endl;
              trajectory = traj.getNewPosByIntegrating(msg.palm.acc, alg_msg.palm, msg.palm.time_stamp);

              printf( "%s %f %f %f \n","trajectory in main:-> ", trajectory[0], trajectory[1], trajectory[2]); 
           //   std::cout<<msg.palm.acc[0]<<" "<<msg.palm.acc[1]<<" "<<msg.palm.acc[2]<<std::endl;
              fprintf(f, "%f %f %f \n", trajectory[0], trajectory[1], trajectory[2]); 
              fprintf(fa, "%f %f %f \n", msg.palm.acc[0], msg.palm.acc[1], msg.palm.acc[2]); 
              std::cout<<std::endl;


              plot_script.clear();
              plot_script.push_back("replot cos(x)");
              plotter.execute(plot_script);
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
getchar(); // press key for exiting plotter
plotter.write("exit");
plotter.flush();
plotter.close();
  return 0;
}
