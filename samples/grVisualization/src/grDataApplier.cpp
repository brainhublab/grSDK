#include "grDataApplier.h"
//
// GRDataApplier definitions
//


#include "grRender.h"

#include <cmath>

/*void splitSensorData(std::vector<float> quat, double arr[4])
{
	arr[0] = data[1];
   //      int i = 0;
   //      double n;
   //      std::stringstream ss(str);
   //      while(ss >> n)
   //      {
			// arr[i] = n;
   //              i++;
   //      }
};
*/

GRDataApplier::GRDataApplier()
{
    plog::init(plog::debug, "log.csv");
    nodeQuanternion = new std::vector<float>();
    fingers["pinky"] = 0;
    fingers["ring"] = 1;
    fingers["middle"] = 2;
    fingers["index"] = 3;
    fingers["thumb"] = 4;

}

GRDataApplier::~GRDataApplier()
{
    delete nodeQuanternion;
}

bool GRDataApplier::setArm(GRHand *oArm)
{
	arm = oArm;
	return arm != nullptr;
};

bool GRDataApplier::fetchSignal()
{

//	printf("Checking...");
	if(!fetchRunning)
	{
		fetchRunning = true;
        // printf("Yes! Fetching data...\n");
		fetchData();

		fetchRunning = false;
		return true;
	}
    // printf("\nNo! Fetching now...\n");
	return false;
};

bool GRDataApplier::run()
{

#ifdef GR_VISUALIZATION_LOGGING_ENABLED
	printf("DataApplier: setting up fetchtimer...\n");
#endif

printf("Setting up.");
// start timer for fetching data from source to separate copies
    fetchTimer = new QTimer();
	QObject::connect(fetchTimer, SIGNAL(timeout()), this, SLOT(fetchSignal()));
//	QObject::connect(this, SIGNAL(fetchSignal()), this, SLOT(fetchData()));

	fetchTimer->start(fetchFrequency);

    // configuring connection to devices

    std::map<int, device_t> availableDevices;
    availableDevices = conn.getAvalibleDevices();
    for(std::map<int, device_t>::const_iterator it = availableDevices.begin(); it != availableDevices.end(); ++it)
    {
        conn.setActiveDevice(it->first);
        conn.connectSocket(it->first);
        activeDevices[it->first] = it->second;
    }

    alg.setupMadgwick(140, 140, 160, 180, 140, 60);


    return true;
}

bool GRDataApplier::addHistoryData(std::vector<double> quant)
{
    if(targetQuanternionHistory == nullptr)
    {
        return false;
    }
    std::vector<float> q;
    for(int i = 0; i < quant.size(); i++)
    {
        q.push_back((float) quant[i]);
    }
    targetQuanternionHistory->push_back(q);
    return true;
}

bool GRDataApplier::writeQuanternionHistory(std::deque<std::vector<float> > * h)
{
    targetQuanternionHistory = h;
    return true;
}

const std::map<int, device_t> &GRDataApplier::getActiveDevices() const
{
    return activeDevices;
}

// Quaternion (x, y, z, w)
void quaternionToRotation(std::vector<float>& quaternion,
                          GLfloat *rotation)//iz kvanteriona delaet matricu rotacii GOVNO glMultMatrixf(matrix)
{
	// Normalize quaternion
	float magnitude = sqrt(quaternion[0] * quaternion[0] +
	                       quaternion[1] * quaternion[1] +
	                       quaternion[2] * quaternion[2] +
	                       quaternion[3] * quaternion[3]);
	for (int i = 0; i < 4; ++i)
	{
		quaternion[i] /= magnitude;
	}

	double xx = quaternion[0] * quaternion[0], xy = quaternion[0] * quaternion[1],
			xz = quaternion[0] * quaternion[2], xw = quaternion[0] * quaternion[3];
	double yy = quaternion[1] * quaternion[1], yz = quaternion[1] * quaternion[2],
			yw = quaternion[1] * quaternion[3];
	double zz = quaternion[2] * quaternion[2], zw = quaternion[2] * quaternion[3];

	// Column major order
	rotation[0] = 1.0f - 2.0f * (yy + zz);
	rotation[1] = 2.0f * (xy - zw);
	rotation[2] = 2.0f * (xz + yw);
	rotation[3] = 0;

	rotation[4] = 2.0f * (xy + zw);
	rotation[5] = 1.0f - 2.0f * (xx + zz);
	rotation[6] = 2.0f * (yz - xw);
	rotation[7] = 0;

	rotation[8] = 2.0f * (xz - yw);
	rotation[9] = 2.0f * (yz + xw);
	rotation[10] = 1.0f - 2.0f * (xx + yy);
	rotation[11] = 0;

	rotation[12] = 0;
	rotation[13] = 0;
	rotation[14] = 0;
	rotation[15] = 1;
}

bool GRDataApplier::processMsg(std::string nodeName)
{
    if(!msg.imus[nodeName]->acc.empty())
    {
        alg.madgwickUpdate(&msg, &alg_msg, 1, "flag");
        std::cout<<"QUANTERNION---->";

        for(int i = 0; i < 4; i++)
        {
            std::cout << (*alg_msg.nodes[nodeName])[i];
        }
        std::cout<<std::endl;

        if(nodeName == "palm")
        {
            applyToHand(*alg_msg.nodes[nodeName]);
            addHistoryData(*alg_msg.nodes[nodeName]);
        }
        else
        {
            applyToFinger(*alg_msg.nodes[nodeName], fingers[nodeName]);
        }
        msg.imus[nodeName]->gyro.clear();
        msg.imus[nodeName]->acc.clear();
        msg.imus[nodeName]->mag.clear();
        return true;
    }

    return false;
}

bool GRDataApplier::fetchData()
{
    std::vector<double> v;
    v.push_back(1);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0);
    applyToHand(v);
    v.clear();
    v.push_back(0.985);
    v.push_back(0);
    v.push_back(0);
    v.push_back(0.014);
    applyToFinger(v, 0);
    applyToFinger(v, 1);
    applyToFinger(v, 2);
    applyToFinger(v, 3);
    applyToFinger(v, 4);
    printf("hello");
    if(activeDevices.empty())
    {
//        printf("No active devices.");
        return false;
    }

    for(std::map<int, device_t>::const_iterator it = activeDevices.begin(); it != activeDevices.end(); ++it)
    {
        conn.getData(it->first, &msg);
        processMsg("palm");
        processMsg("pinky");
        processMsg("ring");
        processMsg("middle");
        processMsg("index");
        processMsg("thumb");
    }

//    applyToFinger(algDev.pinky, 0);
//	applyToFinger(algDev.ring, 1);
//	applyToFinger(algDev.middle, 2);
//	applyToFinger(algDev.index, 3);
//	applyToFinger(algDev.thumb, 4);

//	applyToHand(algDev.palm);
//	emit fetchSignal();
    return true;
}

bool GRDataApplier::setAlgorithms()
{
    algorithms["pinky"] = GRAlgorithm();
    algorithms["ring"] = GRAlgorithm();
    algorithms["middle"] = GRAlgorithm();
    algorithms["index"] = GRAlgorithm();
    algorithms["thumb"] = GRAlgorithm();
    algorithms["palm"] = GRAlgorithm();

    return true;
}

bool GRDataApplier::applyToHand(std::vector<double> &quant)
{
    if (!quant.empty())
	{
        nodeQuanternion->clear();
        for(int i = 0; i < quant.size(); i++)\
        {

            nodeQuanternion->push_back((float)quant[i]);
        }

        GLfloat mat[16];
        quaternionToRotation(*nodeQuanternion, mat);

        printf("Hand Q: %f %f %f %f\n",  (*nodeQuanternion)[0], (*nodeQuanternion)[1], (*nodeQuanternion)[2], (*nodeQuanternion)[3]);
        arm->bendHandWithMatrix(mat);

        //arm->bendHand((*nodeQuanternion)[2], (*nodeQuanternion)[1], (*nodeQuanternion)[3]);
        if(!prevQuants[5].empty())
		{
			prevQuants[5].clear();
		}
        prevQuants[5] = *nodeQuanternion;

//        node.pop_front();
        (*nodeQuanternion).clear();
		return true;
	}

	return false;
};


float getYaw(std::vector<float> &q)
{
    float roll = 0.0f;
    float pitch = 0.0f;
    float yaw = 0.0f;

    roll = atan2f(q[0]*q[1] + q[2]*q[3], 0.5f - q[1]*q[1] - q[2]*q[2]);
//    pitch = asinf(-2.0*(q1*q3 - q0*q2));
    yaw = atan2f(q[1]*q[2] + q[0]*q[3], 0.5f - q[2]*q[2] - q[3]*q[3]);

    roll = roll * 57.29578f;
    pitch = pitch * 57.29578f;
    yaw = yaw * 57.29578f + 180.0f;
    return yaw;
}


bool GRDataApplier::applyToFinger(std::vector<double> &node, int index)
{
	//printf("Here we are");
    if (!node.empty())
	{
        nodeQuanternion->clear();
        for(int i = 0; i < node.size(); i++)\
        {

            nodeQuanternion->push_back((float)node[i]);
        }

        GLfloat mat[16];

		float yaw = getYaw(*nodeQuanternion);

        float palmYaw = 0.0f;



        printf("This is yaw: %f, this is difference: %f\n", yaw, (palmYaw-yaw));


        //if(palmYaw == 0.f || ((palmYaw-yaw) > -100.f && (palmYaw-yaw) < 30.f))// || ( yaw > 250 && (palmYaw-yaw) < -70.f && (palmYaw-yaw) > 30.f))

        {
		quaternionToRotation(*nodeQuanternion, mat);
        arm->bendFingerWithMatrix(index, mat);
        }

        (*nodeQuanternion).clear();

        return true;
	}
	return false;
}
/*bool GRDataApplier::applyToFinger(std::deque<std::vector<float>> &node, int index)
{
    //printf("Here we are");
    if (!node.empty() && !node.front().size() < 4)
    {
        nodeQuanternion = &node.front();
        GLfloat mat[16];

        float yaw = getYaw(*nodeQuanternion);

        float palmYaw = 0.0f;



        printf("This is yaw: %f, this is difference: %f\n", yaw, (palmYaw-yaw));


        //if(palmYaw == 0.f || ((palmYaw-yaw) > -100.f && (palmYaw-yaw) < 30.f))// || ( yaw > 250 && (palmYaw-yaw) < -70.f && (palmYaw-yaw) > 30.f))

        {
        quaternionToRotation(*nodeQuanternion, mat);
        arm->bendFingerWithMatrix(index, mat);
        }

        node.pop_front();
        (*nodeQuanternion).clear();

        return true;
    }
    return false;
}
*/
/*hack	std::cout<<"Pop front	:";
		for(auto a : node.front())
		{
			std::cout<<a;
		}
		std::cout<<std::endl;
		nodeQuanternion = &node.front();
*/
		// get only z rotation
/*hack        (*nodeQuanternion)[1] = 0;
		(*nodeQuanternion)[2] = 0;
		double mag = sqrt(  ((*nodeQuanternion)[0])*((*nodeQuanternion)[0]) +
							((*nodeQuanternion)[3])*((*nodeQuanternion)[3]));
		(*nodeQuanternion)[0] /= mag;
		(*nodeQuanternion)[3] /= mag;


		if(index == 4)
			printf("This is yaw: %f\n", getYaw(*nodeQuanternion));
//            printf("Bending thumb on %f angles!\n", (*nodeQuanternion)[3]);
//            printf("%d euler: %f %f %f %f\n",index,  (*nodeQuanternion)[0], (*nodeQuanternion)[1], (*nodeQuanternion)[2], (*nodeQuanternion)[3]);


		bool moveit = false;
hack*/
		/*if(!prevQuants[index].empty())
		{
			float palmQuantDiff = getYaw(prevQuants[5]) - getYaw(algDev.palm.front());
			float fingerQuantDiff = getYaw(prevQuants[index]) - getYaw(*nodeQuanternion);
			if((palmQuantDiff > fingerQuantDiff && (abs(palmQuantDiff-fingerQuantDiff) > 1.f)) ||
					(palmQuantDiff < fingerQuantDiff && (abs(fingerQuantDiff-palmQuantDiff) > 1.f)))
			{
				moveit = true;
			}
		}*/
//if( getYaw(*nodeQuanternion) > getYaw(algDev.palm.front()))

		//if( getYaw(*nodeQuanternion) > getYaw(algDev.palm.front()))
		//{
			//printf("Moooving fingers");
			// limit
	//		float diff = getYaw(algDev.palm.front()) - getYaw(*nodeQuanternion);
	//		float diff2 = getYaw(*nodeQuanternion) - getYaw(algDev.palm.front());
				//printf("\n============\nHere is diff: %f, Yaw of %d finger: %f, Yaw of Palm: %f \n\n", diff, index, getYaw(*nodeQuanternion), getYaw(algDev.palm.front()));
					//if(getYaw(*nodeQuanternion) < 300.f && getYaw(*nodeQuanternion) > 100.f)
			//float d = -60;
//hack			//if(diff < 15 && diff > -90)
//hack			if(getYaw(*nodeQuanternion) < 280.f && getYaw(*nodeQuanternion) > 120.f)// && diff < 15.f && diff > -90)
//			{

		//		GLfloat mat[16];
			//	quaternionToRotation(*nodeQuanternion, mat);
				//arm->bendFingerWithMatrix(index, mat);
//			}
		//}
//        arm->bendFirstPhalange(index, 0*(*nodeQuanternion)[3], 0, 180);
/*hack		std::cout<<"Pop front: ";
		for(auto a : node.front())
		{
		   std::cout<<a;
		}
		std::cout<<std::endl;

	 if(!prevQuants[index].empty())
		{

			prevQuants[index].clear();
		}
		prevQuants[index] = node.front();
hack*/
