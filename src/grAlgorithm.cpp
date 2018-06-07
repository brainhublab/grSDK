#include <grAlgorithm.h>

#define sampleFreq 100.0f
#define betaDef 0.1f

GRAlgorithm::GRAlgorithm()
{
    
    _madgwickObjects["pinky"] = &(this->_pinkyMadgwick);
    _madgwickObjects["ring"] = &(this->_ringMadgwick);
    _madgwickObjects["middle"] = &(this->_middleMadgwick);
    _madgwickObjects["index"] = &(this->_indexMadgwick);
    _madgwickObjects["thumb"] = &(this->_thumbMadgwick);
    _madgwickObjects["palm"] = &(this->_palmMadgwick);
 
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

/*Update step of Madgwick algorithm
 * Takes gr_message and return quaternion 
 */
bool GRAlgorithm::madgwickUpdate(gr_message* message, gr_alg_message* result)
{
    std::vector<double> rotations;
    std::unordered_map<std::string, imu*>::iterator it;
    // std::cout<<"\nbefore madgwickUpdate() Q :"<<q0<<" "<<q1<<" "<<q2<<" "<<q3<<std::endl;
    for(it=message->imus.begin(); it!=message->imus.end(); it++ )
    {
        if(message->imus[it->first]->is_connected)
        {
            _madgwickObjects[it->first]->MadgwickAHRSupdate(
                    message->imus[it->first]->gyro[0], message->imus[it->first]->gyro[1], message->imus[it->first]->gyro[2], 
                    message->imus[it->first]->acc[0], message->imus[it->first]->acc[1], message->imus[it->first]->acc[2], 
                    message->imus[it->first]->mag[0], message->imus[it->first]->mag[1], message->imus[it->first]->mag[2], 
                    &rotations);
            if(it->first == "pinky")
            {
                result->pinky = rotations;
            }
            else if(it->first == "ring")
            {
                result->ring = rotations;
            }
            else if(it->first == "middle")
            {
                result->middle = rotations;
            }
            else if(it->first == "index")
            {
                result->index = rotations;
            }
            else if(it->first == "thumb")
            {
                result->thumb = rotations;
            }
            else if(it->first == "palm")
            {
                result->palm = rotations;
            }
            else
            {
                std::cout<<"Wow you have some random readings please debug me:"<<it->first<<std::endl;
            }
        }
    }
    rotations.clear();

}

/*SetUp method for Madgwick algorithm 
 */
bool GRAlgorithm::setupMadgwick(int pCallib, int rCallib, int mCallib, int iCallib, int tCallib, int paCallib)
{
    _madgwickObjects["pinky"]->setFreqCalibration(pCallib);
    _madgwickObjects["ring"]->setFreqCalibration(rCallib);
    _madgwickObjects["middle"]->setFreqCalibration(mCallib);
    _madgwickObjects["index"]->setFreqCalibration(iCallib);
    _madgwickObjects["thumb"]->setFreqCalibration(tCallib);
    _madgwickObjects["palm"]->setFreqCalibration(paCallib);
}
/* TODO implement in thread madgwick update
void GRAlgorithm::madgwickUpdateThr(imu* imu, int freqCallibration, std::string flag)
{
    std::thread madgwick(&GRAlgorithm::madgwickUpdate, this, imu,  freqCallibration, flag);
    pinky.detach();
    std::cout<<"run MadgwickAHRSupdate thread for pinky"<<endl;
}
*/


std::vector<double> GRAlgorithm::_computeAngles(std::vector<double> q)
{

    std::vector<double> angles;
    angles.clear();
    double roll = 0.0f;
    double pitch = 0.0f;
    double yaw = 0.0f;
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
    return _angles;
}


/* Standard deviation calculation
 */
double GRAlgorithm::_stDev(std::vector<double>* input)
{
    double sum = std::accumulate(input->begin(), input->end(), 0.0);
    double mean = sum / input->size();

    std::vector<double> diff(input->size());
    std::transform(input->begin(), input->end(), diff.begin(),
            std::bind2nd(std::minus<double>(), mean));

    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / input->size());

    return stdev;
}

/*Average
 */
double GRAlgorithm::_average(std::vector<double>* input)
{
    double average = std::accumulate(input->begin(), input->end(), 0.0);
}

double GRAlgorithm::_stdErr(std::vector<double>* input)
{
    double tmpVar = 0.0;
    tmpVar = _stDev(input);
    return (tmpVar/sqrt((double)input->size()));
}

