#ifndef GR_TRAJECTORY
#define GR_TRAJECTORY

#include <vector>
#include "Eigen/Dense"

using Eigen::Vector3d;
using namespace std;

#define G 0.035157875 // 9.000416 / 256


class GRTrajectory
{
    private:
        Vector3d pos_last;
        Vector3d velocity_last;
        float timestamp_last;

        Vector3d _getNewPosByVelocity(Vector3d, float);
        Vector3d _getNewPosByIntegrating(Vector3d, float);

        vector<float> _toStdVector(Vector3d);
        Vector3d _toVector3d(vector<float>);
    public:
        GRTrajectory();
        ~GRTrajectory();

        vector<float> getNewPosByVelocity(vector<float>, float);
        vector<float> getNewPosByIntegrating(vector<float>, float);

        vector<float> getNewPos(vector<float> acc, float timestamp);
};

#endif
