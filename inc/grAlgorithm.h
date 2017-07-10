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


};


#endif
