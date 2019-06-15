#ifndef GR_MADGWICK
#define GR_MADGWICK

#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <math.h>
#include <array>

/**
 * GR Madgwick class
 */
class GRMadgwick
{

public:
	/**
	 * @brief constructor
	 */
	GRMadgwick();
	/**
	 * @brief destructor
	 */
	~GRMadgwick();
	/**
	 * @brief copy constructor
	 */
	GRMadgwick(const GRMadgwick&);
	/**
	 * @brief assigment operator
	 */
	GRMadgwick& operator=(const GRMadgwick&);


	/**
	 * @brief
	 */
	 /**
		* @brief main step update of Madgwick
		*/
  void MadgwickAHRSupdate(double gx, double gy, double gz, double ax, double ay, double az,
        double mx, double my, double mz, std::array<double, 4>*);
	/**
	 * @brief
	 */
	 /**
 	 * @brief update AHRS without magnetometer
 	 */
  void MadgwickAHRSupdateIMU(double gx, double gy, double gz, double ax, double ay, double az,
        std::array<double, 4>*);
	/**
	 * @brief set arbitary frequency calibration variable
	 */
        bool setFreqCalibration(int);
private:
	/**
	 * @brief TODO
	 */
	double constrain(double , double , double );
	/**
	 * @brief defining of gravity
	 */
        double gravity = 256;

        //madgwick algorithm vars and methods
	/**
	 * @brief quaternion of sensor frame relative to auxiliary frame
	 */
        double q0 , q1, q2, q3;
	/**
	 * @brief variable
	 */
        double beta;
	/**
	 * @brief callibration frequency
	 */
        int freqCallibration;

	/**
	 * @brief angles computing from quaternion
	 */
        std::vector<double> computeAngles();
	/**
	 * @brief inverse sqrt from doom or quake :D
	 */
        double invSqrt(double x);
};
#endif
