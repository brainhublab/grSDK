#ifndef GR_TRAJECTORY
#define GR_TRAJECTORY

#include <vector>
#include "Eigen/Dense"
#include "Eigen/Geometry"

using Eigen::Vector3d;
using Eigen::Quaterniond;
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

        Vector3d _rotateAcc(Vector3d, Quaterniond);

        vector<double> _toStdVector(Vector3d);
        Vector3d _toVector3d(vector<double>);
        Quaterniond _toQuaterniond(vector<double>);
    public:
        GRTrajectory();
        ~GRTrajectory();

        vector<double> getNewPosByVelocity(vector<double>, vector<double>, int);
        vector<double> getNewPosByIntegrating(vector<double>, vector<double>, int);

        vector<double> getNewPos(vector<double> acc, vector<double>, int timestamp);
};

#endif
