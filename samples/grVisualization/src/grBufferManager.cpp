#include "grBufferManager.h"
//
// GRBufferManager definitions
//


#include "grRender.h"

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

void GRBufferManager::fetchData()
{
	conn.connectAndRead(&dev);
	//sleep(1);
	alg.madgwickUpdateBuffer(&dev.palm, &algDev.palm);
	alg1.madgwickUpdateBuffer(&dev.pinky, &algDev.pinky);
	alg2.madgwickUpdateBuffer(&dev.ring, &algDev.ring);
	alg3.madgwickUpdateBuffer(&dev.middle, &algDev.middle);
	alg4.madgwickUpdateBuffer(&dev.index, &algDev.index);
	alg5.madgwickUpdateBuffer(&dev.thumb, &algDev.thumb);

	if(sourceBuffer != nullptr && !sourceBuffer->empty() )
    {

		//double grad2rad = 3.141592/180.0;

        //double d[3] = {0, 0, 0};

        //splitSensorData(sourceBuffer->front(), d);
        std::vector<float> d = sourceBuffer->front();
/*
		std::cout << "READED DATA from source buffer: ";
        for(int i = 0; i < 4; i++)
        {
            std::cout << d[i];
            std::cout << " ";
        }
        std::cout << std::endl;
 */
        GLfloat mat[16];
		quaternionToRotation(d, mat);

        // bend hands
		//grad2rad = 1;
  //widget->leftArm.bendHand(190-d[1]*grad2rad, d[0], d[2]*grad2rad);
        widget->rightArm.bendHandWithMatrix(mat);
        // move front data to different buffers
        firstBuffer->push_back(sourceBuffer->front());
        secondBuffer->push_back(sourceBuffer->front());

        sourceBuffer->pop_front();

        if(isLoggingEnabled) // TODO: move it to proper place
        {
            // LOG(plog::info) << firstBuffer->back() << i;
        }
    }
	if( !algDev.pinky.empty())
	{
		std::vector<float> d = algDev.pinky.front();
		GLfloat mat[16];
		quaternionToRotation(d, mat);
		widget->rightArm.bendFingerWithMatrix(0, mat);
		algDev.pinky.pop_front();


	}
	if( !algDev.ring.empty())
	{
		std::vector<float> d = algDev.ring.front();
		GLfloat mat[16];
		quaternionToRotation(d, mat);
		widget->rightArm.bendFingerWithMatrix(1, mat);
		algDev.ring.pop_front();
	}
	if( !algDev.middle.empty())
	{
		std::vector<float> d = algDev.middle.front();
		GLfloat mat[16];
		quaternionToRotation(d, mat);
		widget->rightArm.bendFingerWithMatrix(2, mat);
		algDev.middle.pop_front();
	}
	if(!algDev.index.empty())
	{
		std::vector<float> d = algDev.index.front();
		GLfloat mat[16];
		quaternionToRotation(d, mat);
		widget->rightArm.bendFingerWithMatrix(3, mat);
		algDev.index.pop_front();
	}
	if(!algDev.thumb.empty())
	{
		std::vector<float> d = algDev.thumb.front();
		GLfloat mat[16];
		quaternionToRotation(d, mat);
		widget->rightArm.bendFingerWithMatrix(4, mat);
		algDev.thumb.pop_front();
	}
}
