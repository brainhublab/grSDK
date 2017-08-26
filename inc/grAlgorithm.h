#ifndef GR_ALGORITHM
#define GR_ALGORITHM

#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <deque>
#include <math.h>
#include <thread>
#include <numeric>
#include <functional>
#include <algorithm>

#include "GRT/GRT.h"

#include <grConnection.h>
#include <grDevice.h>
#include <grMadgwick.h>
#include <grGrt.h>
using namespace std;
struct k_filter_vars //variables needet from simplified kalman 
{
    double volt;
    double proccess;
    double pc;
    double g;
    double p;
    double xp;
    double zp;
    double xe;

    k_filter_vars()
    {
        volt = 0.0;
        proccess = 0.0;
        pc = 0.0;
        g = 0.0;
        p = 1.0;
        xp = 0.0;
        zp = 0.0;
        xe = 0.0;
    }
};

struct acc_k_vars //arbitary accelerometer vars for kalman
{
    k_filter_vars acc_k_x;
    k_filter_vars acc_k_y;
    k_filter_vars acc_k_z;
};

class GRAlgorithm :public GRGrt
{

    public:
        GRAlgorithm(); //constructor
        ~GRAlgorithm();//destructor
        GRAlgorithm(const GRAlgorithm& );//copy constructor
        GRAlgorithm& operator=(const GRAlgorithm&);//operator=
        void grInitAlgorithms(); //initialise of algorithms
        //madgwick
        bool madgwickUpdate(gr_message*, gr_alg_message*, int, std::string flag); //Update iterative data of madgwick algorithm
        //void madgwickUpdateThr(device_t*, alg_device_t*, int, std::string flag);//TODO need to implement
        bool setupMadgwick(int, int, int, int, int, int);//precondition Madgwick algorithm

        //simplified Kalman
        
        bool setUpKfilter(GRConnection conn, acc_k_vars*, std::string, int );//TODO implement for fingers
        double kFilter(double, k_filter_vars*);//simplified kalman 

             
    private:
        double _roll, _pitch, _yaw;
        std::vector<double> _angles;//vector with computed from quaternion angles
        std::vector<double> _computeAngles(std::vector<double>);//calculation of angles from quaternion method

        //grMadgwick objects
        GRMadgwick _pinkyMadgwick;
        GRMadgwick _ringMadgwick;
        GRMadgwick _middleMadgwick;
        GRMadgwick _indexMadgwick;
        GRMadgwick _thumbMadgwick;
        GRMadgwick _palmMadgwick;

        std::unordered_map<std::string, GRMadgwick*> _madgwickObjects; //map for easier access of objects

        //kFilter help methods
        double _stdev(std::vector<double>*);//standart deviation
        double _average(std::vector<double>*);//average



};


#endif
