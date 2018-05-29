#include <grGrt.h>
/*constructor
*/
GRGrt::GRGrt()
{

    //utils
    this->_gestureLabel = 1;
    //datasetDTW.trainingData.setNumDimensions(datasetDTW.dimensions);
    //datasetDTW.trainingData.setDatasetName("grDTWTrainingData"); //TODO later add relative name 
    //datasetDTW.trainingData.setInfoText("Thisdataset contains some GR data"); //later add relative info text
    this->_dtw.enableNullRejection(true);

    this->_numInputNeurons = 0;
    this->_numHidenNeurons = 0;
    this->_numOutputNeurons = 0;

    this->_mlpInputVectorDimensions = 36;
    this->_mlpTargetVectorDimensions = 36;
    this->_mlpTrainingExamplesNumber = 30;

    this->_mlpInputVector.resize(_mlpInputVectorDimensions);
    this->_mlpTargetVector.resize(_mlpTargetVectorDimensions);


    GRT::TrainingLog::setLoggingEnabled( true ); 

}

GRGrt::GRGrt(std::string alg)
{
    GRGrt();
    setRegressifier(alg);
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
    this->_dtwTrainingData = GRT::TimeSeriesClassificationData(t._dtwTrainingData);
    this->_dtwTestData = GRT::TimeSeriesClassificationData(t._dtwTestData);

    this->_testAccuracy = t._testAccuracy;
}

/*operator =
*/
GRGrt& GRGrt::operator=(const GRGrt& t)
{
    this->_dtw = GRT::DTW(t._dtw);
    this->_dtwTrainingData = GRT::TimeSeriesClassificationData(t._dtwTrainingData);
    this->_dtwTestData = GRT::TimeSeriesClassificationData(t._dtwTestData);

    this->_testAccuracy = t._testAccuracy;

}

bool GRGrt::setRecognitionAlgorithm(std::string alg)
{
    if(alg == "DTW")
    {
        _algType = alg;
    }
    else if(alg == "MLP_R")
    {
        _algType = alg;
    }
    else if(alg == "MLP_C")
    {
        _algType = alg;
    }
    else
    {
        std::cout<<"ERROR: No such recognition algorithm with name: "<<alg<<std::endl;
    }
    return EXIT_SUCCESS;
}
/*loading of training data from filepath
 * training data structor is used from GRT standarts //TODO later need to be from database
 */
bool GRGrt::loadTrainingData(std::string filepath, std::string alg)
{
    if(alg == "DTW")
    {
        if(!this->_dtwTrainingData.load(filepath))
        {
            std::cout << "Failed to load training data!\n";
            return false;
        }

    }
    else if(alg == "MLP_R")
    {
        if(!this->_mlpRegressionTrainingData.load(filepath))
        {
            std::cout<<"ERROR: failed to load training data for: "<<alg
                <<" from path"<< filepath<<std::endl;
            return EXIT_FAILURE;
        }

    }
    else if(alg == "MLP_C")
    {
        if(!this->_mlpClassificationTrainingData.load(filepath))
        {
            std::cout<<"ERROR: failed to load training data for "<< alg
                <<" from path "<<filepath<<std::endl;
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
        if(!this->_dtwTestData.load(filepath))
        {
            std::cout << "Failed to load test data!\n";
            return false;
        }
    }
    else if(alg == "MLP_R")
    {
        if(!this->_mlpRegressionTrainingData.load(filepath))
        {
            std::cout<<"ERROR: Failed to load test data for "<<alg
                <<"from filepath"<<filepath<<std::endl;
            return EXIT_FAILURE;
        }
        if(this->_mlpRegressionTrainingData.getNumInputDimensions() !=this-> _mlpRegressionTestData.getNumInputDimensions())
        {
            std::cout<<"ERROR: The number of input dimensions in the training data ( "
                <<this->_mlpRegressionTrainingData.getNumInputDimensions()<<" )"
                <<"does not match the number of input dimensions of test data ( "
                <<this->_mlpRegressionTestData.getNumInputDimensions()<<" )"<<std::endl;
            return EXIT_FAILURE;

        }
        if(this->_mlpRegressionTrainingData.getNumTargetDimensions() != this->_mlpRegressionTestData.getNumTargetDimensions())
        {
            std::cout<<"ERROR: The number of target dimensions of tranining data ("
                <<this->_mlpRegressionTrainingData.getNumTargetDimensions()<<" )"
                <<"does not match with target dimensions of test data ( "
                <<this->_mlpRegressionTestData.getNumTargetDimensions()<<" )"<<std::endl;
            return EXIT_FAILURE;

        }
    }
    else if(alg == "MLP_C")
    {
        if(!this->_mlpClassificationTestData.load(filepath))
        {
            std::cout<<"ERROR: Failed to load test data for "<< alg
                <<" from path "<< filepath<<std::endl;
        }
    }
    std::cout<<"Training and test dataset are loaded successfuly "<<std::endl;
    if(alg == "MLP_R")
    {
        std::cout<<"Training data stats: "<<std::endl;
        this->_mlpRegressionTrainingData.printStats();
        std::cout<<"Test data stats: "<<std::endl;
        this->_mlpRegressionTestData.printStats();

        this->_numInputNeurons = this->_mlpRegressionTrainingData.getNumInputDimensions();
        this->_numHidenNeurons = 5;
        this->_numOutputNeurons = this->_mlpRegressionTrainingData.getNumTargetDimensions();
    }
    return true;
}

/*initializing algorithms
*/
bool GRGrt::setParams(std::string alg)
{
    if(alg == "DTW")
    {

    }
    else if(alg == "MLP_R")
    {
        this->_mlp.init(_numInputNeurons, _numHidenNeurons, _numOutputNeurons);

        //TODO need to be set from config !!!
        //Set the training settings yes it's important 
        this->_mlp.setMaxNumEpochs(500); //This sets the maximum number of epochs (1 epoch is 1 complete iteration of the training data) that are allowed
        this->_mlp.setMinChange(1.0e-5);//This sets the minimum change allowed in training error between any two epochs
        this->_mlp.setLearningRate(0.1); //This sets the rate at which the learning algorithm updates the weights of the neural network
        this->_mlp.setMomentum(0.5); //TODO need to be verified
        this->_mlp.setNumRestarts(20);//This sets the number of times the MLP will be trained, each training iteration starts with new random values 
        this->_mlp.setUseValidationSet(true); ////This sets aside a small portiion of the training data to be used as a validation set to mitigate overfitting
        this->_mlp.setValidationSetSize(20); ////Use 20% of the training data for validation during the training phase
        this->_mlp.setRandomiseTrainingOrder(true); ////Randomize the order of the training data so that the training algorithm does not bias the training

        this->_mlp.enableScaling(true);

        //aading the MLP to pipeline
        this->_pipeline.setRegressifier(this->_mlp);     
    }
    else if(alg == "MLP_C")
    {
        this->_mlp.init(_numInputNeurons, _numHidenNeurons, _numOutputNeurons);

        //TODO need to be set from config !!!
        //Set the training settings yes it's important 
        this->_mlp.setMaxNumEpochs(500); //This sets the maximum number of epochs (1 epoch is 1 complete iteration of the training data) that are allowed
        this->_mlp.setMinChange(1.0e-5);//This sets the minimum change allowed in training error between any two epochs
        this->_mlp.setLearningRate(0.1); //This sets the rate at which the learning algorithm updates the weights of the neural network
        this->_mlp.setMomentum(0.5); //TODO need to be verified
        this->_mlp.setNumRestarts(20);//This sets the number of times the MLP will be trained, each training iteration starts with new random values 
        this->_mlp.setUseValidationSet(true); ////This sets aside a small portiion of the training data to be used as a validation set to mitigate overfitting
        this->_mlp.setValidationSetSize(20); ////Use 20% of the training data for validation during the training phase
        this->_mlp.setRandomiseTrainingOrder(true); ////Randomize the order of the training data so that the training algorithm does not bias the training

        this->_mlp.enableScaling(true);

        //aading the MLP to pipeline
        this->_pipeline.setRegressifier(this->_mlp);     

    }

}
/* setting of test data with providing of size of dataset 
*/
bool GRGrt::setTestDataFromTraining(int size, std::string alg)
{
    if(size < 0 || size > 100)
    {
        std::cout << "Incorrect size!"<<std::endl;
        return EXIT_FAILURE;
    }

    //seze from examples is 20%
    if(alg == "DTW")
    {
        if(this->_dtwTrainingData.getNumSamples() == 0)
        {
            std::cout << "ERROR: There is no training data to use"<<std::endl;
            return EXIT_FAILURE;
        }

        this->_dtwTestData = this->_dtwTrainingData.split(100 - size);
    }
    else if(alg == "MLP_R")
    {
        if(this->_mlpRegressionTrainingData.getNumSamples == 0) 
        {
            std::cout<<"ERROR: There is no training data to use"<<std::endl;
            return EXIT_FAILURE;
        }

        this->_mlpRegressionTestData = this->_mlpRegressionTrainingData.split(100 - size)
    }
    else if(alg == "MLP_C")
    {
        if(this->_mlpClassificationTrainingData.getNumSamples == 0)
        {
            std::cout<<"ERROR: There is no trining data to use"<<std::endl;
            return EXIT_FAILURE;
        }
        this->_mlpClassificationTestData = this->_mlpClassificationTrainingData.split(100 - size);
    }
    else
    {
        std::cout<<"ERROR: there is no algorithm with name: "<<alg<<std::endl;
        return EXIT_FAILURE;

    }
    return EXIT_SUCCESS;
}

/* training 
*/
bool GRGrt::train(std::string alg)
{
    if(alg == "DTW")
    {
        if(!this->_dtw.train(this->_dtwTrainingData))
        {
            std::cout << "Failed to train classifier!"<<std::endl;
            return EXIT_FAILURE;
        }
    }
    else if(alg == "MLP_R")
    {
        if(!this->_pipeline.train(_mlpRegressionTrainingData))
        {
            std::cout<<"ERROR: Failed to train MLP model "<<std::endl;
            return EXIT_FAILURE;
        }
    }
    else if(alg == "MLP_C")
    {
        if(!this->_pipeline.train(this->_mlpClassificationTrainingData))
        {
            std::cout<<"ERROR: Failed to train Clasification MLP"<<std::endl;
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
double GRGrt::test(std::string alg) //HERE
{
    if(alg == "DTW")
    {
        double accuracy = 0;
        for(GRT::UINT i=0; i<this->_dtwTestData.getNumSamples(); i++){
            //Get the i'th test sample - this is a timeseries
            GRT::UINT classLabel = this->_dtwTestData[i].getClassLabel();
            GRT::MatrixDouble timeseries = this->_dtwTestData[i].getData();

            //Perform a prediction using the classifier
            if( !this->_dtw.predict( timeseries ) )
            {
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

        this->_testAccuracy = (double(accuracy) / double(_dtwTestData.getNumSamples())) * 100.0;

        std::cout << "Test Accuracy: " << this->_testAccuracy << "%" << std::endl;

        return this->_testAccuracy;
    }
    else if(alg == "MLP_R")
    {
        if(!this->_pipeline.test(this->_mlpRegressionTestData))
        {
            std::cout<<"ERROR: Failed to test MLP model"<<std::endl;
            return EXIT_FAILURE;
        }
        std::cout<<"MLP test is complete starting test RMS error..."<<this->_pipeline.getTestRMSError()<<std::endl;

        //TODO make output file as parameter
        std::fstream file;
        file.open("mlp_regression_results.csv", std::fstream::out);

        for(GRT::UINT i=0; i<_mlpRegressionTestData.getNumSamples(); i++)
        {
            GRT::VectorDouble inputVector = _mlpRegressionTestData[i].getInputVector();
            GRT::VectorDouble targetVector = _mlpRegressionTestData[i].getTargetVector();

            //Map the input vector using the trained regression model
            if(!_pipeline.predict(inputVector))
            {
                std::cout<<"ERROR: Failed to map test sample"<< i <<std::endl;
                return EXIT_FAILURE;
            }

            GRT::VectorDouble outputVector = _pipeline.getRegressionData();

            //Write the mapped value and also the target value to the file
            for(GRT::UINT j=0; j<outputVector.size(); j++)
            {
                file<<outputVector[j]<<"\t";
            }
            for(GRT::UINT j=0;j<targetVector.size(); j++)
            {
                file<<targetVector[j]<<"\t";
            }
            file<<std::endl; 
        }

        file.close();
        _testAccuracy = double(_pipeline.getTestAccuracy());
        std::cout<<"Test Accuracy of regresional MLP: "<< _testAccuracy <<std::endl;
        return _testAccuracy;



    }
    else if(alg == "MLP_C")
    {

        GRT::RegressionData regressionFromClassificationTestData = _mlpClassificationTestData.reformatAsRegression();
        if(!this->_pipeline.test(regressionFromClassificationTestData))
        {
            std::cout<<"ERROR: Failed to test Classification MLP model"<<std::endl;
            return EXIT_FAILURE;
        }

        std::fstream file;
        file.open("mlp_classification.csv", std::fstream::out);
        for(GRT::UINT i=0; i<regressionFromClassificationTestData.getNumSamples; i++)
        {
            GRT::VectorDouble inputVector = regressionFromClassificationTestData[i].getInputVector();
            GRT::VectorDouble targetVector = regressionFromClassificationTestData[i].getTargetVector();

            if(!_pipeline.predict(inputVector))
            {
                std::cout<<"ERROR: failed to map test sample "<<i<<std::endl;
                return EXIT_FAILURE;
            }
            GRT::VectorDouble _pipeline.getRegressionData();
            
            for(GRT::UINT j=0; j<outputVector.size(); j++)
            {
                file<<outputVector[j]<<'\t';

            }
            for(GRT::UINT j=0; J<targetVector.size(); j++)
            {
                file<<targetVector[j]<<'\t';
            }
            file<<std::endl;
        }
        file.close();
        _testAccuracy = double(_pipeline.getTestAccuracy());
        std::cout<<"Test accuracy of classification MLP: "<<_testAccuracy<<std::endl;

        return _testAccuracy;
    }
    else
    {
        std::cout<<"No such algorithm wit name: "<<alg<<std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
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
bool GRGrt::setDatasetProperties(std::string alg, std::string datasetName, std::string infoText, std::string fProp, int dimIn)
{
    if(alg == "DTW")
    {
        this->_dimensions = dimIn; 
        this->_dtwTrainingData.setNumDimensions(_dimensions);
        this->_dtwTrainingData.setDatasetName(datasetName);
        this->_dtwTrainingData.setInfoText(infoText);
        this->_fileProp = fProp + ".grt";
        return EXIT_SUCCESS;
    }
    else if(alg == "MLP_R")
    {
        this->_mlpRegressionTrainingData.setInputAndTargetDimensions(this->_mlpInputVectorDimensions, this->_mlpTargetVectorDimensions);
        this->_mlpRegressionTrainingData.setDatasetName(datasetName);
        this->_mlpRegressionTrainingData.setInfoText(infoText);
        this->_fileProp = fProp + ".grt";
        return EXIT_SUCCESS;
    }
    else if(alg == "MLP_C")
    {
        this->_dimensions = dimIn;
        this->_mlpClassificationTrainingData.setNumDimensions(_dimensions);
        this->_mlpClassificationTrainingData.setDatasetName(datasetName);
        this->_mlpClassificationTrainingData.setInfoText(infotext);
        this->_fileprop = fProp + ".grt";
        return EXIT_SUCCESS;
    }
    else
    {
        std::cout<<"ERROR No such algorithm with name: "<<alg<<std::endl;
        return EXIT_FAILURE;
    }

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
    _dtwTrainingData.clear();

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
    _dtwTrainingData.addSample(_gestureLabel, _gestureSample);
    _gestureSample.clear();
}

/*saving dataset
*/
bool GRGrt::saveDataset()
{
    if(!_dtwTrainingData.save(("../trainingData/grTrainingDTW"+_fileProp)))
    {
        std::cout<<"ERROR: Failed to save dataset to file \n";
        return EXIT_FAILURE;
    }
    //datasetDTW.trainingData.clear();TODO
}

bool GRGrt::tranQantizationModel(std::string quantizationSettingsFile)
{
    this->_quantizer = GRT::KMeansQuantizer(_dtwTrainingData.getNumDimensions());

    if(!this->_quantizer.train(this->_dtwTrainingData))
    {
        std::cout<<"ERROR: Failed to train quantizer"<<std::endl;
        return EXIT_FAILURE;
    }

    if(!this->_quantizer.save(quantizationSettingsFile))
    {
        std::cout<<"ERROR: Failed to safe quantizer settings to file: "<<quantizationSettingsFile<<std::endl;
        return EXIT_FAILURE;
    }
    return true;
}

bool GRGrt::quantizeData(std::string quantizationSettingsFile)
{
    //TODO need to be fixed 
    /* 
       if(!this->_quantizer.load(quantizationSettingsFile))
       {
       std::cout<<"ERROR: filed to load quantization settings from file: "<<quantizationSettingsFile<<std::endl;
       return EXIT_FAILURE;
       }
       for(int i=0; i<_dtwTrainingData.getNumSamples(); i++)
       {
       _quantizer.quantize(_dtwTrainingData[i].getSample());
       std::cout<<"quantization index: "<<i<<std::endl;
       std::cout<<"Sample: ";

       for(int j=0; j<_dtwTrainingData[i].getNumDimensions(); j++ )
       {
       std::cout<<_dtwTrainingData[i][j] <<"\t";
       }
       cout<<"Quantized value: "<<_quantizer.getQuantizedValue()<<std::endl;
       }
       return EXIT_SUCCESS;
       */
}
