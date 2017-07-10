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
		GRMadgwick();
		~GRMadgwick();
		GRMadgwick(const GRMadgwick&);
		GRMadgwick& operator=(const GRMadgwick&);

	
    	void MadgwickAHRSupdate(double gx, double gy, double gz, double ax, double ay, double az, 
                double mx, double my, double mz, std::vector<double>*);
        void MadgwickAHRSupdateIMU(double gx, double gy, double gz, double ax, double ay, double az, 
                std::vector<double>*);
        bool setFreqCalibration(int);	
	private:
		double constrain(double , double , double );
        double gravity = 256;
        //madgwick algorithm vars and methods
        double q0 , q1, q2, q3;  // quaternion of sensor frame relative to auxiliary frame
        double beta;
        int freqCallibration;

        std::vector<double> computeAngles();
        double invSqrt(double x);
};
#endif
