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

    this->_mlpInputVectorDimensions = 26;
    this->_mlpTargetVectorDimensions = 1;
    this->_mlpTrainingExamplesNumber = 30;

    this->_mlpInputVector.resize(_mlpInputVectorDimensions);
    this->_mlpTargetVector.resize(_mlpTargetVectorDimensions);


    GRT::TrainingLog::setLoggingEnabled( true ); 

    this->_enableFeatureExtraction = false;
    this->_algType = "NO ALGORITHM";

    // this->_quantizer = GRT::KMeansQuantizer( 5, 10); is not needed
}
/*
   GRGrt::GRGrt(std::string alg)
   {
   GRGrt();
   setRegressifier(alg);//TODO WHAAAT?!!??!?!?
   }
   */
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

bool GRGrt::setAlgorithms(std::string alg, bool enableFutureExtraction)
{
    if(enableFutureExtraction)
    {
        _enableFeatureExtraction == enableFutureExtraction;
    }
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
        return false;
    }
    return true;
}
/*loading of training data from filepath
 * training data structor is used from GRT standarts //TODO later need to be from database
 */
bool GRGrt::loadTrainingData(std::string filepath)
{
    if(_algType == "DTW")
    {
        if(!this->_dtwTrainingData.load(filepath))
        {
            std::cout << "Failed to load training data!\n";
            return false;
        }

    }
    else if(_algType == "MLP_R")
    {
        if(!this->_mlpRegressionTrainingData.load(filepath))
        {
            std::cout<<"ERROR: failed to load training data for: "<<_algType
                <<" from path"<< filepath<<std::endl;
            return false;
        }
        if(!_setNumNeurons())
        {
            std::cout<<"ERROR: cannot set neurons from dataset"<<std::endl;
            return false;
        }

    }
    if(_algType == "MLP_C")
    {
        if(!this->_mlpClassificationTrainingData.load(filepath))
        {

            std::cout<<"ERROR: failed to load training data for "<< _algType
                <<" from path "<<filepath<<std::endl;
            return false;
        }
        if(!_setNumNeurons())
        {
            std::cout<<"ERROR: cannot set Neurons from dataset"<<std::endl;
            return false;
        }
    }



    else
    {
        std::cout<<"ERROR: You forgot to specify recognition algorithm "<<std::endl;
        return false;
    }
    std::cout<<"Training dataset is loaded successfuly"<<std::endl;
    std::cout<<"Training data stats: "<<std::endl;
    //this->_mlpClassificationTrainingData.printStats();
    return true;
}

bool GRGrt::_setNumNeurons()
{
    if(_algType == "MLP_R")
    {
        this->_numInputNeurons = this->_mlpRegressionTrainingData.getNumInputDimensions();
        this->_numHidenNeurons = 5;
        this->_numOutputNeurons =this->_mlpRegressionTrainingData.getNumTargetDimensions();
    }
    else if(_algType == "MLP_C")
    {
        this->_numInputNeurons = this->_mlpClassificationTrainingData.getNumDimensions();
        this->_numHidenNeurons = 5;
        this->_numOutputNeurons = this->_mlpClassificationTrainingData.getNumClasses();
        std::cout<<"------------------------------------------------------------------------neurons are seted"<<std::endl;

        return true;
    }
    else
    {
        std::cout<<"ERROR: You forgot to specify recognition algorithm "<<std::endl;
        return false;
    }

    return true;
}
/*loading test data from filepath
*/
bool GRGrt::loadTestData(std::string filepath)
{
    if(_algType == "DTW")
    {
        if(!this->_dtwTestData.load(filepath))
        {
            std::cout << "Failed to load test data!\n";
            return false;
        }
    }
    else if(_algType == "MLP_R")
    {
        if(!this->_mlpRegressionTestData.load(filepath))
        {
            std::cout<<"ERROR: Failed to load test data for "<<_algType
                <<"from filepath"<<filepath<<std::endl;
            return false;
        }
        if(!_matchDimensionsOfTrainingAndTestData(this->_mlpRegressionTrainingData, this->_mlpRegressionTestData))
        {
            return false;
        }
    }
    else if(_algType == "MLP_C")
    {
        if(!this->_mlpClassificationTestData.load(filepath))
        {
            std::cout<<"ERROR: Failed to load test data for "<< _algType
                <<" from path "<< filepath<<std::endl;
            return false;
        }

        if(!_matchDimensionsOfTrainingAndTestData(this->_mlpClassificationTrainingData.reformatAsRegressionData(),
                    this->_mlpClassificationTestData.reformatAsRegressionData()))
        {
            return false;
        }
    }
    else 
    {
        std::cout<<"ERROR: You forgot to specify recognition algorithm "<< _algType<<std::endl;
        return false;
    }
    return true;
}

bool GRGrt::_matchDimensionsOfTrainingAndTestData(GRT::RegressionData trainingData, GRT::RegressionData testData)
{
    if(trainingData.getNumInputDimensions() !=testData.getNumInputDimensions())
    {
        std::cout<<"ERROR: The number of input dimensions in the training data ( "
            <<trainingData.getNumInputDimensions()<<" )"
            <<"does not match the number of input dimensions of test data ( "
            <<testData.getNumInputDimensions()<<" )"<<std::endl;
        return false;

    }

    if(trainingData.getNumTargetDimensions() != testData.getNumTargetDimensions())
    {
        std::cout<<"ERROR: The number of target dimensions of tranining data ("
            <<trainingData.getNumTargetDimensions()<<" )"
            <<"does not match with target dimensions of test data ( "
            <<testData.getNumTargetDimensions()<<" )"<<std::endl;
        return false;

    }
    std::cout<<"Test dataset is loaded successfuly "<<std::endl;

    std::cout<<"Test data stats: "<<std::endl;
    this->_mlpRegressionTestData.printStats();

    return true;
}


/*initializing algorithms
*/
bool GRGrt::prepare()
{
    if(_enableFeatureExtraction)
    {
        this->_pipeline << this->_quantizer;
    }

    if(_algType == "DTW")
    {
        this->_pipeline << this->_dtw;
    }
    else if(_algType == "MLP_R" || _algType == "MLP_C")
    {
        this->_mlp.init(_numInputNeurons, _numHidenNeurons, _numOutputNeurons, inputActivationFunction, hiddenActivationFunction, outputActivationFunction );

        //TODO need to be set from config !!!
        //Set the training settings yes it's important 
        this->_mlp.setMaxNumEpochs(500); //This sets the maximum number of epochs (1 epoch is 1 complete iteration of the training data) that are allowed
        this->_mlp.setMinChange(1.0e-10);//This sets the minimum change allowed in training error between any two epochs
        this->_mlp.setLearningRate(0.1); //This sets the rate at which the learning algorithm updates the weights of the neural network
        this->_mlp.setMomentum(0.1); //TODO need to be verified
        this->_mlp.setNumRestarts(10000);//This sets the number of times the MLP will be trained, each training iteration starts with new random values 
        this->_mlp.setUseValidationSet(false); ////This sets aside a small portiion of the training data to be used as a validation set to mitigate overfitting
        this->_mlp.setValidationSetSize(20); ////Use 20% of the training data for validation during the training phase
        this->_mlp.setRandomiseTrainingOrder(true); ////Randomize the order of the training data so that the training algorithm does not bias the training

        this->_mlp.enableScaling(true);

        //aading the MLP to pipeline
        //   _pipeline << _mlp;
        //   _pipeline.setClassifier(_mlp);
        //        this->_pipeline.setClassifier(_mlp);
        std::cout<<"---------------------------------------------praparing successfuly"<<std::endl;     
    }
    else
    {
        std::cout<<"ERROR: You forgot to specify recognition algorithm "<<std::endl;
        return false;
    }
    return true;

}
/* setting of test data with providing of size of dataset 
*/
bool GRGrt::setTestDataFromTraining(int size)
{
    if(size < 0 || size > 100)
    {
        std::cout << "Incorrect size!"<<std::endl;
        return false;
    }

    //seze from examples is 20%
    if(_algType == "DTW")
    {
        if(this->_dtwTrainingData.getNumSamples() == 0)
        {
            std::cout << "ERROR: There is no training data to use"<<std::endl;
            return false;
        }

        this->_dtwTestData = this->_dtwTrainingData.split(100 - size);
    }
    else if(_algType == "MLP_R")
    {
        if(this->_mlpRegressionTrainingData.getNumSamples() == 0) 
        {
            std::cout<<"ERROR: There is no training data to use"<<std::endl;
            return false;
        }

        this->_mlpRegressionTestData = this->_mlpRegressionTrainingData.split(100 - size);
    }
    else if(_algType == "MLP_C")
    {
        if(this->_mlpClassificationTrainingData.getNumSamples() == 0)
        {
            std::cout<<"ERROR: There is no trining data to use"<<std::endl;
            return false;
        }
        this->_mlpClassificationTestData = this->_mlpClassificationTrainingData.split(100 - size);
    }
    else
    {
        std::cout<<"ERROR: You forgot to specify recognition algorithm "<<std::endl;
        return false;
    }
    return true;
}

/* training 
*/
bool GRGrt::train()
{
    if(_algType == "DTW")
    {
        if(!this->_dtw.train(this->_dtwTrainingData))
        {
            std::cout << "Failed to train classifier!"<<std::endl;
            return false;
        }
    }
    else if(_algType == "MLP_R")
    {
        if(!this->_pipeline.train(this->_mlpRegressionTrainingData))
        {
            std::cout<<"ERROR: Failed to train Regression  MLP model "<<std::endl;
            return false;
        }
    }
    else if(_algType == "MLP_C")
    {
        //  if(!this->_pipeline.train(this->_mlpClassificationTrainingData))
        if(!this->_mlp.train(this->_mlpClassificationTrainingData))
        {
            std::cout<<"ERROR: Failed to train Clasification MLP"<<std::endl;
            return false;

        }
        //if()
    }

    else
    {
        std::cout<<"ERROR: You forgot to specify recognition algorithm "<<std::endl;
        return false;
    }
    std::cout<<"Model trainned!"<<std::endl;
    return true;
}

/*testing of trained data
*/
double GRGrt::test() //HERE
{
    if(_algType == "DTW")
    {
        double accuracy = 0;
        /*if(!_pipeline.test(this->_dtwTestData))
          {
          std::cout<<"ERROR: Failed to test DTW model";
          return false;
          }*/
        if(!_dtw.load("../trainingData/DTW_model.grt"))
        {
            std::cout<<"ERROR CANNOT LOAD DTW"<<std::endl;
        } 
        std::cout<<"Performing test with data output"<<std::endl;
        for(GRT::UINT i=0; i<this->_dtwTestData.getNumSamples(); i++){
            //Get the i'th test sample - this is a timeseries
            GRT::UINT classLabel = this->_dtwTestData[i].getClassLabel();
            GRT::MatrixDouble timeseries = this->_dtwTestData[i].getData();

            //Perform a prediction using the classifier
            if( !this->_dtw.predict( timeseries ) )
            {
                std::cout << "Failed to perform prediction for test sampel: " << i <<"\n";
                return false;
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

    else if(_algType == "MLP_R")
    {
        /*  if(!this->_pipeline.test(this->_mlpRegressionTestData))
            {
            std::cout<<"ERROR: Failed to test MLP model"<<std::endl;
            return false;
            }
            */std::cout<<"MLP test is complete starting test RMS error..."<<this->_pipeline.getTestRMSError()<<std::endl;

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
                return false;
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
    else if(_algType == "MLP_C")
    {

        GRT::RegressionData regressionFromClassificationTestData = _mlpClassificationTestData.reformatAsRegressionData();
        /*        if(!this->_pipeline.test(regressionFromClassificationTestData))
                  {
                  std::cout<<"ERROR: Failed to test Classification MLP model"<<std::endl;
                  return false;
                  }
                  */
        std::fstream file;
        file.open("mlp_classification.csv", std::fstream::out);
        for(GRT::UINT i=0; i<regressionFromClassificationTestData.getNumSamples(); i++)
        {
            GRT::VectorDouble inputVector = regressionFromClassificationTestData[i].getInputVector();
            GRT::VectorDouble targetVector = regressionFromClassificationTestData[i].getTargetVector();

            if(!_mlp.predict(inputVector))
            {
                std::cout<<"ERROR: failed to map test sample "<<i<<std::endl;
                return false;
            }
            GRT::VectorDouble outputVector = _pipeline.getRegressionData();

            for(GRT::UINT j=0; j<outputVector.size(); j++)
            {
                file<<outputVector[j]<<'\t';

            }
            for(GRT::UINT j=0; j<targetVector.size(); j++)
            {
                file<<targetVector[j]<<'\t';
            }

            file<<std::endl;
            GRT::UINT predictedClassLabel = _mlp.getPredictedClassLabel();
            double maximumLikelihood = _mlp.getMaximumLikelihood();
            GRT::VectorDouble classLikelihoods = _mlp.getClassLikelihoods();
            GRT::VectorDouble classDistances = _mlp.getClassDistances();

        }
        file.close();

        // _testAccuracy = double(_mlp.getTestAccuracy());
        std::cout<<"Test accuracy of classification MLP: "<<_testAccuracy<<std::endl;

        return _testAccuracy;
    }
    else
    {
        std::cout<<"No such algorithm wit name: "<<_algType<<std::endl;
        return false;
    }
    return true;
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
    if(!this->_mlp.save(filepath))
    {
        std::cout<<"ERROR: Cannot Save the pipeline"<<std::endl;;
        return false;

    }
    /*
       if(_algType == "DTW" && !this->_dtw.save(filepath))
       {
       std::cout << "Failed to save the classifier model!\n";
       return false;
       }
       else if((_algType == "MLP_R" || _algType == "MLP_C") && !this->_mlp.save(filepath))
       {
       std::cout<<"ERROR: Failed to save the "<<_algType<<"model"<<std::endl;
       return false;
       }
       else
       {
       std::cout<<"ERROR: You forgot to cpecify algorithm"<<std::endl;
       return false;
       }
       */
    return true;
}

/*load trained model from filepath
*/
bool GRGrt::loadModel(std::string filepath) //TODO HERE TO CONTINUE
{
    if(!this->_pipeline.load(filepath))
    {
        std::cout<<"ERROR: cannot load pipeline"<<std::endl;
        return false;
    }

    /*
       if(_algType == "DTW" && !this->_dtw.load(filepath))
       {
       std::cout << "Failed to load the classifier model!\n";
       return false;
       }
       else if((_algType == "MLP_R"|| _algType == "MLP_C") && !this->_mlp.load(filepath) )
       {
       std::cout<<"ERROR: Failed to load model to "<<_algType<<std::endl;
       return false;
       }
       else
       {
       std::cout<<"ERROR: You forgot to cpecify algorithm"<<std::endl;
       return false;

       }
       */
    return true;
}

/*prediction
*/
bool GRGrt::predict(GRT::MatrixDouble timeseries)
{
    if(!this->_pipeline.predict(timeseries))
    {
        std::cout<<"ERROR: cannot predict"<<std::endl;
        return false;
    }

    /*
       if(_algType == "DTW" && !_dtw.predict(timeseries))
       {
       std::cout << "Failed to perform prediction!\n";
       return false;
       }
       else if((_algType == "MLP_R" || _algType == "MLP_C" ) && !_mlp.predict(timeseries))
       {
       std::cout << "Failed to perform prediction!\n";
       return false;

       }
       else
       {
       std::cout<<"ERROR: You forgot to cpecify algorithm"<<std::endl;
       return false;

       }
       */
    return true;
}

bool GRGrt::predict(GRT::VectorDouble samples)
{
    if(!this->_pipeline.predict(samples))
    {
        std::cout<<"ERROR: cannot predsict"<<std::endl;
        return false;
    }
    return true;
}

/*getting of label for prediction data
*/
GRT::UINT GRGrt::getPredictedClassLabel()
{
    return this->_pipeline.getPredictedClassLabel();
    /*
       if(_algType == "DTW")
       {
       return this->_dtw.getPredictedClassLabel();
       }
       else if(_algType == "MLP_R" || _algType == "MLP_C")
       {

    //TODO specify prediction
    }
    */
}

/*needet for GRT
*/
double GRGrt::getMaximumLikelihood()
{
    if(_algType == "DTW")
    {
        return this->_dtw.getMaximumLikelihood();
    }
    else if(_algType == "MLP_R" || _algType == "MLP_C")
    {
        if(_algType == "MLP_R")
        {
            std::cout<<"ERROR: this option avalible only for classificatiohn algorithms"<<std::endl;
            return 0;
        }
        else
        {
            return this->_mlp.getMaximumLikelihood();
        }
    }
}

/*setting dataset sensors datastructures
*/

/*properties for dataset
*/
bool GRGrt::setDatasetProperties(std::string datasetName, std::string infoText, std::string fProp, int dimIn)
{
    if(_algType == "DTW")
    {
        this->_dimensions = dimIn; 
        this->_dtwTrainingData.setNumDimensions(_dimensions);
        this->_dtwTrainingData.setDatasetName(datasetName);
        this->_dtwTrainingData.setInfoText(infoText);
        this->_fileProp = fProp + ".grt";
        return true;
    }
    else if(_algType == "MLP_R" )
    {
        this->_dimensions = dimIn;
        this->_mlpRegressionTrainingData.setInputAndTargetDimensions(this->_mlpInputVectorDimensions, this->_mlpTargetVectorDimensions);
        this->_mlpRegressionTrainingData.setDatasetName(datasetName);
        this->_mlpRegressionTrainingData.setInfoText(infoText);
        this->_fileProp = fProp + ".grt";
        return true;
    }
    else if(_algType == "MLP_C")
    {
        this->_dimensions = dimIn;

        _mlpClassificationTrainingData.setNumDimensions(dimIn);
        this->_mlpClassificationTrainingData.setDatasetName(datasetName);
        this->_mlpClassificationTrainingData.setInfoText(infoText);
        this->_fileProp = fProp + ".grt";
        return true;

    }
    else
    {
        std::cout<<"ERROR No such algorithm with name: "<<_algType<<std::endl;
        return false;
    }

}

/*set nex label in training data
*/
void GRGrt::setNextLabel()
{
    this->_gestureLabel ++;
    if(_algType == "MLP_R" || _algType == "MLP_R")//TODO for now but must be changed
    {
        _mlpTargetVector[0] = _gestureLabel;
    }
}

/*clearing
*/
void GRGrt::clearTrainingData()
{
    if(this->_algType == "DTW")
    {
        this->_dtwTrainingData.clear();
    }
    else if(this->_algType == "MLP_R" )
    {
        this->_mlpRegressionTrainingData.clear();

    }
    else if(this->_algType == "MLP_C")
    {
        this->_mlpClassificationTrainingData.clear();
    }

}

bool GRGrt::addSample(std::vector<double>* accData, std::vector<double>* gyroData)
{
    GRT::VectorDouble sample;
    sample.resize((accData->size()+gyroData->size()));
    for(int i=0; i < (accData->size() + gyroData->size()); i++)
    {
        if(i<accData->size())
        {
            sample[i] = accData->at(i);
        }
        else
        {
            sample[i] = gyroData->at(i-(accData->size()));
        }    
    }


    if(this->_algType == "DTW" )
    {

        if(!this->_gestureSample.addSample(_gestureLabel, sample))
        {
            std::cout<<"ERROR: cannot set sample for: "<<_algType<<std::endl;
            return false;
        }

        //this->_gestureSample.push_back(sample);//TODO need to check 
    }
    else if(this->_algType == "MLP_R")
    {

        this->_mlpInputVector.clear();
        this->_mlpInputVector = sample;
        this->_mlpRegressionSample.set(_mlpInputVector, _mlpTargetVector);

    }
    else if(this->_algType == "MLP_C")
    {
        std::cout<<"---------------------------------------------------------------------------------------"<<std::endl;
        this->_mlpClassificationSample.set(_gestureLabel, sample);

        this->_mlpClassificationSamples.push_back(_mlpClassificationSample);
        this->_mlpClassificationSample.clear();

    }
    else
    {
        std::cout<<"ERROR: You forgot to cpecify algorithm"<<std::endl;
        return false;
    }
    sample.clear();
    return true;
}
/*pushing dataset in DTW algorithm
*/
bool GRGrt::pushGesture()
{
    if(this->_algType == "DTW")
    {
        if(!this->_dtwTrainingData.addSample(this->_gestureSample.getClassLabel(), 
                    this->_gestureSample.getData()))
        {
            std::cout<<"ERROR: cannot pus gesture for: "<<_algType<<std::endl;
            return false;
        }
        this->_gestureSample.clear();
    }
    else if(this->_algType == "MLP_R")
    {
        if(!this->_mlpRegressionTrainingData.addSample(_mlpRegressionSample.getInputVector(), 
                    _mlpRegressionSample.getTargetVector()))
        {
            std::cout<<"ERROR: cnnot push the gesture for: "<<_algType<<std::endl;
            return false;
        }
        this->_mlpRegressionSample.clear();

    }
    else if(this->_algType == "MLP_C")
    {
        for(int i=0; i<_mlpClassificationSamples.size();i++)
        {
            if(!this->_mlpClassificationTrainingData.addSample(_mlpClassificationSamples[i].getClassLabel(), 
                        _mlpClassificationSamples[i].getSample()))
            {
                std::cout<<"ERROR: cannot push the gesture for: "<<_algType<<std::endl;
                return false;
            }
        } 
        _mlpClassificationSample.clear();
        _mlpClassificationSamples.clear();

    }
    else
    {
        std::cout<<"ERROR: You forgot to cpecify algorithm"<<std::endl;
        return false;
    }

    return true;
}

/*saving dataset
*/
bool GRGrt::saveDataset()
{
    if(this->_algType == "DTW")
    {
        if(!this->_dtwTrainingData.save(("../trainingData/grTrainingDTW"+this->_fileProp)))
        {
            std::cout<<"ERROR: Failed to save dataset to file"<<std::endl;
            return false;
        }
    }
    else if(this->_algType == "MLP_R")
    {
        if(!this->_mlpRegressionTrainingData.save(("../trainingData/grTrainingMLP_R"+this->_fileProp)))
        {
            std::cout<<"ERROR: Failed to save dataset to file"<<std::endl;
            return false;

        }
    }
    else if(this->_algType == "MLP_C")
    {
        if(!this->_mlpClassificationTrainingData.save(("../trainingData/grtrainingMLP_C"+this->_fileProp)))
        {
            std::cout<<"ERROR: Failed to save dataset to file"<<std::endl;
            return false;
        }

    }
    else
    {
        std::cout<<"ERROR: You forgot to specify recognition algorithm"<<std::endl;
        return false;
    }
    return true;
    //datasetDTW.trainingData.clear();TODO
}
//TODO if needet 
/*
   bool GRGrt::tranQantizationModel(std::string quantizationSettingsFile)
   {
   this->_quantizer = GRT::KMeansQuantizer(_dtwTrainingData.getNumDimensions());

   if(!this->_quantizer.train(this->_dtwTrainingData))
   {
   std::cout<<"ERROR: Failed to train quantizer"<<std::endl;
   return false;
   }

   if(!this->_quantizer.save(quantizationSettingsFile))
   {
   std::cout<<"ERROR: Failed to safe quantizer settings to file: "<<quantizationSettingsFile<<std::endl;
   return false;
   }
   return true;
   }

   bool GRGrt::quantizeData(std::string quantizationSettingsFile)
   {
//TODO need to be fixed 
if(!this->_quantizer.load(quantizationSettingsFile))
{
std::cout<<"ERROR: filed to load quantization settings from file: "<<quantizationSettingsFile<<std::endl;
return false;
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
return true;
}
*/

