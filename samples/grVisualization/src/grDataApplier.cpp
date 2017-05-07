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
}

GRDataApplier::~GRDataApplier()
{}

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
	for(int i = 0; i < 64; i++)
	{
		printf(".");
		usleep(20);
        conn.getData(&dev);
		if(!dev.pinky.acc.empty())
			dev.pinky.acc.pop_front();
		if(!dev.pinky.gyro.empty())
			dev.pinky.gyro.pop_front();
		if(!dev.pinky.mag.empty())
			dev.pinky.mag.pop_front();

		if(!dev.ring.acc.empty())
			dev.ring.acc.pop_front();
		if(!dev.ring.gyro.empty())
			dev.ring.gyro.pop_front();
		if(!dev.ring.mag.empty())
			dev.ring.mag.pop_front();

		if(!dev.middle.acc.empty())
			dev.middle.acc.pop_front();
		if(!dev.middle.gyro.empty())
			dev.middle.gyro.pop_front();
		if(!dev.middle.mag.empty())
			dev.middle.mag.pop_front();

		if(!dev.index.acc.empty())
			dev.index.acc.pop_front();
		if(!dev.index.gyro.empty())
			dev.index.gyro.pop_front();
		if(!dev.index.mag.empty())
			dev.index.mag.pop_front();

		if(!dev.thumb.acc.empty())
			dev.thumb.acc.pop_front();

		if(!dev.thumb.gyro.empty())
			dev.thumb.gyro.pop_front();
		if(!dev.thumb.mag.empty())
			dev.thumb.mag.pop_front();


		if(!dev.palm.acc.empty())
			dev.palm.acc.pop_front();
		if(!dev.palm.gyro.empty())
			dev.palm.gyro.pop_front();
		if(!dev.palm.mag.empty())
			dev.palm.mag.pop_front();

		printf(".");
    }
	// start timer for fetching data from source to separate copies
	fetchTimer = new QTimer();
	QObject::connect(fetchTimer, SIGNAL(timeout()), this, SLOT(fetchSignal()));
//	QObject::connect(this, SIGNAL(fetchSignal()), this, SLOT(fetchData()));

	fetchTimer->start(fetchFrequency);

	return true;
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


bool GRDataApplier::fetchData()
{
    conn.getData(&dev);
	//usleep(100);
    alg1.madgwickUpdateBuffer(&dev.pinky, &algDev.pinky, 140, "QATERNION"); // QUATERION"
    alg2.madgwickUpdateBuffer(&dev.ring, &algDev.ring, 140, "QATERNION");
    alg3.madgwickUpdateBuffer(&dev.middle, &algDev.middle, 160, "QATERNION");
    alg4.madgwickUpdateBuffer(&dev.index, &algDev.index, 180, "QATERNION");
    alg5.madgwickUpdateBuffer(&dev.thumb, &algDev.thumb, 120, "QATERNION");
    alg.madgwickUpdateBuffer(&dev.palm, &algDev.palm, 40, "QATERNION");


    applyToFinger(algDev.pinky, 0);
	applyToFinger(algDev.ring, 1);
	applyToFinger(algDev.middle, 2);
	applyToFinger(algDev.index, 3);
	applyToFinger(algDev.thumb, 4);

	applyToHand(algDev.palm);
//	emit fetchSignal();
	return true;
}

bool GRDataApplier::applyToHand(std::deque<std::vector<float>> &node)
{
	if (!node.empty())
	{
        nodeQuanternion = &node.front();

        GLfloat mat[16];
        quaternionToRotation(*nodeQuanternion, mat);

//        printf("Hand Q: %f %f %f %f\n",  (*nodeQuanternion)[0], (*nodeQuanternion)[1], (*nodeQuanternion)[2], (*nodeQuanternion)[3]);
        arm->bendHandWithMatrix(mat);

        //arm->bendHand((*nodeQuanternion)[2], (*nodeQuanternion)[1], (*nodeQuanternion)[3]);
        if(!prevQuants[5].empty())
            prevQuants[5].clear();
        prevQuants[5] = *nodeQuanternion;

        node.pop_front();
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


bool GRDataApplier::applyToFinger(std::deque<std::vector<float>> &node, int index)
{
    printf("Here we are");
    if (!node.empty() && !node.front().size() < 4)
	{
        printf("Pop front:");
        for(auto a : node.front())
        {
            printf("%f", a);
        }
        nodeQuanternion = &node.front();

        // get only z rotation
        (*nodeQuanternion)[1] = 0;
        (*nodeQuanternion)[2] = 0;
        double mag = sqrt(  ((*nodeQuanternion)[0])*((*nodeQuanternion)[0]) +
                            ((*nodeQuanternion)[3])*((*nodeQuanternion)[3]));
        (*nodeQuanternion)[0] /= mag;
        (*nodeQuanternion)[3] /= mag;


        if(index == 4)
            printf("This is yaw: %f\n", getYaw(*nodeQuanternion));
//            printf("Bending thumb on %f angles!\n", (*nodeQuanternion)[3]);
//            printf("%d euler: %f %f %f %f\n",index,  (*nodeQuanternion)[0], (*nodeQuanternion)[1], (*nodeQuanternion)[2], (*nodeQuanternion)[3]);


        if( getYaw(*nodeQuanternion) > getYaw(algDev.palm.front()))
        {
            printf("Moooving fingers");
            // limit
            if(getYaw(*nodeQuanternion) < 250.f && getYaw(*nodeQuanternion) > 180.f)
            {

            GLfloat mat[16];
            quaternionToRotation(*nodeQuanternion, mat);
            arm->bendFingerWithMatrix(index, mat);
            }
        }
//        arm->bendFirstPhalange(index, 0*(*nodeQuanternion)[3], 0, 180);
        printf("Pop front:");
        for(auto a : node.front())
        {
            printf("%f", a);
        }


        if(!prevQuants[index].empty())
        {

            prevQuants[index].clear();
        }
        prevQuants[index] = node.front();
        node.pop_front();
        (*nodeQuanternion).clear();
		return true;
	}
	return false;
}
