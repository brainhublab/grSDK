#ifndef GR_ALGORITHM
#define GR_ALGORITHM

#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <deque>
#include <math.h>
#include <thread>

//#include <eigen3/Eigen/Dense>
//#include <eigen3/Eigen/Geometry>

#include "GRT/GRT.h"

#include <grDevice.h>
#include <grMadgwick.h>
#include <grGrt.h>
using namespace std;
//using namespace Eigen;
struct k_filter_vars
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
}

class GRAlgorithm :public GRGrt
{

    public:
        GRAlgorithm();
        ~GRAlgorithm();
        GRAlgorithm(const GRAlgorithm& );
        GRAlgorithm& operator=(const GRAlgorithm&);
        void grInitAlgorithms();
        //madgwick
        bool madgwickUpdate(gr_message*, gr_alg_message*, int, std::string flag);
        //void madgwickUpdateThr(device_t*, alg_device_t*, int, std::string flag);//TODO need to implement
        bool setupMadgwick(int, int, int, int, int, int);

        //simplified Kalman
        bool setUpKfilter(std::vector<double>, k_filter_vars*);
        double kFilter(double, k_filter_vars*);

             
    private:
        double roll, pitch, yaw;
        std::vector<double> angles;
        std::vector<double> computeAngles(std::vector<double>);

        GRMadgwick pinkyMadgwick;
        GRMadgwick ringMadgwick;
        GRMadgwick middleMadgwick;
        GRMadgwick indexMadgwick;
        GRMadgwick thumbMadgwick;
        GRMadgwick palmMadgwick;

        std::unordered_map<std::string, GRMadgwick*> madgwickObjects;

        //kFilter help methods
        double stdev(std::vector<double>*);
        double average(std::vector<double>*);



};


#endif
