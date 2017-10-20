#include "grDataApplier.h"
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

//
// GRDataApplier definitions
//

GRDataApplier::GRDataApplier()
{
    plog::init(plog::debug, "log.csv");
    nodeQuanternion = new std::vector<float>();
    fingers["pinky"] = 0;
    fingers["ring"] = 1;
    fingers["middle"] = 2;
    fingers["index"] = 3;
    fingers["thumb"] = 4;
    alg.setupMadgwick(2000, 2000, 2000, 2000, 2000, 1499); //need to check

}

GRDataApplier::~GRDataApplier()
{
	thread.wait();
    delete nodeQuanternion;
}


// sets OpenGL model of arm for data applying
bool GRDataApplier::setArm(GRHand *oArm)
{
	arm = oArm;
	return arm != nullptr;
};


// sets GR controller for accessing arm data
bool GRDataApplier::assignDeviceName(std::string name)
{
	deviceName = name;
	return true;
}
bool GRDataApplier::setDeviceId(int id)
{
	if(id < 0) return false;
	deviceId = id;
	return true;
}

bool GRDataApplier::setConnection(GRConnection* c)
{
	conn = c;
	return true;
}

// runs fetchData
bool GRDataApplier::fetchSignal()
{
    if(!fetchRunning)
	{
        // ok, another fetch is not running at the moment
		fetchRunning = true;
        fetchData();

		fetchRunning = false;
		return true;

	}
    return false;
};

bool GRDataApplier::runDataReading()
{
	printf("Inside rundata  reading\n");
	fetchData();
	//
		// setting up fetching function calls
}
// method searches GR devices and connects to them
bool GRDataApplier::run()
{
#ifdef GR_VISUALIZATION_LOGGING_ENABLED
	printf("DataApplier: setting up fetchtimer...\n");
#endif
	if( arm == nullptr )
	{
		printf("Arm pointer is not provided!\n");
		return false;
	}
    if(deviceName == "GR[L]")
    {
	 leftFactor = -1;
    }
    else if(deviceName == "GR[R]")
    {
    	rightFactor = -1;
    }
	 /* 
	for( int i = 10; i != 0; i-- )
	{
		conn->getData(deviceId, &msg);
	}*/
 	moveToThread(&thread);
	printf("moving to thread");
	conn->connectSocket(deviceId);
	connect(&thread, SIGNAL(started()), this, SLOT(runDataReading()));
	thread.start();
    return true;
}

/*
 * quant is quaternion vector with length 4
 * pushes quant to tagretQuaternion history
*/
bool GRDataApplier::addHistoryData(std::vector<double> quant)
{
    if(targetQuanternionHistory == nullptr)
    {
        return false;
    }
    // TODO: this for cycle is seamless
    std::vector<float> q;
    q.assign(quant.begin(), quant.end());
    targetQuanternionHistory->push_back(q);
    return true;
}

/*
 * h is pointer to deque of quaternions
 * sets deque pointer for history writing
*/
bool GRDataApplier::writeQuanternionHistory(std::deque<std::vector<float> > * h)
{
    targetQuanternionHistory = h;
    return true;
}

bool changeFingerData(imu* finger)
{

	finger->gyro[2] = -finger->gyro[1];
	finger->gyro[0] = 0;
	finger->gyro[1] = 0;

	finger->acc[2] = -finger->acc[1];
	finger->acc[0] = 0;
	finger->acc[1] = 0;

	finger->mag[2] = -finger->mag[1];
	finger->mag[0] = 0;
	finger->mag[1] = 0;

	return true;
}
/*
 * gets new data from connection and runs processing of data for each hand node
*/
bool GRDataApplier::fetchData() // get data and call processMsg
{
	while(true)
	{
	conn->getData(deviceId, &msg); // read data to msg variable
	if(msg.palm.empty())
	{
		printf("NO data from %d\n", deviceId);
		continue;
	}


//	printf("got data from %d\n", deviceId);

	double tmp;
// palm
	tmp = msg.palm.gyro[0];
	msg.palm.gyro[0] = leftFactor*msg.palm.gyro[2];
	//msg.palm.gyro[1] = msg.palm.gyro[1];
	msg.palm.gyro[2] = rightFactor*tmp;

      	tmp = msg.palm.acc[0];
	msg.palm.acc[0] = leftFactor*msg.palm.acc[2];
	//msg.palm.acc[1] = msg.palm.acc[1];
	msg.palm.acc[2] = rightFactor*tmp;

      	tmp = msg.palm.mag[0];
	msg.palm.mag[0] = leftFactor*msg.palm.mag[2];
	//msg.palm.mag[1] = msg.palm.mag[1];
	msg.palm.mag[2] = rightFactor*tmp;
	// fingers
/*
	changeFingerData(&msg.pinky);
	changeFingerData(&msg.ring);
	changeFingerData(&msg.middle);
	changeFingerData(&msg.index);
	changeFingerData(&msg.thumb);
*/	
	// for each node prcess this msg
        processMsg("palm");
        processMsg("pinky");
        processMsg("ring");
        processMsg("middle");
        processMsg("index");
        processMsg("thumb");
	}
	//printf("Processed data from %d\n", deviceId);
	return true;
}

/*
 * nodename is "palm" or one string from fingers map, for accessing proper finger index
 * sets deque pointer for history writing
*/
bool GRDataApplier::processMsg(std::string nodeName)
{
    std::cout << "Processing msg for " << nodeName << std::endl;
    if(!msg.imus[nodeName]->acc.empty()) //check if msg has data for current node
    {
        alg.madgwickUpdate(&msg, &alg_msg, 1, "flag");
        std::cout << "\tInside if " << nodeName << std::endl;
//        std::cout<<"QUANTERNION---->";

//        for(int i = 0; i < 4; i++)
//        {
//            std::cout << (*alg_msg.nodes[nodeName])[i];
//        }
//        std::cout<<std::endl;

        // Apply this data to OpenGL 3d-model
        if(nodeName == "palm")
        {
		//get new position;
    		if(withTrajectory)
			    last_position = trajectory.getNewPosByRunge(msg.palm.acc, alg_msg.palm, msg.palm.time_stamp);
		    else
		    {
			last_position;
			last_position[0] = 0;
			last_position[1] = 0;
			last_position[2] = 0;
		}
			moveHand(last_position);
		    if(withRotations)
		    {
			applyToHand(*alg_msg.nodes[nodeName]);
        		addHistoryData(*alg_msg.nodes[nodeName]);
        	}
	    }
        else
        {
		    if(withRotations){
                std::cout << "\t\t Call applyToFinger " << nodeName << std::endl;
		    	applyToFinger(*alg_msg.nodes[nodeName], fingers[nodeName]);
        
		    }
	    }
        // clear data
        msg.imus[nodeName]->gyro.clear();
        msg.imus[nodeName]->acc.clear();
        msg.imus[nodeName]->mag.clear();
        return true;
    }

    return false;
}

/*
 * Gonverts quaternioin to matrix rotation | Quaternion (x, y, z, w)
*/
void quaternionToRotation(std::vector<float>& quaternion,
                          GLfloat *rotation)
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



Eigen::Quaterniond quatMult(Eigen::Quaterniond q1, Eigen::Quaterniond q2) {
    Eigen::Quaterniond resultQ;
    resultQ.setIdentity();
    resultQ.w() = q1.w() * q2.w() - q1.vec().dot(q2.vec());
    resultQ.w() = q1.w() * q2.w() - q1.vec().dot(q2.vec());
    resultQ.vec() = q1.w() * q2.vec() + q2.w() * q1.vec() + q1.vec().cross(q2.vec());
    return resultQ;
    return resultQ;
}

/* 
 * moves OpenGL arm in a scene to new position (works bad now) TODO
 */
bool GRDataApplier::moveHand(std::vector<double>& position)
{
	if(position.empty()) return false;
    printf("position last: %f %f %f \n", last_position[0], last_position[1], last_position[2]);
    arm->setHandPosition(position[0], position[1], position[2]);
	return true;
}



/*
 * q is quaternion
 * returns Euler yaw from q
*/
float getYaw(std::vector<float> &q)
{
   // float roll = 0.0f;
 //   float pitch = 0.0f;
    float yaw = 0.0f;

//    roll = atan2f(q[0]*q[1] + q[2]*q[3], 0.5f - q[1]*q[1] - q[2]*q[2]);
// TODO: WHY?   pitch = asinf(-2.0*(q1*q3 - q0*q2));
    yaw = atan2f(q[1]*q[2] + q[0]*q[3], 0.5f - q[2]*q[2] - q[3]*q[3]);

    //roll = roll * 57.29578f;
    //pitch = pitch * 57.29578f;
    yaw = yaw * 57.29578f + 180.0f;
    return yaw;
}

/*
 * quant is quaternion with size 4
 * rotates palm with this quaternion
*/
bool GRDataApplier::applyToHand(std::vector<double> &quant)
{
    if (!quant.empty())
	{
        nodeQuanternion->clear();
	nodeQuanternion->assign(quant.begin(), quant.end());

        GLfloat mat[16];
        quaternionToRotation(*nodeQuanternion, mat);
	printf("Yaw %f\n", getYaw(*nodeQuanternion));
        //printf("Hand Q: %f %f %f %f\n",  (*nodeQuanternion)[0], (*nodeQuanternion)[1], (*nodeQuanternion)[2], (*nodeQuanternion)[3]);	
	
	if( getYaw(*nodeQuanternion) > 100.f && getYaw(*nodeQuanternion) < 300.f)
	{
		arm->bendHandWithMatrix(mat);
	}
		//arm->bendHand((*nodeQuanternion)[2], (*nodeQuanternion)[1], (*nodeQuanternion)[3]);

        if(!prevQuants[5].empty())
	{
		prevQuants[5].clear();
	}
        prevQuants[5] = *nodeQuanternion;

        (*nodeQuanternion).clear();
	return true;
	}


	return false;
};


/*
 * q is quaternion with size 4, index is figer index (see fingers map)
 * rotates finger with index [index] with q
*/
bool GRDataApplier::applyToFinger(std::vector<double> &q, int index)
{
    if (1)//(!q.empty())
	{
        nodeQuanternion->clear();
	nodeQuanternion->assign(q.begin(), q.end());



        GLfloat mat[16];
/*
	float yaw = getYaw(*nodeQuanternion);
        float palmYaw = 0.0f;
*/

	quaternionToRotation(*nodeQuanternion, mat);

        std::cout<<index<< getYaw(*nodeQuanternion) << std::endl;
	if( getYaw(*nodeQuanternion) > 150.f && getYaw(*nodeQuanternion) < 250.f)
	{
		arm->bendFingerWithMatrix(index, mat);
        std::cout<<index<< getYaw(*nodeQuanternion) << std::endl;
	}
        (*nodeQuanternion).clear();

        return true;
	}
	return false;
}
