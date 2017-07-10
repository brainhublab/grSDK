#include <iostream>
#include "grTrajectory.h"

GRTrajectory::GRTrajectory()
{
    this->timestamp_last = 0;
    this->pos_last(0) = 0;
    this->pos_last(1) = 0;
    this->pos_last(2) = 0;

    this->velocity_last(0) = 0;
    this->velocity_last(1) = 0;
    this->velocity_last(2) = 0;
}

GRTrajectory::~GRTrajectory()
{}

Vector3d GRTrajectory::_getNewPosByVelocity(Vector3d acc, unsigned long timestamp)
{
    std::cout<<"timestamp -->"<<timestamp<<std::endl;

    Vector3d velocity, distance, pos_next;
    double time_elapsed = (timestamp - this->timestamp_last) / 1000.f;
    std::cout<<"time elapsed -->"<<time_elapsed<<std::endl;
    velocity = this->velocity_last + ((((acc * 0.244) / 1000) * G) * time_elapsed);
    distance = velocity * time_elapsed;

    pos_next = this->pos_last + distance;
    this->velocity_last = velocity;
    return pos_next;
}

Vector3d GRTrajectory::_getNewPosByIntegrating(Vector3d acc, unsigned long timestamp)
{
    Vector3d pos_next;
    double time_elapsed = (timestamp - this->timestamp_last) / 1000.f;
    std::cout<<"temp acc --->" <<((acc * 0.244) / 1000) <<std::endl;

    pos_next = this->pos_last + this->velocity_last * time_elapsed + 0.5 * (((acc * 0.244) / 1000) * G) * (time_elapsed * time_elapsed);

    this->velocity_last = this->velocity_last + ((((acc * 0.244) / 1000) * G) * time_elapsed);
    return pos_next;
}

vector<double> GRTrajectory::_toStdVector(Vector3d in)
{
    vector<double> out = {in(0), in(1), in(2)};
    return out;
}

Vector3d GRTrajectory::_toVector3d(vector<double> in)
{
    Vector3d out(in[0], in[1], in[2]);
    return out;
}

Quaterniond GRTrajectory::_toQuaterniond(vector<double> in)
{
    Quaterniond out(in[0], in[1], in[2], in[3]);
    return out;
}

Vector3d GRTrajectory::_rotateAcc(Vector3d acc, Quaterniond q)
{
    q.normalize();

    Quaterniond acc_q;
    acc_q.w() = 0;
    acc_q.vec() = acc;

    Quaterniond out_q = q * acc_q * q.inverse();
    Vector3d out = out_q.vec();
    return out;
}

vector<double> GRTrajectory::getNewPosByVelocity(vector<double> acc, vector<double> q, unsigned long timestamp)
{
    Vector3d acc_v, acc_fixed, pos_next;
    Quaterniond q_q;

    acc_v = this->_toVector3d(acc);
    q_q = this->_toQuaterniond(q);
    acc_fixed = this->_rotateAcc(acc_v, q_q);

    pos_next = this->_getNewPosByVelocity(acc_fixed, timestamp);
    this->pos_last = pos_next;
    this->timestamp_last = timestamp;

    return this->_toStdVector(pos_next);
}

vector<double> GRTrajectory::getNewPosByIntegrating(vector<double> acc, vector<double> q, unsigned long timestamp)
{
    if(this->timestamp_last == 0)
    {
        this->timestamp_last = timestamp;
        return this->_toStdVector(this->pos_last);
    }
    Vector3d acc_v, acc_fixed, pos_next;
    Quaterniond q_q;

    acc_v = this->_toVector3d(acc);
    q_q = this->_toQuaterniond(q);
    acc_fixed = this->_rotateAcc(acc_v, q_q);

    std::cout << "acc_fixed" << std::endl << acc_fixed << std::endl;

    pos_next = this->_getNewPosByIntegrating(acc_fixed, timestamp);
    this->pos_last = pos_next;
    this->timestamp_last = timestamp;

    return this->_toStdVector(pos_next);
}

vector<double> GRTrajectory::getNewPos(vector<double> acc, vector<double> q, unsigned long timestamp)
{
    Vector3d acc_v, acc_fixed, speed_pos, integrated_pos, pos_next;
    Quaterniond q_q;

    acc_v = this->_toVector3d(acc);
    q_q = this->_toQuaterniond(q);
    acc_fixed = this->_rotateAcc(acc_v, q_q);

    speed_pos = this->_getNewPosByVelocity(acc_fixed, timestamp);
    integrated_pos = this->_getNewPosByIntegrating(acc_fixed, timestamp);

    pos_next = (speed_pos + integrated_pos) / 2;

    this->pos_last = pos_next;
    this->timestamp_last = timestamp;

    return this->_toStdVector(pos_next);
}
