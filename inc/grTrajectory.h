#ifndef GR_TRAJECTORY
#define GR_TRAJECTORY

#include <iostream>

#include <vector>
#include <cmath>
#include "Eigen/Dense"
#include "Eigen/Geometry"
//#include "Eigen"
#include "grAlgorithm.h"


using Eigen::Vector3d;
using Eigen::Quaterniond;
using  namespace Eigen;
using namespace std;

#define G  9.80665  // 9.000416 need to be 9.80665
#define ACC_MULT  0.061 //0.061
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
        Eigen::Vector3d pos_last; //last stored position
        Eigen::Vector3d pos_last_last; //last last stored position
        Eigen::Vector3d velocity_last; //last stored velocity
        unsigned long timestamp_last; //last stored timestamp
        bool first_call; //arbitary variable for initialization of algorithms

        Eigen::Vector3d _getNewPosByVelocity(Eigen::Vector3d, unsigned long); //method in two step gettng of position
        Eigen::Vector3d _getNewPosByIntegrating(Eigen::Vector3d, unsigned long, Eigen::Quaterniond); //double integration position

        Eigen::Vector3d _rotateAcc(Eigen::Vector3d, Eigen::Quaterniond);//rotate accelerometer vector with quaternion
        Eigen::Vector3d _convertAccToG(Eigen::Vector3d);//coverts acceleromer raw data to G

        vector<double> _toStdVector(Eigen::Vector3d);//convert Eigen::Vector3d to std::vector
        Eigen::Vector3d _toVector3d(vector<double>);//convert std::vector to Eigen::vector3d
        Eigen::Quaterniond _toQuaterniond(vector<double>);//convert std::vector to Eigen::Quaterniond
        Eigen::Vector3d _gravity_compensate(vector<double> , vector<double> );//gravity compenstation with acc and q
        Eigen::Vector3d _gravity = Vector3d(0.0, 0.0, G); //gravity vector
        Eigen::Vector3d _acc_last; //last stored accelerometer data
        Eigen::Matrix4d _correctionMatrix; // correction matrix for callibration of accelerometer
        Eigen::Matrix4d _desiredMatrix; // desired matrix for calibration of accelerometer
        Eigen::Matrix4d _realMatrix;//matrix with real raw data from accelerometer

        Eigen::Vector3d _rotateVectorByQuaternion(Eigen::Vector3d , Eigen::Quaterniond );
        bool _setupGravityMatrices(); // setup method for correction matrix for accelerometer

        GRAlgorithm _alg;

        Eigen::Vector3d _drift_offset = Eigen::Vector3d(0.174, 0.0130, 0.208);
        double _treshold;
        std::vector<Eigen::Vector3d> _stationaryVelocities;
        bool _stationary;
        Eigen::Vector3d _drifRate;

};

#endif
