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
    Eigen::Vector3d vel = Eigen::Vector3d(0.f, 0.f, 0.f);
    Eigen::Vector3d pos = Eigen::Vector3d(0.f, 0.f, 0.f);
    Eigen::Vector3d acc = Eigen::Vector3d(0.f, 0.f, 0.f);
};
/**
 * GR Trajectory class descr
 */
class GRTrajectory
{
    public:
	/**
	 * @brief constructor
	 */
        GRTrajectory(); 
	/**
	 * @brief destructor
	 */
        ~GRTrajectory();
	/**
	 * @brief 
	 */
        std::vector<double> getNewPosByRunge(std::vector<double>, std::vector<double>, unsigned long);
	/**
	 * @brief 
	 */
        std::vector<double> getAccelerations(std::vector<double>, std::vector<double>);
	/**
	 * @brief 
	 */
        bool isStationary();

    private:
        /* Storing some values from previous calculations */

	/**
	 * @brief last stored timestamp 
	 */
        unsigned long _timestampLast;
	/**
	 * @brief last runge vectors
	 */
        runge_vars _rungeLast;

        /* Constant values */
	/**
	 * @brief constant gravity vector
	 */
        const Eigen::Vector3d _gravity = Eigen::Vector3d(0.0, 0.0, G);

        /* Quaternion rotation methods */
	
	/**
	 * @brief rotate accelerometer vector with quaternion 
	 */
        Eigen::Vector3d _rotateAcc(Eigen::Vector3d, Eigen::Quaterniond);

        /* Unit conversion methods */

	/**
	 * @brief coverts acceleromer raw data to G
	 */
        Eigen::Vector3d _convertAccToG(Eigen::Vector3d);
	/**
	 * @brief coverts acceleromer raw data to G
	 */
        Eigen::Vector3d _convertAccToMS(Eigen::Vector3d);

        /* Type conversion methods */
	
	/**
	 * @brief convert Eigen::Vector3d to std::vector
	 */
        vector<double> _toStdVector(Eigen::Vector3d);
	/**
	 * @brief convert std::vector to Eigen::vector3d
	 */
        Eigen::Vector3d _toVector3d(vector<double>);
	/**
	 * @brief convert std::vector to Eigen::Quaterniond
	 */
        Eigen::Quaterniond _toQuaterniond(vector<double>);

        /* Integration calculation methods */

	/**
	 * @brief Runge-Kutta method
	 */
        runge_vars _rk4(Eigen::Vector3d, runge_vars, double );

	/**
	 * @brief 
	 */
        Eigen::Vector3d _correctVector(Eigen::Vector3d);

	/**
	 * @brief 
	 */
        Eigen::Vector3d _accLast;

	/**
	 * @brief describes if current acc is stationary  
	 */
        bool _isStationary;

	/**
	 * @brief 
	 */
        double _accTreshold;
	/**
	 * @brief 
	 */
        double _gyroTreshold;
	/**
	 * @brief 
	 */
        double _varianceTreshold;
	/**
	 * @brief 
	 */
        std::queue<int> _stationaryWindow;
};

#endif
