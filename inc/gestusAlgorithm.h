#ifndef GESTUS_ALGORITHM
#define GESTUS_ALGORITHM

#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>
#include <deque>
#include <math>

#include "kalman/ekfilter.hpp"
using namespace std;

class GestutAlgorithm : Kalman::EKFilter<double, 0>
{

    public:
        GestutAlgorithm();

        bool imuEKF();
    private:
        //functions for generating jacobian matrices for kalman filter
        void makeA();
        void makeH();
        void makeV();
        void makeR();
        void makeW();
        void makeQ();
        void makeProccess();
        void makeMeasure();


};


#endif
