#ifndef GR_UTILITIES
#define GR_UTILITIES

#include <iostream>
#include <string>
#include <vector>
#include "GRT/GRT.h"



class GRUtilities
{
    public:
    GRUtilities();
    ~GRUtilities();
    GRUtilities(const GRUtilities&);
    GRUtilities& operator=(const GRUtilities&);

    void setSensors(std::vector<std::string>)


    bool trainDTW(device_t* ,GRT::UINT, std::vector<td::string>);
    bool trainHMM(device_t*, GRT::UINT, std::vector<std::string>);

    private:
    std::vector<std::string> sensors;
    int dimensions;
    GRT::TimeSeriesClassificationData trainingDataDTW;
    GRT::TimeSeriesClassificationData trainingDataHMM;
}
#endif
