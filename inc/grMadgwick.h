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

	
    protected:
    	void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, 
                float mx, float my, float mz, std::vector<float>*, int, std::string);
        void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az, 
                std::vector<float>*, int, std::string);
	
	private:
		double constrain(double , double , double );
        double gravity = 256;
        //madgwick algorithm vars and methods
        float q0 , q1, q2, q3;  // quaternion of sensor frame relative to auxiliary frame
        float beta;

        std::vector<float> computeAngles();
        float invSqrt(float x);
};
#endif
