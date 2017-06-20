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

Vector3d GRTrajectory::_getNewPosByVelocity(Vector3d acc, int timestamp)
{
    Vector3d velocity, distance, pos_next;
    int time_elapsed = timestamp - this->timestamp_last;

    velocity = this->velocity_last + ((acc * G) * time_elapsed);
    distance = velocity * time_elapsed;

    pos_next = this->pos_last + distance;

    return pos_next;
}

Vector3d GRTrajectory::_getNewPosByIntegrating(Vector3d acc, int timestamp)
{
    Vector3d pos_next;
    int time_elapsed = timestamp - this->timestamp_last;

    pos_next = this->pos_last + this->velocity_last * time_elapsed + 0.5 * (acc * G) * (time_elapsed * time_elapsed);

    return pos_next;
}

vector<double> GRTrajectory::_toStdVector(Vector3d in)
{
    vector<double> out = {in(0), in(1), in(2)};
    return out;
}

Vector3d GRTrajectory::_toVector3d(vector<double> in)
{
    Vector3d out = {in[0], in[1], in[2]};
    return out;
}

vector<double> GRTrajectory::getNewPosByVelocity(vector<double> acc, int timestamp)
{
    Vector3d acc_v, pos_next;

    acc_v = this->_toVector3d(acc);

    pos_next = this->_getNewPosByVelocity(acc_v, timestamp);
    this->pos_last = pos_next;
    this->timestamp_last = timestamp;

    return this->_toStdVector(pos_next);
}

vector<double> GRTrajectory::getNewPosByIntegrating(vector<double> acc, int timestamp)
{
    Vector3d acc_v, pos_next;

    acc_v = this->_toVector3d(acc);

    pos_next = this->_getNewPosByIntegrating(acc_v, timestamp);
    this->pos_last = pos_next;
    this->timestamp_last = timestamp;

    return this->_toStdVector(pos_next);
}

vector<double> GRTrajectory::getNewPos(vector<double> acc, int timestamp)
{
    Vector3d acc_v, speed_pos, integrated_pos, pos_next;

    acc_v = this->_toVector3d(acc);

    speed_pos = this->_getNewPosByVelocity(acc_v, timestamp);
    integrated_pos = this->_getNewPosByIntegrating(acc_v, timestamp);

    pos_next = (speed_pos + integrated_pos) / 2;

    this->pos_last = pos_next;
    this->timestamp_last = timestamp;

    return this->_toStdVector(pos_next);
}
