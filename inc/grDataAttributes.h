#ifndef GR_DATA_ATTRIBUTES
#define GR_DATA_ATTRIBUTES

#include <string>
#include <vector>
#include "GRT/GRT.h"

struct data_attributes
{
    std::vector<std::string> sensors;
    GRT::UINT gestureLabel;
    int dimensions;
    GRT::VectorDouble sample;
    GRT::MatrixDouble trainingSample;
    GRT::TimeSeriesClassificationData trainingData;
    std::string fileProp;

    data_attributes()
    {
        sensors.clear();
        gestureLabel = 0;
        dimensions = 0;
        fileProp.clear();
    }

};

#endif
