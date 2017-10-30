#ifndef GR_GRT
#define GR_GRT

#include <iostream>
#include <string>

#include "GRT/GRT.h"

#include "grDevice.h"

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
		void setDatasetProperties(std::string, std::string, std::string, int);//properties of dataset
		void setNextLabel();//nex label of dataset in training data 
		void clearTrainingData(); //clear
        
        bool addSample(std::vector<double>* , std::vector<double>* );
		bool pushGesture();//push dataset to DTW //TODO vector trajectory & quanternion
		bool saveDataset();//saving in path 
       

private:


		GRT::DTW _dtw;//dtw object
        GRT::TimeSeriesClassificationData _testData;//test data

        double _testAccuracy = 0.0;//

        GRT::UINT _gestureLabel;
        int _dimensions;
        GRT::MatrixDouble _gestureSample;
        GRT::TimeSeriesClassificationData _trainingData;
        std::string _fileProp;
};
#endif
