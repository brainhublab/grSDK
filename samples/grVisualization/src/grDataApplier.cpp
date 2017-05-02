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

	printf("Checking...");
	if(!fetchRunning)
	{
		fetchRunning = true;
		printf("Yes! Fetching data...\n");
		fetchData();

		fetchRunning = false;
		return true;
	}
	printf("\nNo! Fetching now...\n");
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
		conn.connectAndRead(&dev);
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
	conn.connectAndRead(&dev);
	//usleep(100);
	alg1.madgwickUpdateBuffer(&dev.pinky, &algDev.pinky, 140);
	alg2.madgwickUpdateBuffer(&dev.ring, &algDev.ring, 140);
	alg3.madgwickUpdateBuffer(&dev.middle, &algDev.middle, 160);
	alg4.madgwickUpdateBuffer(&dev.index, &algDev.index, 180);
	alg5.madgwickUpdateBuffer(&dev.thumb, &algDev.thumb, 120);
	alg.madgwickUpdateBuffer(&dev.palm, &algDev.palm, 40);


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
		arm->bendHandWithMatrix(mat);
		node.pop_front();
		(*nodeQuanternion).clear();
		return true;
	}

	return false;
};

bool GRDataApplier::applyToFinger(std::deque<std::vector<float>> &node, int index)
{
	if (!node.empty())
	{
		nodeQuanternion = &node.front();

		printf("%d quant: %f %f %f %f\n",index,  (*nodeQuanternion)[0], (*nodeQuanternion)[1], (*nodeQuanternion)[2], (*nodeQuanternion)[3]);

		// get only z rotation
		(*nodeQuanternion)[1] = 0;
		(*nodeQuanternion)[2] = 0;
		double mag = sqrt(  ((*nodeQuanternion)[0])*((*nodeQuanternion)[0]) +
							((*nodeQuanternion)[3])*((*nodeQuanternion)[3]));
		(*nodeQuanternion)[0] /= mag;
		(*nodeQuanternion)[3] /= mag;
		// limit
//		if ((*nodeQuanternion)[0] < 0.97f)
//			(*nodeQuanternion)[0] = 0.97f;
//		(*nodeQuanternion)[1] = 0.f ;
//		(*nodeQuanternion)[2] = 0.f;

//		if ((*nodeQuanternion)[3] < -0.205f)
//			(*nodeQuanternion)[3] = -0.205f;
//		if ((*nodeQuanternion)[3] > 0.750f)
//			(*nodeQuanternion)[3] = 0.750f;
//
		GLfloat mat[16];
		quaternionToRotation(*nodeQuanternion, mat);
		arm->bendFingerWithMatrix(index, mat);

		node.pop_front();
		(*nodeQuanternion).clear();
		return true;
	}
	return false;
}
