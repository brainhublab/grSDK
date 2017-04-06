#include <grAlgorithm.h>

//float sampleFreq  512.0f      // sample frequency in Hz
//float betaDef     0.1f        // 2 * proportional gain

//float invSqrt(float x);

//beta = betaDef;

GRAlgorithm::GRAlgorithm()
{

}
/*
GRAlgorithm::GRAlgorithm(std::vector<device_t> dev)
{
    this->device = dev;
}
*/
GRAlgorithm::~GRAlgorithm()
{

}

GRAlgorithm::GRAlgorithm(const GRAlgorithm& t)
{

}

GRAlgorithm& GRAlgorithm::operator=(const GRAlgorithm& t)
{

}

void grInitAlgorithms()
{
    /*
    //todo: each goes in a thread of it's own
    updateBuffer(&device.pinky, &result.pinky);
    updateBuffer(&device.ring, &result.ring);
    updateBuffer(&device.middle, &result.middle);
    updateBuffer(&device.index, &result.index);
    updateBuffer(&device.palm, &result.palm);
    */
}

/*
void GRAlgorithm::grDCMNomalize()
{
    double error = 0.0;
    Eigen::Matrix2d tmp[3][3];
    double renorm = 0.0;

    error = -(DCMMatrix.row(0).dot(DCMMatrix.row(1)) * 0.5);
    //vector scale
    tmp.row(0) = DCMMatrix.row(1) * error;
    tmp.row(1) = DCMMatrix.row(0) * error;
    //vector add
    tmp.row(0) = tmp.row(0) + DCMMatrix(0);
    tmp.row(1) = tmp.row(1) + DCMMatrix(1);
    //vector cross prouct
    tmp.row(2) = tmp.row(0) * tmp.row(1);

    renorm = 0.5 * (3 - tmp.row(0).dot(tmp.row(0)));
    DCMMatrix(0) = tmp(0) * renorm;

    renorm = 0.5 * (3 - tmp.row(1).dot(tmp.row(1)));
    DCMMatrix(1) = tmp(1) * renorm;

    renorm = 0.5 * (3 - tmp.row(2).dot(tmp.row(2)));
    DCMMatrix(2) = tmp(2) * renorm;

}
void GRAlgorithm::grDCMDriftCorrection()
{
    double magHeadingX;
    double magHeadingY;
    double errorCourse;
    //compensation the roll pitch yaw
    double scaledOmegaP[3];
    double scaledOmegaI[3];
    double accelMagnitutde;
    double accelWeight;

    //roll and pitch
    accelMagnitutde = sqrt(accelVector(0) * accelVector(0) +
            accelVector(1) * accelVector(1) +
            accelVector(2) * accelVector(2));

    accelMagnitutde = accelMagnitutde / gravity;//scale to gravity
    // Dynamic weighting of accelerometer info (reliability filter)
    // Weight for accelerometer info (<0.5G = 0.0, 1G = 1.0 , >1.5G = 0.0)
    accelWeight = constrain(1 - 2 * abs(1 - accelMagnitutde), 0, 1);

    errorRollPitch = accelVector * DCMMatrix(2);
    omegaP = errorRollPitch * (kpRollPitch * accelWeight);

    scaledOmegaI = errorRollPitch * (kiRollPitch * accelWeight);
    omegaI = omeI + scaledOmegaI;

    //yaw
    // We make the gyro YAW drift correction based on compass magnetic heading
    magHeadingX = cos(magHeading);
    magHeadingY = sin(magHeading);

    errorCourse = (DCMMatrix(0, 0) * magHeadingY) - (DCMMatrix(0, 1) * magHeadingX);//calculating yaw error
    errorYaw = DCMMatrix.row(2) * errorCourse; // upply yaw correction

    scaledOmegaP = errorYaw * kpYaw;
    omegaP = omegaP + scaledOmegaP;

    scaledOmegaI = errorYaw * kiYaw;
    omegaI = omegaI + scaledOmegaI;


}
void GRAlgorithm::grDCMMatrixUpdate()
{

}
void GRAlgorithm::grDCMEulerAngels()
{

}
*/
void GRAlgorithm::MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz, std::deque<std::vector<float>>* results) {

    /*if(results->size() != 0)
    {
        q0 = results->back()[0];
        q1 = results->back()[1];
        q2 = results->back()[2];
        q3 = results->back()[3];
    }
    else
    {*/
        //q0 = 1.0f;
        //q1 = 0.0f;
        //q2 = 0.0f;
        //q3 = 0.0f;
    //}

    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float hx, hy;
    float _2q0mx, _2q0my, _2q0mz, _2q1mx, _2bx, _2bz, _4bx, _4bz, _2q0, _2q1, _2q2, _2q3, _2q0q2, _2q2q3, q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
    // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
    if((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
        MadgwickAHRSupdateIMU(gx, gy, gz, ax, ay, az, results);
        return;
    }

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

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
        _2q0mx = 2.0f * q0 * mx;
        _2q0my = 2.0f * q0 * my;
        _2q0mz = 2.0f * q0 * mz;
        _2q1mx = 2.0f * q1 * mx;
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _2q0q2 = 2.0f * q0 * q2;
        _2q2q3 = 2.0f * q2 * q3;
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
        _4bx = 2.0f * _2bx;
        _4bz = 2.0f * _2bz;

        // Gradient decent algorithm corrective step
        // old with error
        s0 = -_2q2 * (2.0f * q1q3 - _2q0q2 - ax) + _2q1 * (2.0f * q0q1 + _2q2q3 - ay) - _2bz * q2 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q3 + _2bz * q1) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q2 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s1 = _2q3 * (2.0f * q1q3 - _2q0q2 - ax) + _2q0 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q1 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + _2bz * q3 * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q2 + _2bz * q0) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q3 - _4bz * q1) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s2 = -_2q0 * (2.0f * q1q3 - _2q0q2 - ax) + _2q3 * (2.0f * q0q1 + _2q2q3 - ay) - 4.0f * q2 * (1 - 2.0f * q1q1 - 2.0f * q2q2 - az) + (-_4bx * q2 - _2bz * q0) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (_2bx * q1 + _2bz * q3) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + (_2bx * q0 - _4bz * q2) * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        s3 = _2q1 * (2.0f * q1q3 - _2q0q2 - ax) + _2q2 * (2.0f * q0q1 + _2q2q3 - ay) + (-_4bx * q3 + _2bz * q1) * (_2bx * (0.5f - q2q2 - q3q3) + _2bz * (q1q3 - q0q2) - mx) + (-_2bx * q0 + _2bz * q2) * (_2bx * (q1q2 - q0q3) + _2bz * (q0q1 + q2q3) - my) + _2bx * q1 * (_2bx * (q0q2 + q1q3) + _2bz * (0.5f - q1q1 - q2q2) - mz);
        /*
        // new fixed
        s0= -_2q2*(2*(q1q3 - q0q2) - ax) + _2q1*(2*(q0q1 + q2q3) - ay) + -_4bz*q2*(_4bx*(0.5 - q2q2 - q3q3) + _4bz*(q1q3 - q0q2) - mx) + (-_4bx*q3+_4bz*q1)*(_4bx*(q1q2 - q0q3) + _4bz*(q0q1 + q2q3) - my) + _4bx*q2*(_4bx*(q0q2 + q1q3) + _4bz*(0.5 - q1q1 - q2q2) - mz);
        s1= _2q3*(2*(q1q3 - q0q2) - ax) + _2q0*(2*(q0q1 + q2q3) - ay) + -4*q1*(2*(0.5 - q1q1 - q2q2) - az) + _4bz*q3*(_4bx*(0.5 - q2q2 - q3q3) + _4bz*(q1q3 - q0q2) - mx) + (_4bx*q2+_4bz*q0)*(_4bx*(q1q2 - q0q3) + _4bz*(q0q1 + q2q3) - my) + (_4bx*q3-_8bz*q1)*(_4bx*(q0q2 + q1q3) + _4bz*(0.5 - q1q1 - q2q2) - mz);
        s2= -_2q0*(2*(q1q3 - q0q2) - ax) + _2q3*(2*(q0q1 + q2q3) - ay) + (-4*q2)*(2*(0.5 - q1q1 - q2q2) - az) + (-_8bx*q2-_4bz*q0)*(_4bx*(0.5 - q2q2 - q3q3) + _4bz*(q1q3 - q0q2) - mx)+(_4bx*q1+_4bz*q3)*(_4bx*(q1q2 - q0q3) + _4bz*(q0q1 + q2q3) - my)+(_4bx*q0-_8bz*q2)*(_4bx*(q0q2 + q1q3) + _4bz*(0.5 - q1q1 - q2q2) - mz);
        s3= _2q1*(2*(q1q3 - q0q2) - ax) + _2q2*(2*(q0q1 + q2q3) - ay)+(-_8bx*q3+_4bz*q1)*(_4bx*(0.5 - q2q2 - q3q3) + _4bz*(q1q3 - q0q2) - mx)+(-_4bx*q0+_4bz*q2)*(_4bx*(q1q2 - q0q3) + _4bz*(q0q1 + q2q3) - my)+(_4bx*q1)*(_4bx*(q0q2 + q1q3) + _4bz*(0.5 - q1q1 - q2q2) - mz);
*/
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
    q0 += qDot1 * (1.0f / sampleFreq);
    q1 += qDot2 * (1.0f / sampleFreq);
    q2 += qDot3 * (1.0f / sampleFreq);
    q3 += qDot4 * (1.0f / sampleFreq);

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    std::vector<float> new_result = {q0, q1, q2, q3};
    results->push_back(new_result);
 }
 void GRAlgorithm::MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az, std::deque<std::vector<float>>* results) {
    float q0, q1, q2, q3;

    if(results->size() == 4)
    {
        q0 = results->back()[0];
        q1 = results->back()[1];
        q2 = results->back()[2];
        q3 = results->back()[3];
    }
    else
    {
        q0 = 0.f;
        q1 = 0.f;
        q2 = 0.f;
        q3 = 0.f;
    }

    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

        // Normalise accelerometer measurement
        recipNorm = invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0 = 2.0f * q0;
        _2q1 = 2.0f * q1;
        _2q2 = 2.0f * q2;
        _2q3 = 2.0f * q3;
        _4q0 = 4.0f * q0;
        _4q1 = 4.0f * q1;
        _4q2 = 4.0f * q2;
        _8q1 = 8.0f * q1;
        _8q2 = 8.0f * q2;
        q0q0 = q0 * q0;
        q1q1 = q1 * q1;
        q2q2 = q2 * q2;
        q3q3 = q3 * q3;

        // Gradient decent algorithm corrective step
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
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
    q0 += qDot1 * (1.0f / sampleFreq);
    q1 += qDot2 * (1.0f / sampleFreq);
    q2 += qDot3 * (1.0f / sampleFreq);
    q3 += qDot4 * (1.0f / sampleFreq);

    // Normalise quaternion
    recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;

    std::vector<float> new_result = {q0, q1, q2, q3};
    results->push_back(new_result);
}

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root

float GRAlgorithm::invSqrt(float x) {
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long*)&y;
    i = 0x5f3759df - (i>>1);
    y = *(float*)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

// helper private methods
double GRAlgorithm::constrain(double x, double a, double b)
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

void GRAlgorithm::updateBuffer(imu* imu, std::deque<std::vector<float>>* quaternions)
{
    std::vector<float> gyro, accel, mag;
    while(true)
    {
        if(!imu->gyro.empty() && !imu->acc.empty() && !imu->mag.empty())
        {
            gyro = imu->gyro.front();
            imu->gyro.pop_front();
            accel = imu->acc.front();
            imu->acc.pop_front();
            mag = imu->mag.front();
            imu->mag.pop_front();

            MadgwickAHRSupdate(gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2], mag[0], mag[1], mag[2], quaternions);
        }
    }


}
void GRAlgorithm::madgwickAHRS(device_t* inDevice, alg_device_t* outDevice)
{
    std::thread pinky(&GRAlgorithm::updateBuffer, this, &inDevice->pinky,  &outDevice->pinky);
    pinky.detach();
    std::cout<<"run MadgwickAHRSupdate thread for pinky"<<endl;
    
    std::thread ring(&GRAlgorithm::updateBuffer, this, &inDevice->ring,  &outDevice->ring);
    ring.detach();
    std::cout<<"run MadgwickAHRSupdate thread for ring"<<endl;

    std::thread middle(&GRAlgorithm::updateBuffer, this, &inDevice->middle,  &outDevice->middle);
    middle.detach();
    std::cout<<"run MadgwickAHRSupdate thread for middle"<<endl;

    std::thread index(&GRAlgorithm::updateBuffer, this, &inDevice->index,  &outDevice->index);
    index.detach();
    std::cout<<"run MadgwickAHRSupdate thread for index"<<endl;

    std::thread thumb(&GRAlgorithm::updateBuffer, this, &inDevice->thumb,  &outDevice->thumb);
    thumb.detach();
    std::cout<<"run MadgwickAHRSupdate thread for thumb"<<endl;

    std::thread palm(&GRAlgorithm::updateBuffer, this, &inDevice->palm,  &outDevice->palm);
    palm.detach();
    std::cout<<"run MadgwickAHRSupdate thread for palm"<<endl;




}
