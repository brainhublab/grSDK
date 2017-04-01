#ifndef GESTUS_ALGORITHM
#define GESTUS_ALGORITHM

#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <deque>
#include <cmath>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Geometry>


#include "kalman/ekfilter.hpp"
using namespace std;
//using namespace Eigen;
struct imu
{
    int id;
    Eigen::Quaterniond quanternion;
};

class GRAlgorithm : Kalman::EKFilter<double, 0>
{

    public:
        GRAlgorithm();
        ~GRAlgorithm();
        GRAlgorithm(const GRAlgorithm& );
        GRAlgorithm& operator=(const GRAlgorithm&);

        /*void grDCMNomalize();
        void grDCMDriftCorrection();
        void grDCMMatrixUpdate();
        void grDCMEulerAngels();
        */
        void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, imu*);
        void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);

        //matrix multiply, vector dot product, cross product, mult vector by scalar, vector add,  are implemented, 

        bool imuEKF();
    private:
        //helper methods
        double constrain(double , double , double );
        double gravity = 256;
        //functions for generating jacobian matrices for kalman filter
        //DCM variables
        double kpRollPitch = 0.02;
        double kiRollPitch = 0.00002;
        double kpYaw = 1.2;
        double kiYaw = 0.00002;

        double magHeading;
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

};


#endif
