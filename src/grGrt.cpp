#include <grGrt.h>

GRGrt::GRGrt()
{

//utils
	datasetDTW.sensors.clear();
    datasetDTW.gestureLabel = 1;
    datasetHMM.sensors.clear();
    datasetHMM.gestureLabel = 1;
    //datasetDTW.trainingData.setNumDimensions(datasetDTW.dimensions);
    //datasetDTW.trainingData.setDatasetName("grDTWTrainingData"); //TODO later add relative name 
    //datasetDTW.trainingData.setInfoText("Thisdataset contains some GR data"); //later add relative info text


}

GRGrt::~GRGrt()
{

}

GRGrt::GRGrt(const GRGrt &t)
{
	this->dtw = GRT::DTW(t.dtw);
    this->trainingData = GRT::TimeSeriesClassificationData(t.trainingData);
    this->testData = GRT::TimeSeriesClassificationData(t.testData);

    this->testAccuracy = t.testAccuracy;
}

GRGrt& GRGrt::operator=(const GRGrt& t)
{
	this->dtw = GRT::DTW(t.dtw);
    this->trainingData = GRT::TimeSeriesClassificationData(t.trainingData);
    this->testData = GRT::TimeSeriesClassificationData(t.testData);

    this->testAccuracy = t.testAccuracy;

}
bool GRGrt::loadTrainingData(std::string filepath)
{
    if(!this->trainingData.load(filepath))
    {
        std::cout << "Failed to load training data!\n";
        return false;
    }

    return true;
}

bool GRGrt::loadTestData(std::string filepath)
{
    if(!this->testData.load(filepath))
    {
        std::cout << "Failed to load test data!\n";
        return false;
    }

    return true;
}

bool GRGrt::setTestDataFromTraining(int size)
{
    if(this->trainingData.getNumSamples() == 0)
    {
        std::cout << "No trainig data to use!\n";
        return false;
    }

    if(size < 0 || size > 100)
    {
        std::cout << "Incorrect size!\n";
        return false;
    }

    this->testData = this->trainingData.split(100 - size);

    return true;
}

bool GRGrt::train()
{
    if(!this->dtw.train(this->trainingData))
    {
        std::cout << "Failed to train classifier!\n";
        return false;
    }

    return true;
}

double GRGrt::test()
{
    double accuracy = 0;
    for(GRT::UINT i=0; i<this->testData.getNumSamples(); i++){
        //Get the i'th test sample - this is a timeseries
        GRT::UINT classLabel = this->testData[i].getClassLabel();
        GRT::MatrixDouble timeseries = this->testData[i].getData();

        //Perform a prediction using the classifier
        if( !this->dtw.predict( timeseries ) ){
            std::cout << "Failed to perform prediction for test sampel: " << i <<"\n";
            return EXIT_FAILURE;
        }

        //Get the predicted class label
        GRT::UINT predictedClassLabel = this->dtw.getPredictedClassLabel();
        double maximumLikelihood = this->dtw.getMaximumLikelihood();
        GRT::VectorDouble classLikelihoods = this->dtw.getClassLikelihoods();
        GRT::VectorDouble classDistances = this->dtw.getClassDistances();

        //Update the accuracy
        if( classLabel == predictedClassLabel ) accuracy++;
        std::cout << "TestSample: " << i <<  "\tClassLabel: " << classLabel << "\tPredictedClassLabel: " << predictedClassLabel << "\tMaximumLikelihood: " << maximumLikelihood << std::endl;
    }

    this->testAccuracy = (double(accuracy) / double(testData.getNumSamples())) * 100.0;

    std::cout << "Test Accuracy: " << this->testAccuracy << "%" << std::endl;

    return this->testAccuracy;
}

double GRGrt::getTestAccuracy()
{
    return this->testAccuracy;
}

bool GRGrt::saveModel(std::string filepath)
{
    if(!this->dtw.save(filepath))
    {
        std::cout << "Failed to save the classifier model!\n";
        return false;
    }

    return true;
}

bool GRGrt::loadModel(std::string filepath)
{
    if(!this->dtw.load(filepath))
    {
        std::cout << "Failed to load the classifier model!\n";
        return false;
    }

    return true;
}

bool GRGrt::predict(GRT::MatrixDouble timeseries)
{
    if(!dtw.predict(timeseries))
    {
        std::cout << "Failed to perform prediction!\n";
        return false;
    }

    return true;
}

GRT::UINT GRGrt::getPredictedClassLabel()
{
    return this->dtw.getPredictedClassLabel();
}

double GRGrt::getMaximumLikelihood()
{
    return this->dtw.getMaximumLikelihood();
}
//utils
void GRGrt::setDataSetSensors(std::vector<std::string> sens, std::string alg)
{
    if(alg == "DTW")
    {
        datasetDTW.dimensions = sens.size() * 6; 
        datasetDTW.sample.resize(datasetDTW.dimensions);
        datasetDTW.sensors = sens;
        datasetDTW.trainingData.setNumDimensions(datasetDTW.dimensions);

    }
    else if(alg == "HMM")
    {
        datasetHMM.dimensions = sens.size() * 6; 
        datasetHMM.sample.resize(datasetHMM.dimensions);
        datasetHMM.sensors = sens;
        datasetHMM.trainingData.setNumDimensions(datasetHMM.dimensions);

    }
    else
    {
        std::cout<< "ERROR:Cannot set sensors no such algorithm with name: "<< alg<<std::endl;
        //return EXIT_FAILURE;
    }
}

void GRGrt::setDatasetProperties(std::string dataSetName, std::string infoText, std::string fProp, std::string alg)
{
    if(alg == "DTW")
    {
        datasetDTW.trainingData.setDatasetName(dataSetName);
        datasetDTW.trainingData.setInfoText(infoText);
        datasetDTW.fileProp = fProp;
    }
    else if(alg == "HMM")
    {
        datasetHMM.trainingData.setDatasetName(dataSetName);
        datasetHMM.trainingData.setInfoText(infoText);
        datasetHMM.fileProp = fProp;
    }
    else
    {
        std::cout<<"ERROR: Cannot set dataset properties no such algorithm wih name :"<<alg<<std::endl;
        //return EXIT_FAILURE;
    }
}

void GRGrt::setNextLabel(std::string alg)
{
    if(alg == "DTW")
    {
        datasetDTW.gestureLabel +=1;
    }
    else if(alg == "HMM")
    {
        datasetHMM.gestureLabel +=1;
    }
    else
    {
        std::cout<<"ERROR: Cannot set next label no such algorithm wit name: "<<alg<<std::endl;
        //return EXIT_FAILURE;
    }
}

void GRGrt::clearTrainingData(std::string alg)
{
    if(alg == "DTW")
    {
        datasetDTW.trainingData.clear();
    }
    else if(alg == "HMM")
    {
        datasetHMM.trainingData.clear();
    }

}
bool GRGrt::pushDatasetDTW(device_t *dev )
{

    
    //UINT gestureLabel = 1;

    int counter = 0;

    for(int i=0; i < datasetDTW.sensors.size(); i++)
    {

       if(!dev->imus[datasetDTW.sensors[i]]->data.front().empty() && 
               !dev->imus[datasetDTW.sensors[i]]->data.front().gyro.empty() && 
               !dev->imus[datasetDTW.sensors[i]]->data.front().acc.empty() )
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
                 datasetDTW.sample[j+counter] = dev->imus[datasetDTW.sensors[i]]->data.front().gyro[j];

            }
            else 
            {
                datasetDTW.sample[(j+counter)] = dev->imus[datasetDTW.sensors[i]]->data.front().acc[j-3];
                
            }
        }
        counter +=6;
        }
    }


    usleep(20);
    datasetDTW.trainingSample.push_back(datasetDTW.sample);
    for(int i=0; i<datasetDTW.sensors.size(); i++)
   {
      if(!dev->imus[datasetDTW.sensors[i]]->data.empty() && 
              !dev->imus[datasetDTW.sensors[i]]->data.front().gyro.empty() && 
              !dev->imus[datasetDTW.sensors[i]]->data.front().acc.empty() )
        {
  
                 dev->imus[datasetDTW.sensors[i]]->data.pop_front();
                //dev->imus[datasetDTW.sensors[i]]->acc.pop_front();
        }
   }


    datasetDTW.trainingData.addSample(datasetDTW.gestureLabel, datasetDTW.trainingSample);

}

bool GRGrt::pushDatasetHMM(device_t *dev)
{
    datasetHMM.trainingData.setNumDimensions(datasetHMM.dimensions);
    datasetHMM.trainingData.setDatasetName("grDTWTrainingData"); //TODO later add relative name 
    datasetHMM.trainingData.setInfoText("Thisdataset contains some GR data"); //later add relative info text

    //UINT gestureLabel = 1;

    int counter = 0;

    for(int i=0; i<datasetHMM.sensors.size(); i++)
    {
        for(int j=0; j<6; j++)
        {
            if(j < 3 )
            {
                datasetHMM.sample[j+counter] = dev->imus[datasetHMM.sensors[i]]->data.front().gyro[j];

            }
            else 
            {
                datasetHMM.sample[(j+counter)] = dev->imus[datasetHMM.sensors[i]]->data.front().acc[i-3];

            }
        }
        counter +=6;
    }


    usleep(20);
    datasetHMM.trainingSample.push_back(datasetHMM.sample);
    for(int i=0; i<datasetHMM.sensors.size(); i++)
    {
        dev->imus[datasetHMM.sensors[i]]->data.pop_front();
        //dev->imus[datasetHMM.sensors[i]]->acc.pop_front();
    }


    datasetHMM.trainingData.addSample(datasetHMM.gestureLabel, datasetHMM.trainingSample);

}

bool GRGrt::saveDataset(std::string alg)
{
    if(alg == "DTW")
    {
        if(!datasetDTW.trainingData.save(("data/grTrainingDTW"+datasetDTW.fileProp)))
        {
            std::cout<<"ERROR: Failed to save dataset to file \n";
            return EXIT_FAILURE;
        }

        //datasetDTW.trainingData.clear();TODO

    }
    else if(alg == "HMM")
    {
        if(!datasetHMM.trainingData.save(("data/grTrainingHMM"+datasetHMM.fileProp)))
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
