#ifndef GR_TRAJECTORY
#define GR_TRAJECTORY

#include <vector>
#include "Eigen/Dense"
#include "Eigen/Geometry"

using Eigen::Vector3d;
using Eigen::Quaterniond;
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

        Vector3d _rotateAcc(Vector3d, Quaterniond);

        vector<double> _toStdVector(Vector3d);
        Vector3d _toVector3d(vector<double>);
        Quaterniond _toQuaterniond(vector<double>);
    public:
        GRTrajectory();
        ~GRTrajectory();

        vector<double> getNewPosByVelocity(vector<double>, vector<double>, unsigned long);
        vector<double> getNewPosByIntegrating(vector<double>, vector<double>, unsigned long);

        vector<double> getNewPos(vector<double> acc, vector<double>, unsigned long);
};

#endif
