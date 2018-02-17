#include "grTrajectory.h"

GRTrajectory::GRTrajectory()
{
    this->_timestampLast = 0;
    this->_isStationary = true;
    this->_accTreshold = 0.6;
    this->_gyroTreshold = 0.6;
}

GRTrajectory::~GRTrajectory()
{}

/*
 * Private method which convert's raw accelerometer data units to G
 * It's take one argument Eigen::Vector3d with raw accelerometer data
 * and return Eigen::Vector3d with accelerometer data in G units
 */
Eigen::Vector3d GRTrajectory::_convertAccToG(Eigen::Vector3d in)
{
    Eigen::Vector3d out;

    out = ((in * ACC_MULT) / 1000);

    return out;
}

/*
 * Private method which convert's raw accelerometer data units to m/s^2
 * It's take one argument Eigen::Vector3d with raw accelerometer data
 * and return Eigen::Vector3d with accelerometer data in m/s^2 units
 */
Eigen::Vector3d GRTrajectory::_convertAccToMS(Eigen::Vector3d in)
{
    Eigen::Vector3d out;

    out = ((in * ACC_MULT) / 1000) * G;

    return out;
}

/*
 * Private method for rotation accelerometer vector by quaternion
 * This is needed for removing of gravity from calculations
 * It's take two arguments Eigen::Vector3d woth raw accelerometer data and
 * Eigen::Quaterniond with quaternion and it's return rotated Eigen::Vector3d vector
 */
Eigen::Vector3d GRTrajectory::_rotateAcc(Eigen::Vector3d acc, Eigen::Quaterniond q)
{
    Eigen::Quaterniond accQ, outQ;
    Eigen::Vector3d out;

    accQ.w() = 0;
    accQ.vec() = acc;

    outQ = q * accQ * q.conjugate();
    out = outQ.vec();

    return out;
}

Eigen::Vector3d GRTrajectory::_correctVector(Eigen::Vector3d accIn)
{
    Eigen::Vector3d dirs, lastDirs, out;

    lastDirs = accIn - _accLast;

    for(int i=0; i<3; i++)
    {

        dirs(i) = accIn(i) / abs(accIn(i));
        lastDirs(i) /= abs(lastDirs(i));
        out(i) = dirs(i) * lastDirs(i);
    }
    return out;

}
vector<double> GRTrajectory::getNewPosByRunge(vector<double> accIn, vector<double> qIn, unsigned long timestamp)
{
    //std::cout << "progress..." << std::endl;

    if(this->_timestampLast == 0)
    {
        Eigen::Vector3d acc;
        Eigen::Quaterniond q;
        std::vector<double> initialPos;

        this->_timestampLast = timestamp;

        q = this->_toQuaterniond(qIn);
        acc = this->_toVector3d(accIn);
        acc = this->_convertAccToG(acc);
        acc *= G;
        acc = this->_rotateAcc(acc, q);

        acc = acc - this->_gravity;

        this->_accLast = acc;

        initialPos = {0.0, 0.0, 0.0};

        return initialPos;
    }

    Eigen::Vector3d acc, accTmp, correctionVector;
    Eigen::Quaterniond q;
    runge_vars rungeNew;
    double dt;

    acc = this->_toVector3d(accIn);
    q = this->_toQuaterniond(qIn);

    dt = (timestamp - this->_timestampLast) / 1000.f;
    this->_timestampLast = timestamp;

    acc = this->_convertAccToG(acc);
    acc *= G;
    acc = this->_rotateAcc(acc, q);
    acc = acc - this->_gravity;
    accTmp = acc;

    acc -= this->_accLast;
    if(acc.norm() < _accTreshold)
    {
        _isStationary = true;
    }
    else
    {
        _isStationary = false;
    }
    correctionVector = _correctVector(accTmp);
   // std::cout<<"correctionVector "<<correctionVector<<std::endl;
   /* for(int i=0; i<3; i++)
    {
        acc(i) *= correctionVector(i);
    }
    */
    //std::cout << dt << std::endl;
    //std::cout << acc(0) << " " << acc(1) << " " << acc(2) << std::endl;

    rungeNew = this->_rk4(acc, this->_rungeLast, dt);

    this->_rungeLast = rungeNew;
    this->_accLast = accTmp;

    return _toStdVector(rungeNew.pos);
}

std::vector<double> GRTrajectory::getAccelerations(std::vector<double> acc_in, std::vector<double> q_in)
{
    Eigen::Vector3d acc, gravity={0, 0, 1};
    Eigen::Quaterniond q;
    acc = this->_toVector3d(acc_in);
    q = this->_toQuaterniond(q_in);
    acc = this->_convertAccToG(acc);
    // acc *= G;
    acc = this->_rotateAcc(acc, q);
    // acc = acc - this->_gravity;
    acc = acc - gravity;

    // // quantization of data, 0 to g = 1 to 10, g to 2g = 11 to 15,
    // // over 2g = 16, same for negatives
    // for(int i = 0; i < 3; i++)
    // {
    //     if(abs(acc(i)) <= 1)
    //     {
    //         acc(i) = round(acc(i) * 9) + 1;
    //     }
    //     else if(abs(acc(i)) <= 2)
    //     {
    //         acc(i) = round((acc(i) - 1) * 4) + 11;
    //     }
    //     else
    //     {
    //         acc(i) = 16 * (acc(i) / abs(acc(i)));
    //     }
    // }

    return _toStdVector(acc);

}
runge_vars GRTrajectory::_rk4(Eigen::Vector3d accIn, runge_vars rungeIn, double dt)
{
    runge_vars r1, r2, r3, r4, out;

    r1 = this->_rungeLast;

    r2.pos = r1.pos + 0.5 * r1.vel * dt;
    r2.vel = r1.vel + 0.5 * r1.acc * dt;
    r2.acc = r1.acc + 0.33 * (accIn - r1.acc);

    r3.pos = r1.pos + 0.5 * r2.vel * dt;
    r3.vel = r1.vel + 0.5 * r2.acc * dt;
    r3.acc = r1.acc + 0.66 * (accIn - r1.acc);

    r4.pos = r1.pos + r3.vel * dt;
    r4.vel = r1.vel + r3.acc * dt;
    r4.acc = accIn;

    out.pos = r1.pos + (dt / 6.0) * (r1.vel + 2 * r2.vel + 2 * r3.vel + r4.vel);
    out.vel = r1.vel + (dt / 6.0) * (r1.acc + 2 * r2.acc + 2 * r3.acc + r4.acc);
    out.acc = accIn;

    return out;

}

/*
 * Private method for converting of Eigen::Vector3d to sdt::vector
 */
vector<double> GRTrajectory::_toStdVector(Eigen::Vector3d in)
{
    vector<double> out = {in(0), in(1), in(2)};
    return out;
}

/*
 * Private method for converting of std::vector to Eigen::Vector3d
 */
Eigen::Vector3d GRTrajectory::_toVector3d(vector<double> in)
{
    Eigen::Vector3d out(in[0], in[1], in[2]);
    return out;
}

/*
 * Private method for converting std::vector with Madgwick quaternion to
 * Eigen::Quaterniond
 */
Eigen::Quaterniond GRTrajectory::_toQuaterniond(vector<double> in)
{
    Eigen::Quaterniond out(in[0], in[1], in[2], in[3]);
    return out;
}

bool GRTrajectory::isStationary()
{
   return _isStationary;
}

#include <nbind/nbind.h>
// problems with #define ?
NBIND_CLASS(GRTrajectory) {
    construct<>();
    construct<GRTrajectory>();
}
