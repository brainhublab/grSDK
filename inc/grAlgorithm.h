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

class GRAlgorithm : protected GRMadgwick, public GRGrt
{

    public:
        GRAlgorithm();
        ~GRAlgorithm();
        GRAlgorithm(const GRAlgorithm& );
        GRAlgorithm& operator=(const GRAlgorithm&);
        void grInitAlgorithms();
        //madgwick
        void madgwickUpdateBuffer(imu*, std::deque<std::vector<float>>*, int, std::string flag);
        void madgwickUpdateThr(device_t*, alg_device_t*, int, std::string flag);
    private:


};


#endif
