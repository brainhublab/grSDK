#ifndef GR_TRAJECTORY
#define GR_TRAJECTORY

#include <vector>
#include "Eigen/Dense"

using Eigen::Vector3d;
using namespace std;

#define G  3.3490547936e-05// 9.000416 need to be 9.80665 


class GRTrajectory
{
    private:
        Vector3d pos_last;
        Vector3d velocity_last;
        unsigned long timestamp_last;

        Vector3d _getNewPosByVelocity(Vector3d, unsigned long);
        Vector3d _getNewPosByIntegrating(Vector3d, unsigned long);

        vector<float> _toStdVector(Vector3d);
        Vector3d _toVector3d(vector<float>);
    public:
        GRTrajectory();
        ~GRTrajectory();

        vector<float> getNewPosByVelocity(vector<float>, unsigned long);
        vector<float> getNewPosByIntegrating(vector<float>, unsigned long);

        vector<float> getNewPos(vector<float> acc, float timestamp);
};

#endif
