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

		GRGrt();//constructor
		~GRGrt();//destructor
		GRGrt(const GRGrt& );//copy constructor
		GRGrt& operator=(const GRGrt& );//operator=
		bool loadTrainingData(std::string filepath);//loading training data by path
        // Loads test data from .grt file
        bool loadTestData(std::string filepath);//loading test data by path
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
        void setDataSetSensors(std::vector<std::string>, std::string);//set sensor for dataset
		void setDatasetProperties(std::string, std::string, std::string, std::string);//properties of dataset
		void setNextLabel(std::string);//nex label of dataset in training data 
		void clearTrainingData(std::string); //clear

		bool pushDatasetDTW(gr_message*);//push dataset to DTW 
		bool pushDatasetHMM(gr_message*);//push dataset to HMM

		bool saveDataset(std::string);//saving in path 

private:


		GRT::DTW _dtw;//dtw object
        GRT::TimeSeriesClassificationData _trainingData;//training data 
        GRT::TimeSeriesClassificationData _testData;//test data

        double _testAccuracy = 0.0;//

        //Utils
        data_attributes _datasetDTW;//object for DTW
    	data_attributes _datasetHMM;//objects for HMM
};
#endif
