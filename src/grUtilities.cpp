#include "grUtilities.h"
GRUtilities::GRUtilities()
{

    datasetDTW.sensors.clear();
    datasetDTW.gestureLabel = 1;
    datasetHMM.sensors.clear();
    datasetHMM.gestureLabel = 1;
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

void GRUtilities::setDatasetProperties(std::string dataSetName, std::string infoText, std::string fProp, std::string alg)
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

void GRUtilities::setNextLabel(std::string alg)
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

void GRUtilities::clearTrainingData(std::string alg)
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
bool GRUtilities::pushDatasetDTW(device_t *dev )
{

    
    //UINT gestureLabel = 1;

    int counter = 0;

    for(int i=0; i < datasetDTW.sensors.size(); i++)
    {

       if(!dev->imus[datasetDTW.sensors[i]]->gyro.empty() && !dev->imus[datasetDTW.sensors[i]]->gyro.front().empty() && !dev->imus[datasetDTW.sensors[i]]->acc.empty() && !dev->imus[datasetDTW.sensors[i]]->acc.front().empty() )
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
                 datasetDTW.sample[j+counter] = dev->imus[datasetDTW.sensors[i]]->gyro.front()[j];

            }
            else 
            {
                datasetDTW.sample[(j+counter)] = dev->imus[datasetDTW.sensors[i]]->acc.front()[j-3];
                
            }
        }
        counter +=6;
        }
    }


    usleep(20);
    datasetDTW.trainingSample.push_back(datasetDTW.sample);
    for(int i=0; i<datasetDTW.sensors.size(); i++)
   {
      if(!dev->imus[datasetDTW.sensors[i]]->gyro.empty() && !dev->imus[datasetDTW.sensors[i]]->gyro.front().empty() && !dev->imus[datasetDTW.sensors[i]]->acc.empty() && !dev->imus[datasetDTW.sensors[i]]->acc.front().empty() )
        {
  
                 dev->imus[datasetDTW.sensors[i]]->gyro.pop_front();
                dev->imus[datasetDTW.sensors[i]]->acc.pop_front();
        }
   }


    datasetDTW.trainingData.addSample(datasetDTW.gestureLabel, datasetDTW.trainingSample);

}

bool GRUtilities::pushDatasetHMM(device_t *dev)
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
                datasetHMM.sample[j+counter] = dev->imus[datasetHMM.sensors[i]]->gyro.front()[j];

            }
            else 
            {
                datasetHMM.sample[(j+counter)] = dev->imus[datasetHMM.sensors[i]]->acc.front()[i-3];

            }
        }
        counter +=6;
    }


    usleep(20);
    datasetHMM.trainingSample.push_back(datasetHMM.sample);
    for(int i=0; i<datasetHMM.sensors.size(); i++)
    {
        dev->imus[datasetHMM.sensors[i]]->gyro.pop_front();
        dev->imus[datasetHMM.sensors[i]]->acc.pop_front();
    }


    datasetHMM.trainingData.addSample(datasetHMM.gestureLabel, datasetHMM.trainingSample);

}

bool GRUtilities::saveDataset(std::string alg)
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
