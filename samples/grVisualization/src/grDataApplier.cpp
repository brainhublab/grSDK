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


// sets OpenGL model of arm for data applying
bool GRDataApplier::setArm(GRHand *oArm)
{
	arm = oArm;
	return arm != nullptr;
};

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


// method searches GR devices and connects to them
bool GRDataApplier::run()
{

#ifdef GR_VISUALIZATION_LOGGING_ENABLED
	printf("DataApplier: setting up fetchtimer...\n");
#endif

//printf("Setting up.");
// start timer for fetching data from source to separate copies
    fetchTimer = new QTimer();
    QObject::connect(fetchTimer, SIGNAL(timeout()), this, SLOT(fetchSignal())); // runs fetchSignal every timeout of fetch timer
//	QObject::connect(this, SIGNAL(fetchSignal()), this, SLOT(fetchData()));

    fetchTimer->start(fetchFrequency); // setting up fetchtimer frequency

    // configuring connection to devices

    std::map<int, device_t> availableDevices;
    availableDevices = conn.getAvalibleDevices();
    for(std::map<int, device_t>::const_iterator it = availableDevices.begin(); it != availableDevices.end(); ++it)
    {
        conn.setActiveDevice(it->first);
        conn.connectSocket(it->first);
        activeDevices[it->first] = it->second; // add information about active devices
    }

    alg.setupMadgwick(140, 140, 160, 180, 140, 60); // setting up algorithm for hand
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
    for(int i = 0; i < quant.size(); i++)
    {
        q.push_back((float) quant[i]);
    }
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


/*
 * returns map of activated devices
*/
const std::map<int, device_t> &GRDataApplier::getActiveDevices() const
{
    return activeDevices;
}


/*
 * gets new data from connection and runs processing of data for each hand node
*/
bool GRDataApplier::fetchData() // get data and call processMsg
{
    // initial testing position of hands
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

    if(activeDevices.empty())
    {
//        printf("No active devices.");
        return false;
    }

    for(std::map<int, device_t>::const_iterator it = activeDevices.begin(); it != activeDevices.end(); ++it)
    {
        conn.getData(it->first, &msg); // read data to msg variable
        // for each node prcess this msg
        processMsg("palm");
        processMsg("pinky");
        processMsg("ring");
        processMsg("middle");
        processMsg("index");
        processMsg("thumb");
    }

    return true;
}

/*
 * nodename is "palm" or one string from fingers map, for accessing proper finger index
 * sets deque pointer for history writing
*/
bool GRDataApplier::processMsg(std::string nodeName)
{
    if(!msg.imus[nodeName]->acc.empty()) //check if msg has data for current node
    {
        alg.madgwickUpdate(&msg, &alg_msg, 1, "flag");
//        std::cout<<"QUANTERNION---->";

//        for(int i = 0; i < 4; i++)
//        {
//            std::cout << (*alg_msg.nodes[nodeName])[i];
//        }
//        std::cout<<std::endl;

        // Apply this data to OpenGL 3d-model
        if(nodeName == "palm")
        {
            applyToHand(*alg_msg.nodes[nodeName]);
            addHistoryData(*alg_msg.nodes[nodeName]);
        }
        else
        {
            applyToFinger(*alg_msg.nodes[nodeName], fingers[nodeName]);
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


/*
 * quant is quaternion with size 4
 * rotates palm with this quaternion
*/
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

        //printf("Hand Q: %f %f %f %f\n",  (*nodeQuanternion)[0], (*nodeQuanternion)[1], (*nodeQuanternion)[2], (*nodeQuanternion)[3]);

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


/*
 * q is quaternion
 * returns Euler yaw from q
*/
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


/*
 * q is quaternion with size 4, index is figer index (see fingers map)
 * rotates finger with index [index] with q
*/
bool GRDataApplier::applyToFinger(std::vector<double> &q, int index)
{
    if (!q.empty())
	{
        nodeQuanternion->clear();
        for(int i = 0; i < q.size(); i++)\
        {

            nodeQuanternion->push_back((float)q[i]);
        }

        GLfloat mat[16];

		float yaw = getYaw(*nodeQuanternion);

        float palmYaw = 0.0f;



//        printf("This is yaw: %f, this is difference: %f\n", yaw, (palmYaw-yaw));


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
