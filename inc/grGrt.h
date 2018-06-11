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

  //      GRGrt(std::string);
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
        bool setAlgorithms(std::string, bool);

//        bool setRegressifier(std::string);///TODO WHAAAT

        bool loadTrainingData(std::string);
        /**
         * @brief loads test data from .grt file
         * @param filepath if .grt file
         */
        bool loadTestData(std::string);

        /**
         * @brief initialize algorithms condition
         * @param std:;string type of algorithm for initialization
         */
        bool prepare();
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
        bool predict(GRT::MatrixDouble );
        bool predict(GRT::VectorDouble);
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
        bool setDatasetProperties(std::string, std::string, std::string, int);
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

        ////////////////////TODO
        bool saveDataset(); 

        bool tranQantizationModel(std::string);

        bool quantizeData(std::string);
    private:

        
        //Feauture extraction algorithms 
        /**
         * @brief K meansQuatizer
         * quantize the data from sensors
         */
        GRT::KMeansQuantizer _quantizer;//TODO set right parameters 
        GRT::ClassificationData _quantizerTrainingData;
        /**
         * @brief movement index object
         * for recognition of triger gesture
         */
        GRT::MovementIndex movementIndex(/*windowSize,numDimensions*/);//TODO set right parameters

        /**
         * @brief movement trajectory features 
         * needed for trigering gestures in combination with movement index
         */
        GRT::MovementTrajectoryFeatures  _trajectoryFeatures();//TODO set right parameters 

        //regresion algorithms

        /**
         * @brief Multi layer perception object 
         * to do some cool things
         */
        GRT::MLP _mlp;

        /**
         * @brief training data for ragression algorithms
         *
         */
        GRT::RegressionData _mlpRegressionTrainingData;

        /**
         * @brief test data for regriession algorithm
         *
         */
        GRT::RegressionData _mlpRegressionTestData;
        GRT::RegressionSample _mlpRegressionSample;


        GRT::ClassificationData _mlpClassificationTrainingData;
        GRT::ClassificationData _mlpClassificationTestData;
        GRT::ClassificationSample _mlpClassificationSample;
        std::vector<GRT::ClassificationSample> _mlpClassificationSamples;

        GRT::UINT _mlpInputVectorDimensions; //dimensions of input vector
        GRT::UINT _mlpTargetVectorDimensions; //Target Vector dimensions
        GRT::UINT _mlpTrainingExamplesNumber; 

        GRT::VectorDouble _mlpInputVector;
        GRT::VectorDouble _mlpTargetVector;

        unsigned int _numInputNeurons;
        unsigned int _numHidenNeurons;
        unsigned int _numOutputNeurons;

        bool _setNumNeurons();
        bool _matchDimensionsOfTrainingAndTestData(GRT::RegressionData, GRT::RegressionData);
        GRT::GestureRecognitionPipeline _pipeline;
        bool _enableFeatureExtraction;

        std::string _algType;


        /**
         * @brief multi dimensional regresion object
         */
        //TODO check how to push it in pipeline



        /**
         * @brief dtw object
         */
        GRT::DTW _dtw;

        /**
         * @brief 
         */
        /**
         * @brief test data
         */
        GRT::TimeSeriesClassificationData _dtwTestData;

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
        GRT::TimeSeriesClassificationSample _gestureSample;
        /**
         * @brief
         */
        GRT::TimeSeriesClassificationData _dtwTrainingData;
        /**
         * @brief
         */
        std::string _fileProp;


};
#endif
