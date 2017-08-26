#ifndef GR_MADGWICK
#define GR_MADGWICK

#include <iostream>
#include <string>
#include <deque>
#include <vector>
#include <math.h>

class GRMadgwick
{

	public:
		GRMadgwick();//constructor
		~GRMadgwick();//destructor
		GRMadgwick(const GRMadgwick&);//copy constructor
		GRMadgwick& operator=(const GRMadgwick&);//operator =

	
    	void MadgwickAHRSupdate(double gx, double gy, double gz, double ax, double ay, double az, 
                double mx, double my, double mz, std::vector<double>*);//main step update of Madgwick
        void MadgwickAHRSupdateIMU(double gx, double gy, double gz, double ax, double ay, double az, 
                std::vector<double>*);//update AHRS without magnetometer
        bool setFreqCalibration(int);//set arbitary frequency calibration variable	
	private:
		double constrain(double , double , double );//hmm
        double gravity = 256;//defining of gravity
        //madgwick algorithm vars and methods
        double q0 , q1, q2, q3;  // quaternion of sensor frame relative to auxiliary frame
        double beta;//variable
        int freqCallibration;//calibration 

        std::vector<double> computeAngles();//angles computing from wuaternion
        double invSqrt(double x);//inverse sqrt from doom or quake :D
};
#endif
