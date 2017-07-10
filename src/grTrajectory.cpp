#include <iostream>
#include "grTrajectory.h"

GRTrajectory::GRTrajectory()
{
    this->first_call = true;
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
    Vector3d acc_ms, velocity, distance, pos_next;
    double dt = (timestamp - this->timestamp_last) / 1000.f;

    acc_ms = this->_getAccValue(acc);

    velocity = this->velocity_last + acc_ms * dt;
    distance = velocity * dt;

    pos_next = this->pos_last + distance;
    this->velocity_last = velocity;
    return pos_next;
}

Vector3d GRTrajectory::_getNewPosByIntegrating(Vector3d acc, unsigned long timestamp)
{
    Vector3d acc_ms, pos_next;
    double dt = (timestamp - this->timestamp_last) / 1000.f;

    acc_ms = this->_getAccValue(acc);

    cout << "temp acc: " << acc_ms << endl;

    pos_next = this->pos_last + this->velocity_last * dt + 0.5 * acc_ms * (dt * dt);

    this->velocity_last = this->velocity_last + acc_ms * dt;
    return pos_next;
}

Vector3d GRTrajectory::_getNewPosByVeliko(Vector3d acc, unsigned long timestamp)
{
    Vector3d pos_next, velocity_new, acc_ms;
    double dt = (timestamp - this->timestamp_last) / 1000.f;

    acc_ms = this->_getAccValue(acc);

    if(this->first_call)
    {
        this->first_call = false;

        pos_next = 0.5 * acc_ms * (dt * dt);
        velocity_new = (pos_next - this->pos_last) / dt;
        this->velocity_last = velocity_new;
    }

    pos_next = 0.5 * acc_ms * (dt * dt) + this->velocity_last * dt + this->pos_last;
    velocity_new = (pos_next - this->pos_last) / dt;

    this->pos_last = pos_next;
    this->velocity_last = velocity_new;
    this->timestamp_last = timestamp;

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

Vector3d GRTrajectory::_getAccValue(Vector3d in)
{
    Vector3d out;

    for(int i = 0; i < in.rows(); i++)
    {
        if(abs(in(i)) < FILTER_LOW)
        {
            in(i) = 0;
        }
    }

    out = ((in * ACC_MULT) / 1000) * G;

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

vector<double> GRTrajectory::getNewPosByVeloko(vector<double> acc, vector<double> q, unsigned long timestamp)
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

    pos_next = this->_getNewPosByVeliko(acc_fixed, timestamp);

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
