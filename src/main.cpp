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
    GRConnection conn;
    device_t* device;
    gr_message msg;
    gr_alg_message alg_msg;
    //device.address = "98:D3:32:10:AC:59";
    std::map<int, device_t> devices;
    devices = conn.getAvalibleDevices();
    int devId;
    for(std::map<int, device_t>::iterator it=devices.begin(); it!=devices.end(); it++)
    {
        if(it->second.name == "GR[R]")
        {
            devId = it->first;
        }
    }

    std::cout<<devId<<"DEV ID"<<std::endl;
    conn.setActiveDevice(devId);
    conn.connectSocket(devId);
    GRAlgorithm alg;
    alg.setupMadgwick(140, 140, 140, 140, 140, 220); //need to check

    GRGrt grt;

    std::unordered_map<std::string, gr_message> data;
    FILE* f, *fa;
    f = fopen("firs.txt", "w");
    fa = fopen("firs_acc.txt", "w");
    std::vector<double> trajectory, acc;
    double time;
    GRTrajectory traj;
    int itr = 0;
    
    int ch;
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr,TRUE);
    
    grt.setDatasetProperties("testDataset", "test n text", "test", 7);
    while(ch!='q')
    {   
       // endwin();
        ch=getch();

        if(ch== 'r')
        {
            //std::cout<<"saving"<<std::endl;
            clrtoeol(); 
            mvprintw(0, 0, "saving");
            
            while(ch != 's' && conn.getData(devId, &msg))
            {

                //std::cout<<"reading"<<std::endl;
                clrtoeol();
                mvprintw(0, 0, "reading");
                
                //endwin();
                ch=getch();    
                if(!msg.imus["palm"]->acc.empty() && itr > 10)
                {
                    alg.madgwickUpdate(&msg, &alg_msg, 1, "flag");
                    trajectory = traj.getNewPosByRunge(msg.palm.acc, alg_msg.palm, msg.palm.time_stamp);
                    grt.addSample(&trajectory, &alg_msg.palm);

                    //      printf( "%s %f %f %f \n","trjectory", trajectory[0], trajectory[1], trajectory[2]);
                    fprintf(f, "%f %f %f %f %f %f %f \n", trajectory[0], trajectory[1], trajectory[2],
                            alg_msg.palm[0], alg_msg.palm[1], alg_msg.palm[2], alg_msg.palm[3]);
                    fprintf(fa, "%f %f %f \n", msg.palm.acc[0], msg.palm.acc[1], msg.palm.acc[2]);
                }
                msg.palm.gyro.clear();
                msg.palm.acc.clear();
                msg.palm.mag.clear();

                alg_msg.clear();

                itr ++;
            }
            clrtoeol();
            mvprintw(0, 0, "saving");
            grt.pushGesture();
        }
        else if(ch == 'n')
        {
            grt.setNextLabel();
            // std::cout<<"AALLAAAAARM NEXT LABEL"<<std::endl;
            clrtoeol();
            mvprintw(0, 0, " NEXT LABEL");
        }
        

    }
    grt.saveDataset();

    endwin();
    return 0;
}
