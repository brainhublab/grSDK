#include <grGrt.h>
/*constructor
 */
GRGrt::GRGrt()
{

//utils
	_datasetDTW.sensors.clear();
    _datasetDTW.gestureLabel = 1;
    _datasetHMM.sensors.clear();
    _datasetHMM.gestureLabel = 1;
    //datasetDTW.trainingData.setNumDimensions(datasetDTW.dimensions);
    //datasetDTW.trainingData.setDatasetName("grDTWTrainingData"); //TODO later add relative name 
    //datasetDTW.trainingData.setInfoText("Thisdataset contains some GR data"); //later add relative info text


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
bool GRGrt::loadTrainingData(std::string filepath)
{
    if(!this->_trainingData.load(filepath))
    {
        std::cout << "Failed to load training data!\n";
        return false;
    }

    return true;
}

/*loading test data from filepath
 */
bool GRGrt::loadTestData(std::string filepath)
{
    if(!this->_testData.load(filepath))
    {
        std::cout << "Failed to load test data!\n";
        return false;
    }

    return true;
}

/* setting of test data with providing of size of dataset 
 */
bool GRGrt::setTestDataFromTraining(int size)
{
    if(this->_trainingData.getNumSamples() == 0)
    {
        std::cout << "No trainig data to use!\n";
        return false;
    }

    if(size < 0 || size > 100)
    {
        std::cout << "Incorrect size!\n";
        return false;
    }

    this->_testData = this->_trainingData.split(100 - size);

    return true;
}

/* training 
 */
bool GRGrt::train()
{
    if(!this->_dtw.train(this->_trainingData))
    {
        std::cout << "Failed to train classifier!\n";
        return false;
    }

    return true;
}

/*testing of trained data
 */
double GRGrt::test()
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
void GRGrt::setDataSetSensors(std::vector<std::string> sens, std::string alg)
{
    if(alg == "DTW")
    {
        _datasetDTW.dimensions = sens.size() * 6; 
        _datasetDTW.sample.resize(_datasetDTW.dimensions);
        _datasetDTW.sensors = sens;
        _datasetDTW.trainingData.setNumDimensions(_datasetDTW.dimensions);

    }
    else if(alg == "HMM")
    {
        _datasetHMM.dimensions = sens.size() * 6; 
        _datasetHMM.sample.resize(_datasetHMM.dimensions);
        _datasetHMM.sensors = sens;
        _datasetHMM.trainingData.setNumDimensions(_datasetHMM.dimensions);

    }
    else
    {
        std::cout<< "ERROR:Cannot set sensors no such algorithm with name: "<< alg<<std::endl;
        //return EXIT_FAILURE;
    }
}

/*properties for dataset
 */
void GRGrt::setDatasetProperties(std::string dataSetName, std::string infoText, std::string fProp, std::string alg)
{
    if(alg == "DTW")
    {
        _datasetDTW.trainingData.setDatasetName(dataSetName);
        _datasetDTW.trainingData.setInfoText(infoText);
        _datasetDTW.fileProp = fProp;
    }
    else if(alg == "HMM")
    {
        _datasetHMM.trainingData.setDatasetName(dataSetName);
        _datasetHMM.trainingData.setInfoText(infoText);
        _datasetHMM.fileProp = fProp;
    }
    else
    {
        std::cout<<"ERROR: Cannot set dataset properties no such algorithm wih name :"<<alg<<std::endl;
        //return EXIT_FAILURE;
    }
}

/*set nex label in training data
 */
void GRGrt::setNextLabel(std::string alg)
{
    if(alg == "DTW")
    {
        _datasetDTW.gestureLabel +=1;
    }
    else if(alg == "HMM")
    {
        _datasetHMM.gestureLabel +=1;
    }
    else
    {
        std::cout<<"ERROR: Cannot set next label no such algorithm wit name: "<<alg<<std::endl;
        //return EXIT_FAILURE;
    }
}

/*clearing
 */
void GRGrt::clearTrainingData(std::string alg)
{
    if(alg == "DTW")
    {
        _datasetDTW.trainingData.clear();
    }
    else if(alg == "HMM")
    {
        _datasetHMM.trainingData.clear();
    }

}

/*pushing dataset in DTW algorithm
 */
bool GRGrt::pushDatasetDTW(gr_message* data )
{

    
    //UINT gestureLabel = 1;

    int counter = 0;

    for(int i=0; i < _datasetDTW.sensors.size(); i++)
    {

       if(!data->imus[_datasetDTW.sensors[i]]->empty() && 
               !data->imus[_datasetDTW.sensors[i]]->gyro.empty() && 
               !data->imus[_datasetDTW.sensors[i]]->empty() )
        {
                   
        for(int j=0; j<6; j++)
        {

            if(j < 3 )
            {
                //std::cout<<datasetDTW.sensors[i]<<std::endl;
                //std::cout<<"F"<<dev->imus["palm"]->gyro.front()[j]<<std::endl;
                //std::cout<<"D "<<(*dev->imus[datasetDTW.sensors[i]]).gyro.front()[j]<<std::endl;
               // std::cout<<"J "<<j<<std::endl;
                 //   std::cout<<dev->imus[datasetDTW.sensors[i]]->gyro.front().size()<<std::endl;
                 _datasetDTW.sample[j+counter] = data->imus[_datasetDTW.sensors[i]]->gyro[j];

            }
            else 
            {
                _datasetDTW.sample[(j+counter)] = data->imus[_datasetDTW.sensors[i]]->acc[j-3];
                
            }
        }
        counter +=6;
        }
    }


    _datasetDTW.trainingSample.push_back(_datasetDTW.sample);
  
    _datasetDTW.trainingData.addSample(_datasetDTW.gestureLabel, _datasetDTW.trainingSample);

}

/* pushing data in HMM algorithm
 */
bool GRGrt::pushDatasetHMM(gr_message* data)
{
    _datasetHMM.trainingData.setNumDimensions(_datasetHMM.dimensions);
    _datasetHMM.trainingData.setDatasetName("grDTWTrainingData"); //TODO later add relative name 
    _datasetHMM.trainingData.setInfoText("Thisdataset contains some GR data"); //later add relative info text

    //UINT gestureLabel = 1;

    int counter = 0;

    for(int i=0; i<_datasetHMM.sensors.size(); i++)
    {
        for(int j=0; j<6; j++)
        {
            if(j < 3 )
            {
                _datasetHMM.sample[j+counter] = data->imus[_datasetHMM.sensors[i]]->gyro[j];

            }
            else 
            {
                _datasetHMM.sample[(j+counter)] = data->imus[_datasetHMM.sensors[i]]->acc[i-3];

            }
        }
        counter +=6;
    }


    usleep(20);
    _datasetHMM.trainingSample.push_back(_datasetHMM.sample);


    _datasetHMM.trainingData.addSample(_datasetHMM.gestureLabel, _datasetHMM.trainingSample);

}


/*saving dataset
 */
bool GRGrt::saveDataset(std::string alg)
{
    if(alg == "DTW")
    {
        if(!_datasetDTW.trainingData.save(("data/grTrainingDTW"+_datasetDTW.fileProp)))
        {
            std::cout<<"ERROR: Failed to save dataset to file \n";
            return EXIT_FAILURE;
        }

        //datasetDTW.trainingData.clear();TODO

    }
    else if(alg == "HMM")
    {
        if(!_datasetHMM.trainingData.save(("data/grTrainingHMM"+_datasetHMM.fileProp)))
        {
            std::cout<<"ERROR: Failed to save dataset to file \n";
            return EXIT_FAILURE;
        }

        //datasetHMM.trainingData.clear();TODO

    }
    else
    {
        std::cout<<"ERROR: Cannot save wrong name of algorithm: "<<alg<<std::endl;
    }
}
