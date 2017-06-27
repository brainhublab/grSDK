#ifndef GR_GRT
#define GR_GRT

#include <iostream>
#include <string>

#include "GRT/GRT.h"

#include "grDevice.h"
#include "grDataAttributes.h"

class GRGrt
{
public:

		GRGrt();
		~GRGrt();
		GRGrt(const GRGrt& );
		GRGrt& operator=(const GRGrt& );
		bool loadTrainingData(std::string filepath);
        // Loads test data from .grt file
        bool loadTestData(std::string filepath);
        // Takes `size`% from training data to use for testing accuracy
        bool setTestDataFromTraining(int size);
        // Traing the algorithm
        bool train();
        // Test the algorithm with testData and return accuracy
        double test();
        double getTestAccuracy();
        // Used for saving/loading trained model of the algorithm
        bool saveModel(std::string filepath);
        bool loadModel(std::string filepath);
        // Predictions functions
        bool predict(GRT::MatrixDouble timeseries);
        GRT::UINT getPredictedClassLabel();
        double getMaximumLikelihood();

        //utils
        void setDataSetSensors(std::vector<std::string>, std::string);
		void setDatasetProperties(std::string, std::string, std::string, std::string);
		void setNextLabel(std::string);
		void clearTrainingData(std::string); 

		bool pushDatasetDTW(gr_message*);
		bool pushDatasetHMM(gr_message*);

		bool saveDataset(std::string);

private:


		GRT::DTW dtw;
        GRT::TimeSeriesClassificationData trainingData;
        GRT::TimeSeriesClassificationData testData;

        double testAccuracy = 0.0;

        //Utils
        data_attributes datasetDTW;
    	data_attributes datasetHMM;
};
#endif
