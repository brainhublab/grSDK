#include <grAlgorithm.h>

#define sampleFreq 100.0f
#define betaDef 0.1f

GRAlgorithm::GRAlgorithm()
{
 
}

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
}

gr_alg_message GRAlgorithm::madgwickUpdate(gr_message* imu, int freqCallibration, std::string flag)
{
    std::vector<float> rotations;
    	   // std::cout<<"\nbefore madgwickUpdate() Q :"<<q0<<" "<<q1<<" "<<q2<<" "<<q3<<std::endl;
        GRMadgwick::MadgwickAHRSupdate(imu->gyro[0], imu->gyro[1], imu->gyro[2], 
                imu->accel[0], imu->accel[1], imu->accel[2], 
                imu->mag[0], imu->mag[1], imu->mag[2], &rotations, freqCallibration);

    if(flag == "QANTERION")
    {
        return rotations;
    }
    else if(flag == "ANGLES")
    {
        rotations = computeAngles(rotations);
        return rodations;
    }
    else
    {
        std::cout<<"ERROR: no such flag "<<flag<<std::endl;
    }

}
/* TODO implement in thread madgwick update
void GRAlgorithm::madgwickUpdateThr(imu* imu, int freqCallibration, std::string flag)
{
    std::thread madgwick(&GRAlgorithm::madgwickUpdate, this, imu,  freqCallibration, flag);
    pinky.detach();
    std::cout<<"run MadgwickAHRSupdate thread for pinky"<<endl;
}
*/
std::vector<float> GRMadgwick::computeAngles(std::vector<float> q)
{

    std::vector<float> angles;
    angles.clear();
    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;
    roll = atan2f(q[0]*q[1] + q[2]*q[3], 0.5f - q[1]*q[1] - q[2]*q[2]);
    pitch = asinf(-2.0*(q[1]*q[3] - q[0]*q[2]));
    yaw = atan2f(q[1]*q[2] + q[0]*q[3], 0.5f - q[2]*q[2] - q[3]*q[3]);

    roll = roll * 57.29578f;
    pitch = pitch * 57.29578f;
    yaw = yaw * 57.29578f + 180.0f;

    angles.push_back(0.0f);
    angles.push_back(roll);
    angles.push_back(pitch);
    angles.push_back(yaw);
    anglesComputed = 1;
    return angles;
}

