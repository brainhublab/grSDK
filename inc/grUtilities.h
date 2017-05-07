#ifndef GR_UTILITIES
#define GR_UTILITIES

#include <iostream>
#include <string>
#include <vector>

#include "grDevice.h"
#include "grDataAttributes.h"

#include "GRT/GRT.h"

//using namespace GRT;


class GRUtilities
{
    public:
    GRUtilities();
    ~GRUtilities();
    GRUtilities(const GRUtilities&);
    GRUtilities& operator=(const GRUtilities&);

    void setSensors(std::vector<std::string>, std::string);
    void setDatasetProperties(std::string, std::string, std::string, std::string);
    void setNextLabel(std::string);
    void clearTrainingData(std::string); 

    bool pushDatasetDTW(device_t*);
    bool pushDatasetHMM(device_t*);

    bool saveDataset(std::string);

    private:

    data_attributes datasetDTW;
    data_attributes datasetHMM;
};
#endif
