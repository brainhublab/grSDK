#ifndef GR_UTILITIES
#define GR_UTILITIES

#include <iostream>
#include <string>
#include <vector>

#include "grDevice.h"
#include "grDataAttributes.h"

#include "grGrt.h"

#include "GRT/GRT.h"


class GRUtilities : public GRGrt 
{
    public:
    GRUtilities();
    ~GRUtilities();
    GRUtilities(const GRUtilities&);
    GRUtilities& operator=(const GRUtilities&);

    private:

};
#endif
