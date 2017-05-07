#include "grUtilities.h"
GRUtilities::GRUtilities()
{

    this->datasetDTW.sensors.clear();
    this->datasetDTW.gestureLabel = 1;
    this->datasetHMM.sensors.clear();
    this->datasetHMM.gestureLabel = 1;
    //datasetDTW.trainingData.setNumDimensions(datasetDTW.dimensions);
    //datasetDTW.trainingData.setDatasetName("grDTWTrainingData"); //TODO later add relative name 
    //datasetDTW.trainingData.setInfoText("Thisdataset contains some GR data"); //later add relative info text

}

GRUtilities::~GRUtilities()
{

}

GRUtilities::GRUtilities(const GRUtilities &t)
{

}

GRUtilities& GRUtilities::operator=(const GRUtilities &t)
{

}

void GRUtilities::setSensors(std::vector<std::string> sens, std::string alg)
{
    if(alg == "DTW")
    {
        this->datasetDTW.dimensions = sens.size() * 6; 
        this->datasetDTW.sample.resize(datasetDTW.dimensions);
        this->datasetDTW.sensors = sens;
        this->datasetDTW.trainingData.setNumDimensions(datasetDTW.dimensions);

    }
    else if(alg == "HMM")
    {
        this->datasetHMM.dimensions = sens.size() * 6; 
        this->datasetHMM.sample.resize(datasetHMM.dimensions);
        this->datasetHMM.sensors = sens;
        this->datasetHMM.trainingData.setNumDimensions(datasetHMM.dimensions);

    }
    else
    {
        std::cout<< "ERROR:Cannot set sensors no such algorithm with name: "<< alg<<std::endl;
        //return EXIT_FAILURE;
    }
}

void GRUtilities::setDatasetProperties(std::string dataSetName, std::string infoText, std::string fProp, std::string alg)
{
    if(alg == "DTW")
    {
        this->datasetDTW.trainingData.setDatasetName(dataSetName);
        this->datasetDTW.trainingData.setInfoText(infoText);
        this->datasetDTW.fileProp = fProp;
    }
    else if(alg == "HMM")
    {
        this->datasetHMM.trainingData.setDatasetName(dataSetName);
        this->datasetHMM.trainingData.setInfoText(infoText);
        this->datasetHMM.fileProp = fProp;
    }
    else
    {
        std::cout<<"ERROR: Cannot set dataset properties no such algorithm wih name :"<<alg<<std::endl;
        //return EXIT_FAILURE;
    }
}

void GRUtilities::setNextLabel(std::string alg)
{
    if(alg == "DTW")
    {
        this->datasetDTW.gestureLabel +=1;
    }
    else if(alg == "HMM")
    {
        this->datasetHMM.gestureLabel +=1;
    }
    else
    {
        std::cout<<"ERROR: Cannot set next label no such algorithm wit name: "<<alg<<std::endl;
        //return EXIT_FAILURE;
    }
}

void GRUtilities::clearTrainingData(std::string alg)
{
    if(alg == "DTW")
    {
        this->datasetDTW.trainingData.clear();
    }
    else if(alg == "HMM")
    {
        this->datasetHMM.trainingData.clear();
    }

}
bool GRUtilities::pushDatasetDTW(device_t *dev )
{

    
    //UINT gestureLabel = 1;

    int counter = 0;

    for(int i=0; i<this->datasetDTW.sensors.size(); i++)
    {

        if(dev->imus[this->datasetDTW.sensors[i]]->gyro.front().size() == 3 &&
              dev->imus[this->datasetDTW.sensors[i]]->acc.front().size() == 3)
        {
                   
        for(int j=0; j<6; j++)
        {

            if(j < 3 )
            {
                //std::cout<<datasetDTW.sensors[i]<<std::endl;
                //std::cout<<"F"<<dev->imus["palm"]->gyro.front()[j]<<std::endl;
                //std::cout<<"D "<<(*dev->imus[datasetDTW.sensors[i]]).gyro.front()[j]<<std::endl;
                std::cout<<"J "<<j<<std::endl;
                {
                    std::cout<<dev->imus[this->datasetDTW.sensors[i]]->gyro.front().size()<<std::endl;
                    this->datasetDTW.sample[j+counter] = dev->imus[this->datasetDTW.sensors[i]]->gyro.front()[j];
                }

            }
            else 
            {
                this->datasetDTW.sample[(j+counter)] = dev->imus[this->datasetDTW.sensors[i]]->acc.front()[j-3];

            }
        }
        counter +=6;
        }
    }


    usleep(20);
    this->datasetDTW.trainingSample.push_back(this->datasetDTW.sample);
    for(int i=0; i<this->datasetDTW.sensors.size(); i++)
    {
        dev->imus[this->datasetDTW.sensors[i]]->gyro.pop_front();
        dev->imus[this->datasetDTW.sensors[i]]->acc.pop_front();
    }


    this->datasetDTW.trainingData.addSample(this->datasetDTW.gestureLabel, this->datasetDTW.trainingSample);

}

bool GRUtilities::pushDatasetHMM(device_t *dev)
{
    this->datasetHMM.trainingData.setNumDimensions(this->datasetHMM.dimensions);
    this->datasetHMM.trainingData.setDatasetName("grDTWTrainingData"); //TODO later add relative name 
    this->datasetHMM.trainingData.setInfoText("Thisdataset contains some GR data"); //later add relative info text

    //UINT gestureLabel = 1;

    int counter = 0;

    for(int i=0; i<this->datasetHMM.sensors.size(); i++)
    {
        for(int j=0; j<6; j++)
        {
            if(j < 3 )
            {
                this->datasetHMM.sample[j+counter] = dev->imus[this->datasetHMM.sensors[i]]->gyro.front()[j];

            }
            else 
            {
                this->datasetHMM.sample[(j+counter)] = dev->imus[this->datasetHMM.sensors[i]]->acc.front()[i-3];

            }
        }
        counter +=6;
    }


    usleep(20);
    this->datasetHMM.trainingSample.push_back(this->datasetHMM.sample);
    for(int i=0; i<this->datasetHMM.sensors.size(); i++)
    {
        dev->imus[this->datasetHMM.sensors[i]]->gyro.pop_front();
        dev->imus[this->datasetHMM.sensors[i]]->acc.pop_front();
    }


    this->datasetHMM.trainingData.addSample(this->datasetHMM.gestureLabel, this->datasetHMM.trainingSample);

}

bool GRUtilities::saveDataset(std::string alg)
{
    if(alg == "DTW")
    {
        if(!this->datasetDTW.trainingData.save(("data/grTrainingDTW"+this->datasetDTW.fileProp)))
        {
            std::cout<<"ERROR: Failed to save dataset to file \n";
            return EXIT_FAILURE;
        }

        //datasetDTW.trainingData.clear();TODO

    }
    else if(alg == "HMM")
    {
        if(!this->datasetHMM.trainingData.save(("data/grTrainingHMM"+this->datasetHMM.fileProp)))
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
