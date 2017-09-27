#include "grTrajectory.h"

GRTrajectory::GRTrajectory()
{
    this->timestampLast = 0;
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

vector<double> GRTrajectory::getNewPosByRunge(vector<double> accIn, vector<double> qIn, unsigned long timestamp)
{
    std::cout << "progress..." << std::endl;

    if(this->timestampLast == 0)
    {
        Eigen::Vector3d acc;
        Eigen::Quaterniond q;
        std::vector<double> initialPos;

        this->timestampLast = timestamp;

        q = this->_toQuaterniond(qIn);
        acc = this->_toVector3d(accIn);
        acc = this->_convertAccToG(acc);
        acc *= G;
        acc = this->_rotateAcc(acc, q);

        acc = acc - this->_gravity;

        this->acc_last = acc;

        initialPos = {0.0, 0.0, 0.0};

        return initialPos;
    }

    Eigen::Vector3d acc, accTmp;
    Eigen::Quaterniond q;
    runge_vars rungeNew;
    double dt;

    acc = this->_toVector3d(accIn);
    q = this->_toQuaterniond(qIn);

    dt = (timestamp - this->timestampLast) / 1000.f;
    this->timestampLast = timestamp;

    acc = this->_convertAccToG(acc);
    acc *= G;
    acc = this->_rotateAcc(acc, q);
    acc = acc - this->_gravity;
    accTmp = acc;

    acc -= this->acc_last;

    std::cout << dt << std::endl;
    std::cout << acc(0) << " " << acc(1) << " " << acc(2) << std::endl;

    rungeNew = this->_rk4(acc, this->rungeLast, dt);

    this->rungeLast = rungeNew;
    this->acc_last = accTmp;

    return _toStdVector(rungeNew.pos);
}

runge_vars GRTrajectory::_rk4(Eigen::Vector3d accIn, runge_vars rungeIn, double dt)
{
    runge_vars r1, r2, r3, r4, out;

    r1 = this->rungeLast;

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
