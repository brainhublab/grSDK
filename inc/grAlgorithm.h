#ifndef GR_ALGORITHM
#define GR_ALGORITHM

#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <deque>
#include <math.h>
#include <thread>
#include <numeric>
#include <functional>
#include <algorithm>

#include "GRT/GRT.h"

//#include "grConnection.h" //TODO:
#include "grDevice.h"
#include "grMadgwick.h"
#include "grGrt.h"

#include "Eigen/Dense"
#include "Eigen/Geometry"
using namespace std;

/**
 * GRAlgorithm class - short descr.
 */
class GRAlgorithm :public GRGrt
{
    public:
        /**
         * @brief constructor
         */
        GRAlgorithm();
        /**
         * @brief destructor
         */
        ~GRAlgorithm();
        /**
         * @brief copy constructor
         */
        GRAlgorithm(const GRAlgorithm& );
        /**
         * @brief assigment operator
         */
        GRAlgorithm& operator=(const GRAlgorithm&);
        /**
         * @brief initialize algorithms
         */
        void grInitAlgorithms();
        //madgwick
        /**
         * @brief Update iterative data of madgwick algorithm
         */
        bool madgwickUpdate(GRMessage*, GRAlgMessage*);
        //void madgwickUpdateThr(device_t*, alg_device_t*, int, std::string flag);//TODO need to implement
        /**
         * @brief precondition Madgwick algorithm
         */
        bool setupMadgwick(int, int, int, int, int, int);

        //simplified Kalman

        // TODO: make const GRAlgMessage in methods for rotations
        /**
         * @brief get specific node rotation from gr algorithm message
         */
        Eigen::Quaterniond getNodeRotation(GRAlgMessage &alg_msg, const std::string& nodeName) const;
        /**
         * @brief get unordered map of roations in quaternion representation
         */
        std::unordered_map<std::string, Eigen::Quaterniond> getRotations(GRAlgMessage alg_msg) const;
        /**
         * @brief get unordered_map of nodename-rotations in [roll, pitch, yaw] representation
         */
        std::unordered_map<std::string, std::vector<double>> getEulerRotations(GRAlgMessage alg_msg) const;

    private:
        /**
         * @brief rotations
         */
        double _roll, _pitch, _yaw;
        /**
         * @brief vector with computed from quaternion angles
         */
        std::vector<double> _angles;
        /**
         * @brief calculation of angles from quaternion method
         */
        std::vector<double> _computeAngles(std::vector<double>);

        //grMadgwick objects
        /**
         * @brief madgwick alg for pinky
         */
        GRMadgwick _pinkyMadgwick;
        /**
         * @brief madgwick alg for ring
         */
        GRMadgwick _ringMadgwick;
        /**
         * @brief madgwick alg for middle
         */
        GRMadgwick _middleMadgwick;
        /**
         * @brief madgwick alg for index
         */
        GRMadgwick _indexMadgwick;
        /**
         * @brief madgwick alg for thumb
         */
        GRMadgwick _thumbMadgwick;
        /**
         * @brief madgwick alg for palm
         */
        GRMadgwick _palmMadgwick;
        /**
         * @brief quaternions for madgwickUpdate
         */
        Eigen::Quaterniond quat;
        Eigen::Quaterniond palmQuat;
        Eigen::Quaterniond relativeQuat;
        std::array<double, 4> _rotations;

        /**
         * @brief map for easier access of objects
         */
        std::unordered_map<std::string, GRMadgwick*> _madgwickObjects;

        double _stDev(std::vector<double>*);
        /**
         * @brief average
         */
        double _average(std::vector<double>*);//average
        /**
         * @brief TODO
         */
        double _stdErr(std::vector<double>*);



};


#endif
