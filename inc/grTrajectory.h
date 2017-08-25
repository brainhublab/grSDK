#ifndef GR_TRAJECTORY
#define GR_TRAJECTORY

#include <vector>
#include <cmath>
#include "Eigen/Dense"
#include "Eigen/Geometry"
//#include "Eigen"

using Eigen::Vector3d;
using Eigen::Quaterniond;
using  namespace Eigen;
using namespace std;

#define G  9.80665  // 9.000416 need to be 9.80665
#define ACC_MULT 0.00409836065574 //0.244 //0.061
#define FILTER_LOW 0

class GRTrajectory
{
    public:
        GRTrajectory(); //constructor
        ~GRTrajectory(); //destructor
        bool calibrateGravityMatrix(std::vector<double>, int); //gravity matrix callibration method
        bool calculateCorrectionMatrix(); //calculation of correction graity matrix
        vector<double> getNewPosByVelocity(vector<double>, vector<double>, unsigned long); //getting new position 
        vector<double> getNewPosByIntegrating(vector<double>, vector<double>, unsigned long); //getting of new position
        vector<double> getNewPos(vector<double> acc, vector<double>, unsigned long);//use both methods
    private:
        Vector3d pos_last; //last stored position
        Vector3d velocity_last; //last stored velocity
        unsigned long timestamp_last; //last stored timestamp
        bool first_call; //arbitary variable for initialization of algorithms 

        Vector3d _getNewPosByVelocity(Vector3d, unsigned long); //method in two step gettng of position
        Vector3d _getNewPosByIntegrating(Vector3d, unsigned long, Quaterniond); //double integration position

        Vector3d _rotateAcc(Vector3d, Quaterniond);//rotate accelerometer vector with quaternion
        Vector3d _convertAccToG(Vector3d);//coverts acceleromer raw data to G

        vector<double> _toStdVector(Vector3d);//convert Eigen::Vector3d to std::vector
        Vector3d _toVector3d(vector<double>);//convert std::vector to Eigen::vector3d
        Quaterniond _toQuaterniond(vector<double>);//convert std::vector to Eigen::Quaterniond
        Vector3d _gravity_compensate(vector<double> , vector<double> );//gravity compenstation with acc and q
        Vector3d _gravity = Vector3d(0.0, 0.0, G); //gravity vector
        Vector3d _acc_last; //last stored accelerometer data
        Matrix4d _correctionMatrix; // correction matrix for callibration of accelerometer
        Matrix4d _desiredMatrix; // desired matrix for calibration of accelerometer
        Matrix4d _realMatrix;//matrix with real raw data from accelerometer

        bool _setupGravityMatrices(); // setup method for correction matrix for accelerometer
};

#endif
