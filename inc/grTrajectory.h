#ifndef GR_TRAJECTORY
#define GR_TRAJECTORY

#include <vector>
#include "Eigen/Dense"

using Eigen::Vector3d;
using namespace std;

#define G 9.000416

class GRTrajectory
{
    private:
        Vector3d pos_last;
        Vector3d velocity_last;
        int timestamp_last;

        Vector3d _getNewPosByVelocity(Vector3d, int);
        Vector3d _getNewPosByIntegrating(Vector3d, int);

        vector<double> _toStdVector(Vector3d);
        Vector3d _toVector3d(vector<double>);
    public:
        GRTrajectory();
        ~GRTrajectory();

        vector<double> getNewPosByVelocity(vector<double>, int);
        vector<double> getNewPosByIntegrating(vector<double>, int);

        vector<double> getNewPos(vector<double> acc, int timestamp);
};

#endif
