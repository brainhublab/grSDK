#include <grGrt.h>
/*constructor
*/
GRGrt::GRGrt()
{

    //utils
    _gestureLabel = 1;
    //datasetDTW.trainingData.setNumDimensions(datasetDTW.dimensions);
    //datasetDTW.trainingData.setDatasetName("grDTWTrainingData"); //TODO later add relative name 
    //datasetDTW.trainingData.setInfoText("Thisdataset contains some GR data"); //later add relative info text
    _dtw.enableNullRejection(true);
    this->_deadZoneLower = -0.2; //TODO need to be set correctly
    this->_deadZoneUpper = 0.2; //TODO need to be se correctly 
    this->_deadZone = _deadZone(this->_deadZoneLower, this->_deadZoneUpper, 1 /*dimention*/);

    _numInputNeurons = 0;
    _numHidenNeurons = 0;
    _numOutputNeurons = 0;

}
/*destructor
*/
GRGrt::~GRGrt()
{

}

/*copy constructor
*/
GRGrt::GRGrt(const GRGrt &t)
{
    this->_dtw = GRT::DTW(t._dtw);
    this->_trainingData = GRT::TimeSeriesClassificationData(t._trainingData);
    this->_testData = GRT::TimeSeriesClassificationData(t._testData);

    this->_testAccuracy = t._testAccuracy;
}

/*operator =
*/
GRGrt& GRGrt::operator=(const GRGrt& t)
{
    this->_dtw = GRT::DTW(t._dtw);
    this->_trainingData = GRT::TimeSeriesClassificationData(t._trainingData);
    this->_testData = GRT::TimeSeriesClassificationData(t._testData);

    this->_testAccuracy = t._testAccuracy;

}

/*loading of training data from filepath
 * training data structor is used from GRT standarts //TODO later need to be from database
 */
bool GRGrt::loadTrainingData(std::string filepath, std::string alg)
{
    if(alg == "DTW")
    {
        if(!this->_trainingData.load(filepath))
        {
            std::cout << "Failed to load training data!\n";
            return false;
        }

    }
    else if(alg == "MLP")
    {
        if(!this->_regressionTrainingData.loadDatasetFromFile(filepath))
        {
            std::cout<<"ERROR: failed to load training data for: "<<alg
                <<" from path"<< filepath<<std::endl;
            return EXIT_FAILURE;
        }

    }
    else
    {
        std::cout<<"ERROR: no such algorithm with name: "<< alg<<std::endl;
        return EXIT_FAILURE;
    }
    return true;
}

/*loading test data from filepath
*/
bool GRGrt::loadTestData(std::string filepath, std::string alg)
{
    if(alg == "DTW")
    {
        if(!this->_testData.load(filepath))
        {
            std::cout << "Failed to load test data!\n";
            return false;
        }
    }
    else if(alg == "MLP")
    {
        if(!this->_regressionTestData.loadDatasetFromFile(filepath))
        {
            std::cout<<"ERROR: Failed to load test data for "<<alg
                <<"from filepath"<<filepath<<std::endl;
            return EXIT_FAILURE;
        }
        if(this->_regressionTrainingData.getNumInputDimensions() !=this-> _regressionTestData.getNumInputDimensions())
        {
            std::cout<<"ERROR: The number of input dimensions in the training data ( "
                <<this->_regressionTrainingData.getNumInputDimensions()<<" )"
                <<"does not match the number of input dimensions of test data ( "
                <<this->_regressionTestData.getNumInputDimensions()<<" )"std::endl;
            return EXIT_FAILURE;

        }
        if(this->_regressionTrainingData.getNumTargetDimensions() != this->_regressionTestData.getNumTargetDimensions())
        {
            std::cout<<"ERROR: The number of target dimensions of tranining data ("
                <<this->_regressionTrainingData.getNumTargetDimensions()<<" )"
                <<"does not match with target dimensions of test data ( "
                <<this->_regressionTestData.getNumTargetDimensions()<<" )"<<std::endl;
            return EXIT_FAILURE;

        }
    }
    std::cout<<"Training and test dataset are loaded successfuly "<<std::endl;
    if(alg == "MLP")
    {
        std::cout<<"Training data stats: "<<std::endl;
        this->_regressionTrainingData.printStats();
        std::cout<<"Test data stats: "<<std::endl;
        this->_regressionTestData.printStats();

        this->_numInputNeurons = this->_regressionTrainingData.getNumInputDimensions();
        this->_numHidenNeurons = 5;
        this->_numOutputNeurons = this->_regressionTrainingData.getNumTargetDimensions();
    }
    return true;
}

/*initializing algorithms
*/
bool GRGrt::init(std::string alg)
{
    if(alg == "MLP")
    {
        this->_mlp.init(_numInputNeurons, _numHiddenNeurons, _numOutputNeurons);

        //Set the training settings yes it's important 
        this->_mlp.setMaxNumEpochs(500); //This sets the maximum number of epochs (1 epoch is 1 complete iteration of the training data) that are allowed
        this->_mlp.setMinChange(1.0e-5);//This sets the minimum change allowed in training error between any two epochs
        this->_mlp.setNumRandomTrainingIterations(20);//This sets the number of times the MLP will be trained, each training iteration starts with new random values 
        this->_mlp.setUseValidationSet(true); ////This sets aside a small portiion of the training data to be used as a validation set to mitigate overfitting
        this->_mlp.setValidationSetSize(20); ////Use 20% of the training data for validation during the training phase
        this->_mlp.setRandomizeTrainingOrder(true); ////Randomize the order of the training data so that the training algorithm does not bias the training

        this->_mlp.enableScaling(true);

        //aading the MLP to pipeline
        this->_pipeline.setRegressifier(this->_mlp);     
    }
}
/* setting of test data with providing of size of dataset 
*/
bool GRGrt::setTestDataFromTraining(int size)
{
    if(this->_trainingData.getNumSamples() == 0)
    {
        std::cout << "No trainig data to use!"<<std::endl;
        return false;
    }

    if(size < 0 || size > 100)
    {
        std::cout << "Incorrect size!"<<std::endl;
        return false;
    }

    this->_testData = this->_trainingData.split(100 - size);

    return true;
}

/* training 
*/
bool GRGrt::train(std::string alg)
{
    if(alg == "DTW")
    {
        if(!this->_dtw.train(this->_trainingData))
        {
            std::cout << "Failed to train classifier!"<<std::endl;
            return EXIT_FAILURE;
        }
    }
    else if(alg == "MLP")
    {
        if(!this->_pipeline.train(_regressionTrainingData))
        {
            std::cout<<"ERROR: Failed to train MLP model "<<std::endl;
            return EXIT_FAILURE;
        }
    }
    else
    {
        std::cout<<"No such algorithm with name: "<<alg<<std::endl;
        return EXIT_FAILURE;
    }
    std::cout<<"Model trainned!"<<std::endl;
    return true;
}

/*testing of trained data
*/
double GRGrt::test(std::string alg)
{
    if(alg == "DTW")
    {
        double accuracy = 0;
        for(GRT::UINT i=0; i<this->_testData.getNumSamples(); i++){
            //Get the i'th test sample - this is a timeseries
            GRT::UINT classLabel = this->_testData[i].getClassLabel();
            GRT::MatrixDouble timeseries = this->_testData[i].getData();

            //Perform a prediction using the classifier
            if( !this->_dtw.predict( timeseries ) ){
                std::cout << "Failed to perform prediction for test sampel: " << i <<"\n";
                return EXIT_FAILURE;
            }

            //Get the predicted class label
            GRT::UINT predictedClassLabel = this->_dtw.getPredictedClassLabel();
            double maximumLikelihood = this->_dtw.getMaximumLikelihood();
            GRT::VectorDouble classLikelihoods = this->_dtw.getClassLikelihoods();
            GRT::VectorDouble classDistances = this->_dtw.getClassDistances();

            //Update the accuracy
            if( classLabel == predictedClassLabel ) accuracy++;
            std::cout << "TestSample: " << i <<  "\tClassLabel: " << classLabel << "\tPredictedClassLabel: " << predictedClassLabel << "\tMaximumLikelihood: " << maximumLikelihood << std::endl;
        }

        this->_testAccuracy = (double(accuracy) / double(_testData.getNumSamples())) * 100.0;

        std::cout << "Test Accuracy: " << this->_testAccuracy << "%" << std::endl;

        return this->_testAccuracy;
    }
    else if(alg == "MLP")
    {
        if(!this->pipeline.test(this->_regressionTestData))
        {
            std::cout<<"ERROR: Failed to test MLP model"<<std::endl;
            return EXIT_FAILURE;
        }
        std::cout<<"MLP test is complete starting test RMS error..."<<this->_pipeline.getTestRMSError()<<std::endl;
    }
}

/*get test acuracy
*/
double GRGrt::getTestAccuracy()
{
    return this->_testAccuracy;
}

/*save trained model in filepath 
*/
bool GRGrt::saveModel(std::string filepath)
{
    if(!this->_dtw.save(filepath))
    {
        std::cout << "Failed to save the classifier model!\n";
        return false;
    }

    return true;
}

/*load trained model from filepath
*/
bool GRGrt::loadModel(std::string filepath)
{
    if(!this->_dtw.load(filepath))
    {
        std::cout << "Failed to load the classifier model!\n";
        return false;
    }

    return true;
}

/*prediction
*/
bool GRGrt::predict(GRT::MatrixDouble timeseries)
{
    if(!_dtw.predict(timeseries))
    {
        std::cout << "Failed to perform prediction!\n";
        return false;
    }

    return true;
}

/*getting of label for prediction data
*/
GRT::UINT GRGrt::getPredictedClassLabel()
{
    return this->_dtw.getPredictedClassLabel();
}

/*needet for GRT
*/
double GRGrt::getMaximumLikelihood()
{
    return this->_dtw.getMaximumLikelihood();
}

/*setting dataset sensors datastructures
*/

/*properties for dataset
*/
void GRGrt::setDatasetProperties(std::string dataSetName, std::string infoText, std::string fProp, int dimIn )
{
    _dimensions = dimIn; 
    _trainingData.setNumDimensions(_dimensions);
    _trainingData.setDatasetName(dataSetName);
    _trainingData.setInfoText(infoText);
    _fileProp = fProp + ".grt";

}

/*set nex label in training data
*/
void GRGrt::setNextLabel()
{
    _gestureLabel ++;
}

/*clearing
*/
void GRGrt::clearTrainingData()
{
    _trainingData.clear();

}

bool GRGrt::addSample(std::vector<double>* trajIn, std::vector<double>* quantIn)
{
    GRT::VectorDouble sample;

    sample.resize(this->_dimensions);
    for(int i=0; i < (trajIn->size() + quantIn->size()); i++)
    {
        if(i<trajIn->size())
        {
            sample[i] = trajIn->at(i);
        }
        else
        {
            sample[i] = quantIn->at(i-3);
        }    
    } 
    _gestureSample.push_back(sample);
    return true;
}
/*pushing dataset in DTW algorithm
*/
bool GRGrt::pushGesture()
{
    _trainingData.addSample(_gestureLabel, _gestureSample);
    _gestureSample.clear();
}

/*saving dataset
*/
bool GRGrt::saveDataset()
{
    if(!_trainingData.save(("../trainingData/grTrainingDTW"+_fileProp)))
    {
        std::cout<<"ERROR: Failed to save dataset to file \n";
        return EXIT_FAILURE;
    }
    //datasetDTW.trainingData.clear();TODO
}

bool GRGrt::tranQantizationModel(std::string quantizationSettingsFile)
{
    this->_quantizer = GRT::KMeansQuantizer(_trainingData.getNumDimensions(), 10);
    
    if(!this->_quantizer.train(this->_trainingData))
    {
        std::cout<<"ERROR: Failed to train quantizer"<<std::endl;
        return EXIT_FAILURE;
    }

    if(!this->_quantizer.saveSettingsToFile(quantizationSettingsFile))
    {
        std::cout<<"ERROR: Failed to safe quantizer settings to file: "<<quantizationSettingsFile<<std::endl;
        return EXIT_FAILURE;
    }
    return true;
}

bool GRGrt::quantizeData(std::string quantizationSettingsFile)
{
    if(!this->_quantizer.loadSettingsFromFile(quantizationSettingsFile))
    {
        std::cout<<"ERROR: filed to load quantization settings from file: "<<quantizationSettingsFile<<std::endl;
        return EXIT_FAILURE;
    }
    for(int i=0; i<_trainingData.getNumSamples(); i++)
    {
        _quantizer.quantize(_trainingData[i].getSample());
        std::cout<<"quantization index: "<<i<<std::endl;
        std::cout<<"Sample: ";

        for(int j=0; j<_trainingData[i].getNumDimensions(); j++ )
        {
            std::cout<<_trainingData[i][j] <<"\t";
        }
        cout<<"Quantized value: "<<_quantizer.getQuantizedValue()<<std::endl;
    }
    return EXIT_SUCCESS;
}
