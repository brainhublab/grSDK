#include "grUtilities.h"
GRUtilities::GRUtilities()
{
   sensors.clean();
}
void GRUtilities::setSensors(std::vector<std::string> sens)
{
    dimensions = sens.size() * 6 
    sensors = sens;
}
bool GRUtilities::trainDTW(device_t *dev, GRT::UINT gestureLabel )
{
    
   trainingDataDTW.setNumDimensions(dimensions);
   trainingDataDTW.setDatasetName("grDTWTrainingData"); //TODO later add relative name 
   trainingDataDTW.setInfoText("Thisdataset contains some GR data"); //later add relative info text

   //UINT gestureLabel = 1;
   VectorDouble = sample(dimensions);

   MatrixDouble trainingSample;

   int couter = 0;
   int sens_count = 0;
   if(dimensions == 6)
   { 
       for(int i=0; < dimensions; i++)
       {
           if(i < 3)
           {
            sample[i] = dev.sensor.gyro.front()[i];
           }
           else
           {
            sample[i] = dev.sensor.acc.front()[i-3];
           }
       }
       dev.sensor.gyro.pop_front();
       dev.sensor.acc.pop_front();
   }
   else if(dimensions == 36)
   {
       for(int i=0; i<6; i++)
       {
           if(i < 3 )
           {
               sample[i] = dev.sensors[0].gyro.front()[i];
               sample[i+6] = dev.sensors[1].gyro.front()[i];
               sample[i+12] = dev.sensors[2].gyro.front()[i];
               sample[i+18] = dev.sensors[3].gyro.front()[i];
               sample[i+24] = dev.sensors[4].gyro.front()[i];
               sample[i+30] = dev.sensors[5].gyro.front()[i];


           }
           else 
           {
               sample[i] = dev.sensors[0].acc.front()[i-3];
               sample[i+6] = dev.sensors[1].acc.front()[i-3];
               sample[i+12] = dev.sensors[2].acc.front()[i-3];
               sample[i+18] = dev.sensors[3].acc.front()[i-3];
               sample[i+24] = dev.sensors[4].acc.front()[i-3];
               sample[i+30] = dev.sensors[5].acc.front()[i-3];
           }

       }
      usleep(20)
      trainingSample.push_back(sample);
      for(int i=0; i<6; i++)
      {
        dev.sensors[i].gyro.pop_front();
        dev.sensors[i].acc.pop_front();
      }

   }
   trainingDataDTW.addSample(gestureLabel, trainingSample);
   
   if(!trainingDataDTW.save("data/grTrainingDTW"))
   {
    std::cout<<"ERROR: Failed to save dataset to file \n";
    return EXIT_FAILURE;
   }
   trainingDataDTW.clear();
}
