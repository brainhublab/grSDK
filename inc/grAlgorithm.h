#ifndef GR_ALGORITHM
#define GR_ALGORITHM

#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <deque>
#include <math.h>
#include <thread>

//#include <eigen3/Eigen/Dense>
//#include <eigen3/Eigen/Geometry>

#include "GRT/GRT.h"

#include <grDevice.h>
using namespace std;
//using namespace Eigen;

class GRAlgorithm 
{

    public:
        GRAlgorithm();
        ~GRAlgorithm();
        GRAlgorithm(const GRAlgorithm& );
        GRAlgorithm& operator=(const GRAlgorithm&);
        void grInitAlgorithms();
        //madgwick
        void madgwickUpdateBuffer(imu*, std::deque<std::vector<float>>*, int, std::string flag);
        void madgwickUpdateThr(device_t*, alg_device_t*, int, std::string flag);
        //gesture recognition
        // Loads training data from .grt file
        bool loadTrainingData(string filepath);
        // Loads test data from .grt file
        bool loadTestData(string filepath);
        // Takes `size`% from training data to use for testing accuracy
        bool setTestDataFromTraining(int size);
        // Traing the algorithm
        bool train();
        // Test the algorithm with testData and return accuracy
        double test();
        double getTestAccuracy();
        // Used for saving/loading trained model of the algorithm
        bool saveModel(string filepath);
        bool loadModel(string filepath);
        // Predictions functions
        bool predict(GRT::MatrixDouble timeseries);
        GRT::UINT getPredictedClassLabel();
        double getMaximumLikelihood();

    private:
        //helper methods
        double constrain(double , double , double );
        double gravity = 256;
        //madgwick algorithm vars and methods
        float q0 , q1, q2, q3;  // quaternion of sensor frame relative to auxiliary frame
        float beta;
        float roll, pitch, yaw;
        std::vector<float> angles;
        char anglesComputed;

        std::vector<float> computeAngles();
        float invSqrt(float x);            // algorithm gain
        void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, 
                float mx, float my, float mz, std::deque<std::vector<float>>*, int, std::string);
        void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az, 
                std::deque<std::vector<float>>*, int, std::string);

        //gesture recognition vars and methods
        GRT::DTW dtw;
        GRT::TimeSeriesClassificationData trainingData;
        GRT::TimeSeriesClassificationData testData;

        double testAccuracy = 0.0;

};


#endif
