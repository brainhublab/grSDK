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

bool GRDataApplier::run()
{

#ifdef GR_VISUALIZATION_LOGGING_ENABLED
	printf("DataApplier: setting up fetchtimer...\n");
#endif
	// start timer for fetching data from source to separate copies
	fetchTimer = new QTimer();
	QObject::connect(fetchTimer, SIGNAL(timeout()), this, SLOT(fetchData()));
	fetchTimer->start(fetchFrequency);

	return true;
}

// Quaternion (x, y, z, w)
void quaternionToRotation(std::vector<float> quaternion,
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


void GRDataApplier::fetchData()
{
	conn.connectAndRead(&dev);
	//sleep(1);
	alg.madgwickUpdateBuffer(&dev.palm, &algDev.palm, 40);
	alg1.madgwickUpdateBuffer(&dev.pinky, &algDev.pinky, 120);
	alg2.madgwickUpdateBuffer(&dev.ring, &algDev.ring, 140);
	alg3.madgwickUpdateBuffer(&dev.middle, &algDev.middle, 160);
	alg4.madgwickUpdateBuffer(&dev.index, &algDev.index, 180);
	alg5.madgwickUpdateBuffer(&dev.thumb, &algDev.thumb, 120);


	std::vector<float>& palmQuant = algDev.palm.front();
	if (!algDev.palm.empty())
	{
		GLfloat mat[16];
		quaternionToRotation(palmQuant, mat);
		arm->bendHandWithMatrix(mat);
		algDev.palm.pop_front();
	}
	if (!algDev.pinky.empty())
	{
		std::vector<float>& d = algDev.pinky.front();
		// TODO: limits


//		if(d[0] < 0.97f)
//			d[0] = 0.97f;

//		if(d[3] < -0.205f)
//			d[3] = -0.205f;
//		if(d[3] > 0.750f)
//			d[3] = 0.750f;


		GLfloat mat[16];
		quaternionToRotation(d, mat);
		arm->bendFingerWithMatrix(0, mat);
		algDev.pinky.pop_front();
	}
	if (!algDev.ring.empty())
	{
		std::vector<float>& d = algDev.ring.front();

		// TODO: limits
//		if(d[0] < 0.97f)
//			d[0] = 0.97f;

//		if(d[3] < -0.205f)
//			d[3] = -0.205f;
//		if(d[3] > 0.750f)
//			d[3] = 0.750f;

		GLfloat mat[16];
		quaternionToRotation(d, mat);
		arm->bendFingerWithMatrix(1, mat);
		algDev.ring.pop_front();
	}
	if (!algDev.middle.empty())
	{
		std::vector<float>& d = algDev.middle.front();

		if (d[0] < 0.97f)
			d[0] = 0.97f;

		if (d[3] < -0.205f)
			d[3] = -0.205f;
		if (d[3] > 0.750f)
			d[3] = 0.750f;

		GLfloat mat[16];
		quaternionToRotation(d, mat);
		arm->bendFingerWithMatrix(2, mat);
		algDev.middle.pop_front();
	}
	if (!algDev.index.empty())
	{
		std::vector<float>& d = algDev.index.front();

		if (d[0] < 0.97f)
			d[0] = 0.97f;
		if (d[3] < -0.205f)
			d[3] = -0.205f;
		if (d[3] > 0.750f)
			d[3] = 0.750f;

		GLfloat mat[16];
		quaternionToRotation(d, mat);
		arm->bendFingerWithMatrix(3, mat);
		algDev.index.pop_front();
	}
	if (!algDev.thumb.empty())
	{
		std::vector<float>& d = algDev.thumb.front();

		if (d[0] < 0.97f)
			d[0] = 0.97f;

		if (d[3] < -0.205f)
			d[3] = -0.205f;
		if (d[3] > 0.750f)
			d[3] = 0.750f;


		GLfloat mat[16];
		quaternionToRotation(d, mat);
		arm->bendFingerWithMatrix(4, mat);
		algDev.thumb.pop_front();
	}
}