#include <grAlgorithm.h>

#define sampleFreq 100.0f
#define betaDef 0.1f

GRAlgorithm::GRAlgorithm()
{
    
    madgwickObjects["pinky"] = &(this->pinkyMadgwick);
    madgwickObjects["ring"] = &(this->ringMadgwick);
    madgwickObjects["middle"] = &(this->middleMadgwick);
    madgwickObjects["index"] = &(this->indexMadgwick);
    madgwickObjects["thumb"] = &(this->thumbMadgwick);
    madgwickObjects["palm"] = &(this->palmMadgwick);
 
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

bool GRAlgorithm::madgwickUpdate(gr_message* message, gr_alg_message* result, 
        int freqCallibration, std::string flag)
{
    std::vector<float> rotations;
    std::unordered_map<std::string, imu*>::iterator it;
    // std::cout<<"\nbefore madgwickUpdate() Q :"<<q0<<" "<<q1<<" "<<q2<<" "<<q3<<std::endl;
    for(it=message->imus.begin(); it!=message->imus.end(); it++ )
    {
        madgwickObjects[it->first]->MadgwickAHRSupdate(
                message->imus[it->first]->gyro[0], message->imus[it->first]->gyro[1], message->imus[it->first]->gyro[2], 
                message->imus[it->first]->acc[0], message->imus[it->first]->acc[1], message->imus[it->first]->acc[2], 
                message->imus[it->first]->mag[0], message->imus[it->first]->mag[1], message->imus[it->first]->mag[2], 
                &rotations);
        if(it->first == "pinky")
        {
            result->pinky.push_back(rotations);
        }
        else if(it->first == "ring")
        {
            result->ring.push_back(rotations);
        }
        else if(it->first == "middle")
        {
            result->middle.push_back(rotations);
        }
        else if(it->first == "index")
        {
            result->index.push_back(rotations);
        }
        else if(it->first == "thumb")
        {
            result->thumb.push_back(rotations);
        }
        else
        {
            result->palm.push_back(rotations);
        }
    }

}
bool GRAlgorithm::setupMadgwick(int pCallib, int rCallib, int mCallib, int iCallib, int tCallib, int paCallib)
{
    madgwickObjects["pinky"]->setFreqCalibration(pCallib);
    madgwickObjects["ring"]->setFreqCalibration(rCallib);
    madgwickObjects["middle"]->setFreqCalibration(mCallib);
    madgwickObjects["index"]->setFreqCalibration(iCallib);
    madgwickObjects["thumb"]->setFreqCalibration(tCallib);
    madgwickObjects["palm"]->setFreqCalibration(paCallib);
}
/* TODO implement in thread madgwick update
void GRAlgorithm::madgwickUpdateThr(imu* imu, int freqCallibration, std::string flag)
{
    std::thread madgwick(&GRAlgorithm::madgwickUpdate, this, imu,  freqCallibration, flag);
    pinky.detach();
    std::cout<<"run MadgwickAHRSupdate thread for pinky"<<endl;
}
*/
std::vector<float> GRAlgorithm::computeAngles(std::vector<float> q)
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
    return angles;
}

