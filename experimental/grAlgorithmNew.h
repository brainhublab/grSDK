#ifndef GR_ALGORITHM
#define GR_ALGORITHM

#include "GRT/GRT.h"
#include <string>

using namespace GRT;
using namespace std;

class GRAlgorithm
{
    private:
        DTW dtw;
        TimeSeriesClassificationData trainingData;
        TimeSeriesClassificationData testData;

        double testAccuracy = 0.0;

    public:
        GRAlgorithm();
        ~GRAlgorithm();
        GRAlgorithm(const GRAlgorithm& );
        GRAlgorithm& operator=(const GRAlgorithm&);

        // For setting DTW settings
        bool initialize();

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
        bool predict(MatrixDouble timeseries);
        UINT getPredictedClassLabel();
        double getMaximumLikelihood();
};

#endif
