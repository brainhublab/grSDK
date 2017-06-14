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

void GRAlgorithm::madgwickUpdateBuffer(imu* imu, std::deque<std::vector<float>>* rotations, 
        int freqCallibration, std::string flag)
{
    std::vector<float> gyro, accel, mag;

    while(!imu->data.front().gyro.empty() && imu->data.front().gyro.size() == 3 && 
            !imu->data.front().acc.empty() && imu->data.front().acc.size() == 3 && 
            !imu->data.front().mag.empty() && imu->data.front().mag.size() == 3)
    {
        // std::cout<<"in alg while"<<endl;
        gyro = imu->data.front().gyro;
        accel = imu->data.front().acc;
        mag = imu->data.front().mag;
        
        imu->data.pop_front();

	   // std::cout<<"\nbefore madgwickUpdate() Q :"<<q0<<" "<<q1<<" "<<q2<<" "<<q3<<std::endl;
        GRMadgwick::MadgwickAHRSupdate(gyro[0], gyro[1], gyro[2], accel[0], accel[1], accel[2], mag[0], mag[1], mag[2], 
                rotations, freqCallibration, flag);
    }

}
void GRAlgorithm::madgwickUpdateThr(device_t* inDevice, alg_device_t* outDevice, 
        int freqCallibration, std::string flag)
{
    std::thread pinky(&GRAlgorithm::madgwickUpdateBuffer, this, &inDevice->pinky,  &outDevice->pinky, freqCallibration, flag);
    pinky.detach();
    std::cout<<"run MadgwickAHRSupdate thread for pinky"<<endl;

    std::thread ring(&GRAlgorithm::madgwickUpdateBuffer, this, &inDevice->ring,  &outDevice->ring, freqCallibration, flag);
    ring.detach();
    std::cout<<"run MadgwickAHRSupdate thread for ring"<<endl;

    std::thread middle(&GRAlgorithm::madgwickUpdateBuffer, this, &inDevice->middle,  &outDevice->middle, freqCallibration, flag);
    middle.detach();
    std::cout<<"run MadgwickAHRSupdate thread for middle"<<endl;

    std::thread index(&GRAlgorithm::madgwickUpdateBuffer, this, &inDevice->index,  &outDevice->index, freqCallibration, flag);
    index.detach();
    std::cout<<"run MadgwickAHRSupdate thread for index"<<endl;

    std::thread thumb(&GRAlgorithm::madgwickUpdateBuffer, this, &inDevice->thumb,  &outDevice->thumb, freqCallibration, flag);
    thumb.detach();
    std::cout<<"run MadgwickAHRSupdate thread for thumb"<<endl;

    std::thread palm(&GRAlgorithm::madgwickUpdateBuffer, this, &inDevice->palm,  &outDevice->palm, freqCallibration, flag);
    palm.detach();
    std::cout<<"run MadgwickAHRSupdate thread for palm"<<endl;
}
