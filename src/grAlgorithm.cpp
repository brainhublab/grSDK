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
    std::vector<double> rotations;
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
        else
        {
            result->palm = rotations;
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
bool GRAlgorithm::setUpKfilter(GRConnection conn, acc_k_vars* k_vars, std::string flag, int devId)
{
    int i = 0;
    gr_message msg;
    std::vector<double> acc_x;
    std::vector<double> acc_y;
    std::vector<double> acc_z;
    while(i < 50)
    {
        conn.getData(devId, &msg);
        if(flag == "pinky")
        {
            acc_x.push_back(msg.pinky.acc[0]);
            acc_y.push_back(msg.pinky.acc[1]);
            acc_z.push_back(msg.pinky.acc[2]);
        }
        else if(flag == "ring")
        {
        
            acc_x.push_back(msg.ring.acc[0]);
            acc_y.push_back(msg.ring.acc[1]);
            acc_z.push_back(msg.ring.acc[2]);
        }
        else if(flag == "middle")
        {
        
            acc_x.push_back(msg.middle.acc[0]);
            acc_y.push_back(msg.middle.acc[1]);
            acc_z.push_back(msg.middle.acc[2]);
        }
        else if(flag == "index")
        {
        
            acc_x.push_back(msg.index.acc[0]);
            acc_y.push_back(msg.index.acc[1]);
            acc_z.push_back(msg.index.acc[2]);
        }
        else if(flag == "thumb")
        {
        
            acc_x.push_back(msg.thumb.acc[0]);
            acc_y.push_back(msg.thumb.acc[1]);
            acc_z.push_back(msg.thumb.acc[2]);
        }
        else if(flag == "palm")
        {
        
            acc_x.push_back(msg.palm.acc[0]);
            acc_y.push_back(msg.palm.acc[1]);
            acc_z.push_back(msg.palm.acc[2]);
        }
        else
        {
            std::cout<<"ERROR: no such flag -> "<<flag<<std::endl;
        }
       msg.clear();
       i++;
      
    }
    k_vars->acc_k_x.volt = stdev(&acc_x);
    k_vars->acc_k_y.volt = stdev(&acc_y);
    k_vars->acc_k_z.volt = stdev(&acc_z);


}

double GRAlgorithm::kFilter(double input, k_filter_vars* k_vars)
{
    k_vars->pc = k_vars->p + k_vars->proccess;
    k_vars->g = k_vars->pc / (k_vars->pc + k_vars->volt);
    k_vars->p = (1 - k_vars->g) * k_vars->pc;
    k_vars->xp = k_vars->xe;
    k_vars->zp = k_vars->xp;
    k_vars->xe = k_vars->g * (input - k_vars->zp) + k_vars->xp;
    
    return k_vars->xe;
}

std::vector<double> GRAlgorithm::computeAngles(std::vector<double> q)
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
    return angles;
}
//Kfilter private help methods

double GRAlgorithm::stdev(std::vector<double>* input)
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

double GRAlgorithm::average(std::vector<double>* input)
{
    double average = std::accumulate(input->begin(), input->end(), 0.0);
}
