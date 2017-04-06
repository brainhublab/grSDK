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


//#include "kalman/ekfilter.hpp"
#include <grDevice.h>
using namespace std;
//using namespace Eigen;
/*
struct imu
{
    int id;
    Eigen::Quaterniond quanternion;
};
*/
struct alg_device_t
{
    // to store quaternions
    std::deque<std::vector<float>> pinky;
    std::deque<std::vector<float>> ring;
    std::deque<std::vector<float>> middle;
    std::deque<std::vector<float>> index;
    std::deque<std::vector<float>> thumb;
    std::deque<std::vector<float>> palm;
};


class GRAlgorithm 
{

    public:
        GRAlgorithm();
        //GRAlgorithm(device_t device);
        ~GRAlgorithm();
        GRAlgorithm(const GRAlgorithm& );
        GRAlgorithm& operator=(const GRAlgorithm&);

        void grInitAlgorithms();

        /*void grDCMNomalize();
        void grDCMDriftCorrection();
        void grDCMMatrixUpdate();
        void grDCMEulerAngels();
        */

        //matrix multiply, vector dot product, cross product, mult vector by scalar, vector add,  are implemented,

        bool imuEKF();
 //   private:
        //helper methods
        double constrain(double , double , double );
        double gravity = 256;
        //functions for generating jacobian matrices for kalman filter
        //DCM variables
        //double kpRollPitch = 0.02;
        //double kiRollPitch = 0.00002;
        //double kpYaw = 1.2;
        //double kiYaw = 0.00002;

        //device_t device;
        //algDev_t result;

        //double magHeading;
        /*
        Eigen::Matrix2d DCMMatrix = new Eigen::Matrix2d(3, 3);

        Eigen::Vector3d accelVector = new Eigen::Vector3d(0, 0, 0);
        Eigen::Vector3d gyroVector = new Eigen::Vector3d(0, 0, 0);
        Eigen::Vector3d omegaVector = Eigen::Vector3d(0, 0, 0);
        Eigen::Vector3d omegaP = Eigen::Vector3d(0, 0, 0);
        Eigen::Vector3d omegaI = Eigen::Vector3d(0, 0, 0);
        Eigen::Vector3d omega = Eigen::Vector3d(0, 0, 0);

        Eigen::Vector3d errorRollPitch = new Eigen::Vector3d(0, 0, 0);
        Eigen::Vector3d errorYaw = new Eigen::Vector3d(0, 0, 0);
        */
        //Magwick variables
        float sampleFreq = 512;
        float betaDef = 0.1;
        float beta = betaDef;
        float invSqrt(float x);            // algorithm gain
                                      // 2 * proportional gain (Kp)
        float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;  // quaternion of sensor frame relative to auxiliary frame
        /*void makeA();
        void makeH();
        void makeV();
        void makeR();
        void makeW();
        void makeQ();
        void makeProccess();
        void makeMeasure();
*/

        void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, std::deque<std::vector<float>>*);
        void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az, std::deque<std::vector<float>>*);

        void updateBuffer(imu*, std::deque<std::vector<float>>*);
        void madgwickAHRS(device_t*, alg_device_t*);

};


#endif
