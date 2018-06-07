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

#include <grConnection.h>
#include <grDevice.h>
#include <grMadgwick.h>
#include <grGrt.h>


#include "Eigen/Dense"
using namespace std;
struct k_filter_vars //variables needet from simplified kalman 
{
    double volt;
    double proccess;
    double pc;
    double g;
    double p;
    double xp;
    double zp;
    double xe;

    std::vector<double> accumulated;
    k_filter_vars()
    {
        volt = 0.0;
        proccess = 0.05;
        pc = 0.0;
        g = 0.0;
        p = 1.0;
        xp = 0.0;
        zp = 0.0;
        xe = 0.0;
    }
};

struct acc_k_vars //arbitary accelerometer vars for kalman
{
    k_filter_vars acc_k_x;
    k_filter_vars acc_k_y;
    k_filter_vars acc_k_z;
};


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
        bool madgwickUpdate(gr_message*, gr_alg_message*);
        //void madgwickUpdateThr(device_t*, alg_device_t*, int, std::string flag);//TODO need to implement
        /**
	 * @brief precondition Madgwick algorithm
	 */
	bool setupMadgwick(int, int, int, int, int, int);

        //simplified Kalman
         
        Eigen::Vector3d kFilterStep(Eigen::Vector3d, acc_k_vars*);
//        bool setUpKfilterCoord(std::vector<std::vector<double> >, acc_k_vars* );
//        bool kFilterStepCoord(std::vector<double>, acc_k_vars*);
             
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
        /**
	 * @brief TODO
	 */



};


#endif
