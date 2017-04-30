#include "grBufferManager.h"
//
// GRBufferManager definitions
//


#include "grRender.h"

#include <cmath>

GRBufferManager::GRBufferManager()
{
   firstBuffer = new std::deque<std::vector<float>>();
   secondBuffer = new std::deque<std::vector<float>>();


   plog::init(plog::debug, "log.csv");
}

GRBufferManager::~GRBufferManager()
{
   delete fetchTimer;
   delete firstBuffer;
   delete secondBuffer;
}

bool GRBufferManager::setupSource()
{


	sourceBuffer = &algDev.palm;

    // start timer for fetching data from source to separate copies
    fetchTimer = new QTimer();
    QObject::connect(fetchTimer, SIGNAL(timeout()), this, SLOT(fetchData()));
	fetchTimer->start(20);

    return true;
}

// Quaternion (x, y, z, w)
void quaternionToRotation(std::vector<float> quaternion, GLfloat *rotation)//iz kvanteriona delaet matricu rotacii GOVNO glMultMatrixf(matrix)
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

//bool qToEuler(std::vector<float> q, GLfloat *rotation) {
//				float heading, attitude, bank;
//				  double sqw = q[3]*q[3];
//				  double sqx = q[0]*q1[0];
//				  double sqy = q[1]*q[1];
//				  double sqz = q[2]*q[2];
//				  double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
//				  double test = q[0]*q[1] + q[2]*q[3];
//				  if (test > 0.499*unit) { // singularity at north pole
//					  heading = 2 * atan2(q[0],q[3]);
//					  attitude = Math.PI/2;
//					  bank = 0;
//				}
//				  else if (test < -0.499*unit) { // singularity at south pole
//					  heading = -2 * atan2(q[0],q1[3]);
//					  attitude = -Math.PI/2;
//					  bank = 0;
//				  }
//				  else
//				  {
//					 heading = atan2(2*q[1]*q[3]-2*q[0]*q[2] , sqx - sqy - sqz + sqw);
//					  attitude = asin(2*test/unit);
//					  bank = atan2(2*q1[0]*q1[3]-2*q1[1]*q1[2] , -sqx + sqy - sqz + sqw);
//				  }

//				  rotation[0] = heading;
//				  rotation[1] = attitude;
//				  rotation[2] = bank;

//				  return true;
//};

void GRBufferManager::fetchData()
{
	conn.connectAndRead(&dev);
	//sleep(1);
	alg.madgwickUpdateBuffer(&dev.palm, &algDev.palm, 40);
	alg1.madgwickUpdateBuffer(&dev.pinky, &algDev.pinky, 120);
	alg2.madgwickUpdateBuffer(&dev.ring, &algDev.ring, 140);
	alg3.madgwickUpdateBuffer(&dev.middle, &algDev.middle, 160);
	alg4.madgwickUpdateBuffer(&dev.index, &algDev.index, 180);
	alg5.madgwickUpdateBuffer(&dev.thumb, &algDev.thumb, 120);


	std::vector<float> palmQuant = algDev.palm.front();
	if( !algDev.palm.empty())
	{
		GLfloat mat[16];
		quaternionToRotation(palmQuant, mat);
		widget->rightArm.bendHandWithMatrix(mat);
		algDev.palm.pop_front();
	}
	if( !algDev.pinky.empty())
	{
		std::vector<float> d = algDev.pinky.front();
	// TODO: limits

		printf("\n pinky:: ");

		for(auto &i : d)
		{
			printf(": %f", i);
		}

//		if(d[0] < 0.97f)
//			d[0] = 0.97f;

//		if(d[3] < -0.205f)
//			d[3] = -0.205f;
//		if(d[3] > 0.750f)
//			d[3] = 0.750f;


		GLfloat mat[16];
		quaternionToRotation(d, mat);
		widget->rightArm.bendFingerWithMatrix(0, mat);
		algDev.pinky.pop_front();
	}
	if( !algDev.ring.empty())
	{
		std::vector<float> d = algDev.ring.front();

		// TODO: limits

		printf("\n ring:: ");

		for(auto &i : d)
		{
			printf(": %f", i);
		}

//		if(d[0] < 0.97f)
//			d[0] = 0.97f;

//		if(d[3] < -0.205f)
//			d[3] = -0.205f;
//		if(d[3] > 0.750f)
//			d[3] = 0.750f;

		GLfloat mat[16];
		quaternionToRotation(d, mat);
		widget->rightArm.bendFingerWithMatrix(1, mat);
		algDev.ring.pop_front();
	}
	if( !algDev.middle.empty())
	{
		std::vector<float> d = algDev.middle.front();

		if(d[0] < 0.97f)
			d[0] = 0.97f;

		if(d[3] < -0.205f)
			d[3] = -0.205f;
		if(d[3] > 0.750f)
			d[3] = 0.750f;

		GLfloat mat[16];
		quaternionToRotation(d, mat);
		widget->rightArm.bendFingerWithMatrix(2, mat);
		algDev.middle.pop_front();
	}
	if(!algDev.index.empty())
	{
		std::vector<float> d = algDev.index.front();

		if(d[0] < 0.97f)
			d[0] = 0.97f;
		if(d[3] < -0.205f)
			d[3] = -0.205f;
		if(d[3] > 0.750f)
			d[3] = 0.750f;

		GLfloat mat[16];
		quaternionToRotation(d, mat);
		widget->rightArm.bendFingerWithMatrix(3, mat);
		algDev.index.pop_front();
	}
	if(!algDev.thumb.empty())
	{
		std::vector<float> d = algDev.thumb.front();

		if(d[0] < 0.97f)
			d[0] = 0.97f;

		if(d[3] < -0.205f)
			d[3] = -0.205f;
		if(d[3] > 0.750f)
			d[3] = 0.750f;


		GLfloat mat[16];
		quaternionToRotation(d, mat);

		widget->rightArm.bendFingerWithMatrix(4, mat);
		algDev.thumb.pop_front();
	}
}
