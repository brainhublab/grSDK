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
    float time_elapsed = (timestamp - this->timestamp_last) / 1000.f;
    std::cout<<"time elapsed -->"<<time_elapsed<<std::endl;
    velocity = this->velocity_last + ((acc * G) * time_elapsed);
    distance = velocity * time_elapsed;

    pos_next = this->pos_last + distance;

    return pos_next;
}

Vector3d GRTrajectory::_getNewPosByIntegrating(Vector3d acc, unsigned long timestamp)
{
    Vector3d pos_next;
    float time_elapsed = (timestamp - this->timestamp_last) / 1000.f;

    pos_next = this->pos_last + this->velocity_last * time_elapsed + 0.5 * (acc * G) * (time_elapsed * time_elapsed);

    return pos_next;
}

vector<float> GRTrajectory::_toStdVector(Vector3d in)
{
    vector<float> out = {in(0), in(1), in(2)};
    return out;
}

Vector3d GRTrajectory::_toVector3d(vector<float> in)
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
    Vector3d acc_v, acc_fixed, pos_next;
    Quaterniond q_q;

    acc_v = this->_toVector3d(acc);
    q_q = this->_toQuaterniond(q);
    acc_fixed = this->_rotateAcc(acc_v, q_q);

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
