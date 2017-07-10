#include <grMadgwick.h>

#define sampleFreq 100.0
#define betaDef 0.1

GRMadgwick::GRMadgwick()
{
    this->beta = betaDef;
    this->q0 = 1.0;
    this->q1 = 0.0;
    this->q2 = 0.0;
    this->q3 = 0.0;
}

GRMadgwick::~GRMadgwick()
{

}

GRMadgwick::GRMadgwick(const GRMadgwick& t)
{

}

GRMadgwick& GRMadgwick::operator=(const GRMadgwick& t)
{

}
void GRMadgwick::MadgwickAHRSupdate(double gx, double gy, double gz, double ax, double ay, double az, 
        double mx, double my, double mz, std::vector<double>* results)
{
    double recipNorm;
    double s0, s1, s2, s3;
    double qDot1, qDot2, qDot3, qDot4;
    double hx, hy;
    double _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;

    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
    if((mx == 0.0) && (my == 0.0) && (mz == 0.0)) {
        MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az, results);
        return;
    }

    gx *= 0.0174533;
    gy *= 0.0174533;
    gz *= 0.0174533;

	//std::cout<<"Pre-Middle Q :"<<q0<<" "<<q1<<" "<<q2<<" "<<q3<<std::endl;
    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5 * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5 * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5 * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5 * (q0 * gz + q1 * gy - q2 * gx);


    if(!((ax == 0.0) && (ay == 0.0) && (az == 0.0))) {

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Normalise magnetometer measurement
        recipNorm = invSqrt(mx * mx + my * my + mz * mz);
        mx *= recipNorm;
        my *= recipNorm;
        mz *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0mx = 2.0 * q0 * mx;
        _2q0my = 2.0 * q0 * my;
        _2q0mz = 2.0 * q0 * mz;
        _2q1mx = 2.0 * q1 * mx;
        _2q0 = 2.0 * q0;
        _2q1 = 2.0 * q1;
        _2q2 = 2.0 * q2;
        _2q3 = 2.0 * q3;
        _2q0q2 = 2.0 * q0 * q2;
        _2q2q3 = 2.0 * q2 * q3;
        q0q0 = q0 * q0;
        q0q1 = q0 * q1;
        q0q2 = q0 * q2;
        q0q3 = q0 * q3;
        q1q1 = q1 * q1;
        q1q2 = q1 * q2;
        q1q3 = q1 * q3;
        q2q2 = q2 * q2;
        q2q3 = q2 * q3;
        q3q3 = q3 * q3;

        // Reference direction of Earth's magnetic field
        hx = mx * q0q0 - _2q0my * q3 + _2q0mz * q2 + mx * q1q1 + _2q1 * my * q2 + _2q1 * mz * q3 - mx * q2q2 - mx * q3q3;
        hy = _2q0mx * q3 + my * q0q0 - _2q0mz * q1 + _2q1mx * q2 - my * q1q1 + my * q2q2 + _2q2 * mz * q3 - my * q3q3;
        _2bx = sqrt(hx * hx + hy * hy);
        _2bz = -_2q0mx * q2 + _2q0my * q1 + mz * q0q0 + _2q1mx * q3 - mz * q1q1 + _2q2 * my * q3 - mz * q2q2 + mz * q3q3;
        _4bx = 2.0 * _2bx;
        _4bz = 2.0 * _2bz;

        // Gradient decent algorithm corrective step
        s0 = -_2q2 * (2.0 * q1q3 - _2q0q2 - ax) + _2q1 * (2.0 * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0 * q1q3 - _2q0q2 - ax) + _2q0 * (2.0 * q0q1 + _2q2q3 - ay) - 4.0 * q1 * (1.0 - 2.0 * q1q1 - 2.0 * q2q2 - az) + _2bz * q3 * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0 * q1q3 - _2q0q2 - ax) + _2q3 * (2.0 * q0q1 + _2q2q3 - ay) - 4.0 * q2 * (1.0 - 2.0 * q1q1 - 2.0 * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0 * q1q3 - _2q0q2 - ax) + _2q2 * (2.0 * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5 - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5 - q1q1 - q2q2) - mz);
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    q0 += qDot1 * (1.0 / (sampleFreq + freqCallibration));
    q1 += qDot2 * (1.0 / (sampleFreq + freqCallibration));
    q2 += qDot3 * (1.0 / (sampleFreq + freqCallibration));
    q3 += qDot4 * (1.0 / (sampleFreq + freqCallibration));

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    std::vector<double> new_result = {q0, q1, q2, q3};

    results->assign(new_result.begin(), new_result.end());


}
void GRMadgwick::MadgwickAHRSupdateIMU(double gx, double gy, double gz, double ax, double ay, double az, 
        std::vector<double>* results) 
{
    double recipNorm;
    double s0, s1, s2, s3;
    double qDot1, qDot2, qDot3, qDot4;
    double _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5 * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5 * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5 * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5 * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if(!((ax == 0.0) && (ay == 0.0) && (az == 0.0))) {

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0 = 2.0 * q0;
        _2q1 = 2.0 * q1;
        _2q2 = 2.0 * q2;
        _2q3 = 2.0 * q3;
        _4q0 = 4.0 * q0;
        _4q1 = 4.0 * q1;
        _4q2 = 4.0 * q2;
        _8q1 = 8.0 * q1;
        _8q2 = 8.0 * q2;
        q0q0 = q0 * q0;
        q1q1 = q1 * q1;
        q2q2 = q2 * q2;
        q3q3 = q3 * q3;

        // Gradient decent algorithm corrective step
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0 * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0 * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0 * q1q1 * q3 - _2q1 * ax + 4.0 * q2q2 * q3 - _2q2 * ay;
        recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    q0 += qDot1 * (1.0 / (sampleFreq + freqCallibration));
    q1 += qDot2 * (1.0 / (sampleFreq + freqCallibration));
    q2 += qDot3 * (1.0 / (sampleFreq + freqCallibration));
    q3 += qDot4 * (1.0 / (sampleFreq + freqCallibration));

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    //    std::cout<<"Q :"<<q0<<q1<<q2<<q3<<std::endl;

    std::vector<double> new_result = {q0, q1, q2, q3};
    results->assign(new_result.begin(), new_result.end());

}
bool GRMadgwick::setFreqCalibration(int callibration)
{
    this->freqCallibration = callibration;
}
// Fast inverse square-root

double GRMadgwick::invSqrt(double xd) {
    
    float x = (float)xd;
   float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    y = y * (1.5f - (halfx * y * y));
    return (double)y; 
}

// helper private methods
double GRMadgwick::constrain(double x, double a, double b)
{
    if(x < a)
    {
        return a;
    }
    else if(b < x)
    {
        return b;
    }
    else
        return x;
}
