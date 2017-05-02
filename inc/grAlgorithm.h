#ifndef GR_ALGORITHM
#define GR_ALGORITHM

#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <deque>
#include <cmath>
#include <thread>

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>

#include "GRT/GRT.h"

#include <grDevice.h>
using namespace std;
//using namespace Eigen;

class GRAlgorithm 
{

    public:
        GRAlgorithm();
        ~GRAlgorithm();
        GRAlgorithm(const GRAlgorithm& );
        GRAlgorithm& operator=(const GRAlgorithm&);
        void grInitAlgorithms();
        //bool 
   private:
        //helper methods
        double constrain(double , double , double );
        double gravity = 256;

        float sampleFreq = 512.0;
        float betaDef = 0.1;
        float beta;
        float invSqrt(float x);            // algorithm gain
                                      // 2 * proportional gain (Kp)
        float q0 , q1, q2, q3;  // quaternion of sensor frame relative to auxiliary frame
		void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, std::deque<std::vector<float>>*, int);
		void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az, std::deque<std::vector<float>>*, int);

		void madgwickUpdateBuffer(imu*, std::deque<std::vector<float>>*, int);
        void madgwickUpdateThr(device_t*, alg_device_t*);

};


#endif
