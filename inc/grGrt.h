#ifndef GR_GRT
#define GR_GRT

#include <iostream>
#include <string>

#include "GRT/GRT.h"

#include "grDevice.h"


/**
 * GR GRT class descr
 */
class GRGrt
{
public:

	/**
	 * @brief constructor
	 */
	GRGrt();
	/**
	 * @brief destructor
	 */
	~GRGrt();
	/**
	 * @brief copy constructor
	 */
	GRGrt(const GRGrt& );
	/**
	 * @brief assigment operator
	 */
	GRGrt& operator=(const GRGrt& );
	/**
	 * @brief loads training data by path
	 * @param filepath is path to training data
	 */
	bool loadTrainingData(std::string filepath);
	/**
	 * @brief loads test data from .grt file
	 * @param filepath if .grt file
         */
	bool loadTestData(std::string filepath);
	/**
	 * @brief Takes size from training data to use for testing accuracy
	 * @param size is a size from training data
         */
	bool setTestDataFromTraining(int size);
	/**
	 * @brief Traing the algorithm
       	 */ 
	bool train();
	/**
	 * @brief Test the algorithm with testData and return accuracy
         */
	double test();
	/**
	 * @brief returns test accuracy
	 */
        double getTestAccuracy();
	/**
	 * @brief saving trained model of the algorithm
         * @param filepath is a path to result-file 
	 */
	bool saveModel(std::string filepath);
	/**
	 * @brief loading trained model of the algorithm
	 * @param filepath is a path to file with model
	 */
        bool loadModel(std::string filepath);
        
	// Predictions functions
	
	/**
	 * @brief
	 */
        bool predict(GRT::MatrixDouble timeseries);
	/**
	 * @brief
	 */
        GRT::UINT getPredictedClassLabel();
	/**
	 * @brief
	 */
        double getMaximumLikelihood();

        //utils
	/**
	 * @brief properties of dataset
	 */
	void setDatasetProperties(std::string, std::string, std::string, int);
	/**
	 * @brief next label of dataset in training data
	 */
	void setNextLabel();
	/**
	 * @brief cleaner
	 */
	void clearTrainingData();
        
        bool addSample(std::vector<double>* , std::vector<double>* );
	/**
	 * @brief push dataset to DTW
	 * TODO vector trajectory & quanternion
	 */
	bool pushGesture();
	/**
	 * @brief saving in path
	 */
	bool saveDataset(); 
       

private:

	/**
	 * @brief dtw object
	 */
	GRT::DTW _dtw;
	/**
	 * @brief test data
	 */
        GRT::TimeSeriesClassificationData _testData;

	/**
	 * @brief
	 */
        double _testAccuracy = 0.0;

	/**
	 * @brief
	 */
        GRT::UINT _gestureLabel;
	/**
	 * @brief
	 */
        int _dimensions;
	/**
	 * @brief
	 */
        GRT::MatrixDouble _gestureSample;
	/**
	 * @brief
	 */
        GRT::TimeSeriesClassificationData _trainingData;
	/**
	 * @brief
	 */
        std::string _fileProp;
};
#endif
