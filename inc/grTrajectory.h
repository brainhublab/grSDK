#ifndef GR_TRAJECTORY
#define GR_TRAJECTORY

#include <iostream>
#include <vector>
#include <cmath>
#include "Eigen/Dense"
#include "Eigen/Geometry"
#include "grAlgorithm.h"


using  namespace Eigen;
using namespace std;

#define G  9.80665
#define ACC_MULT 3.9 // 0.061 // 0.975 // 3.9 // 0.244
#define FILTER_LOW 0

struct runge_vars
{
    Eigen::Vector3d vel;
    Eigen::Vector3d pos;
    Eigen::Vector3d acc;
};

class GRTrajectory
{
    public:
        GRTrajectory(); //constructor
        ~GRTrajectory(); //destructor
        vector<double> getNewPosByRunge(vector<double>, vector<double>, unsigned long);
    private:
        /* Storing some values from previous calculations */
        unsigned long timestampLast;  //last stored timestamp
        runge_vars rungeLast;

        /* Constant values */
        const Eigen::Vector3d _gravity = Eigen::Vector3d(0.0, 0.0, G);  //gravity vector

        /* Quaternion rotation methods */
        Eigen::Vector3d _rotateAcc(Eigen::Vector3d, Eigen::Quaterniond);  //rotate accelerometer vector with quaternion

        /* Unit conversion methods */
        Eigen::Vector3d _convertAccToG(Eigen::Vector3d);  //coverts acceleromer raw data to G
        Eigen::Vector3d _convertAccToMS(Eigen::Vector3d);  //coverts acceleromer raw data to G

        /* Type conversion methods */
        vector<double> _toStdVector(Eigen::Vector3d);  //convert Eigen::Vector3d to std::vector
        Eigen::Vector3d _toVector3d(vector<double>);  //convert std::vector to Eigen::vector3d
        Eigen::Quaterniond _toQuaterniond(vector<double>);  //convert std::vector to Eigen::Quaterniond

        /* Integration calculation methods */
        runge_vars _rk4(Eigen::Vector3d, runge_vars, double );  // Runge-Kutta method

        Eigen::Vector3d acc_last;
};

#endif
