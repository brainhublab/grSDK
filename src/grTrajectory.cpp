#include <iostream>
#include "grTrajectory.h"

#include <Eigen/LU>
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
    setupGravityMatrices();
}

GRTrajectory::~GRTrajectory()
{}
bool GRTrajectory::setupGravityMatrices()
{
    for(int i=0; i<4; i++)
    {
        desiredMatrix(3, i) = 1;
        realMatrix(3, i) = 1;
    }
    desiredMatrix(2, 0) = G;
    desiredMatrix(2, 1) = -G;
    desiredMatrix(0, 2) = G;
    desiredMatrix(1, 3) = G;
    
    

   // std::cout<<desiredMatrix<<std::endl;
}

bool GRTrajectory::calculateCorrectionMatrix()
{
    //std::cout << "Inversing matrixx \n " << realMatrix;
//    PartialPivLU<Matrix4d> lu(realMatrix);

    realMatrix = (realMatrix.inverse()).eval();
    // realMatrix.inverse();
    // lu.inverse();
    // realMatrix = lu.matrixLU();

    //std::cout << "\n inversed!!!;  \n " << realMatrix << std::endl;

    correctionMatrix = desiredMatrix * realMatrix; 
  

   // std::cout << std::endl << "real: " << realMatrix << "\n desiredMatrix: " << desiredMatrix <<
    //   "\n correctionMatrix: " << correctionMatrix << std::endl; 
}

Vector3d GRTrajectory::_getNewPosByVelocity(Vector3d acc, unsigned long timestamp)
{
    Vector3d acc_ms, velocity, distance, pos_next;
    double dt = (timestamp - this->timestamp_last) / 1000.f;

    // acc_ms = this->_getAccValue(acc);

    velocity = this->velocity_last + acc * dt;
    distance = velocity * dt;

    pos_next = this->pos_last + distance;
    this->velocity_last = velocity;
    return pos_next;
}
Vector3d GRTrajectory::_convertAccToG(Vector3d in)
{
    Vector3d out;
    out = ((in * ACC_MULT));// / 1000);// * G;
   // std::cout<<"in acc vector"<<in<<" out acc to G "<<out<<std::endl;
    return out;
}

Vector3d GRTrajectory::_rotateAcc(Vector3d acc, Quaterniond q)
{
    /*Vector3d u(q.x(), q.y(), q.z());
    float s = q.w();

    Vector3d result = 2.0f * u.dot(acc) * u + (s*s - u.dot(u)) * acc + 2.0f *s * u.cross(acc);

    return result;
    */
    
    Quaterniond acc_q, inversed_q;
//   q.normalize();
   q = q.conjugate(); 
   /*
    q.normalize();
    q = q.conjugate();
*/
    inversed_q = q;
    /*inversed_q.normalize();
    inversed_q = inversed_q.conjugate(); 
  */  
 //  inversed_q.normalize(); 
   inversed_q = inversed_q.conjugate();
    
    acc_q.w() = 0;
    acc_q.vec() = acc;
    // std::cout<<"acc_q "<<acc_q.w()<<acc_q.vec()<<"===="<<acc<<std::endl;
    
    Quaterniond out_q = (q * acc_q) * inversed_q;
    Vector3d out = out_q.vec();
    // std::cout<<"out quanternion ----------> "<<out_q.vec()<<std::endl;
    // std::cout<<"out vector ------->>>>>"<<out<<std::endl;
    return out;

    
}

bool GRTrajectory::calibrateGravityMatrix(std::vector<double> acc, int j)
{   

    Vector3d acc_e = _toVector3d(acc);
    acc_e = _convertAccToG(acc_e);
    acc_e = acc_e * G;
    //std::cout << "Adding " << acc_e << " for " << j << " column of \n" << realMatrix << std::endl;
    for(int i = 0; i < 3; i++)
    {
        realMatrix(i, j) = acc_e(i);
    }

    //std::cout << "Now realMatrix is = \n" << realMatrix << std::endl;

}
Vector3d GRTrajectory::_getNewPosByIntegrating(Vector3d acc, unsigned long timestamp, Quaterniond q)
{
    Vector4d acc4d;
    Vector3d velocity, pos_next, acc_tmp;
    double dt = (timestamp - this->timestamp_last) / 1000.f;
    acc_tmp = acc;
    acc = acc - acc_last;
    //std::cout << "Hello, this is acc-acc_last BEFORE convert: " << std::endl << acc << std::endl;
    acc = this->_convertAccToG(acc);

    //std::cout << "Hello, this is acc-acc_last after convert: " << std::endl << acc << std::endl;

    //std::cout<<"before rotation--->>"<<acc<<std::endl;
   // acc = _rotateAcc(acc, q); //rotation 
  //  std::cout<<"after rotation--->>"<<acc<<std::endl;

    acc = acc * G; //converting from G units to M/s^2
 ///   std::cout<<"after rotation and *G (m/s/s)--->>"<<acc<<std::endl;
/*
   // acc = correctionMatrix * acc;
    for(int i =0; i< 3; i++)
    {

        acc4d(i) = acc(i);
    }
    acc4d(3) = 1;

    acc4d = correctionMatrix * acc4d ;

    std::cout<<"aloha this is 4d vector:  "<<acc4d<<std::endl;

 
    //cout << "delta: " << dt << ";temp acc: " << acc_ms << endl;
 
    for(int i=0; i<3; i++)
   {
        acc(i) = acc4d(i);
   }
*/

    // acc(2) = acc(2) - G;
    //std::cout<<"after rotation and --->>"<<acc<<std::endl;
    
    
    velocity = this->velocity_last + acc* dt;
    pos_next = this->pos_last + (velocity * dt + 0.5 * acc * (dt * dt));
   //pos_next = this->pos_last + (velocity_last * dt);

    this->velocity_last = velocity;//this->velocity_last + acc_ms * dt;
     acc_last = acc_tmp;
    this->pos_last = pos_next;
    this->timestamp_last = timestamp;
    

    return pos_next;
}

Vector3d GRTrajectory::_getNewPosByVeliko(Vector3d acc, unsigned long timestamp)
{
    Vector3d pos_next, velocity_new, acc_ms;
    double dt = (timestamp - this->timestamp_last) / 1000.f;

    //acc_ms = this->_getAccValue(acc); TODO

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

Vector3d GRTrajectory::gravity_compensate(vector<double> q, vector<double> acc)
{
    std::vector<double> g;
    g.push_back(0);
    g.push_back(0);
    g.push_back(0);


    // return _toVector3d(result);
}

vector<double> GRTrajectory::getNewPosByVelocity(vector<double> acc, vector<double> q, unsigned long timestamp)
{
    Vector3d acc_v, acc_fixed, pos_next;
    Quaterniond q_q;
    //
    // remove gravity
    //
    // a - here call gravity_compenstate

    // b
    // acc_v = this->_toVector3d(acc);

    //q_q = this->_toQuaterniond(q);
    //acc_fixed = this->_rotateAcc(acc_v, q_q);
    acc_fixed = gravity_compensate(q, acc);
    //  b - here, after rotating with quant-n just acc_fixed = acc_fixed - [0, 0, +-9.8]

    // OR c - apply low-pass algorithm 
    // https://developer.android.com/reference/android/hardware/SensorEvent.html#values
    // float alpha = 0.8;
    // gravity = [0 0 9.8];
    // gravity = alpha * gravity + (1-alpha) * acc_v;
    // result_acc = acc_v - gravity;


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
        this->acc_last = _toVector3d(acc);
        pos_last *= 0;
        return this->_toStdVector(this->pos_last);

    }
    Vector3d acc_v;
    Quaterniond q_q;

    acc_v = this->_toVector3d(acc);
    q_q = this->_toQuaterniond(q);

    //std::cout << "Position last: " << pos_last << std::endl;

    return  this->_toStdVector(_getNewPosByIntegrating(acc_v, timestamp, q_q));
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
    integrated_pos = this->_getNewPosByIntegrating(acc_fixed, timestamp, q_q);

    pos_next = (speed_pos + integrated_pos) / 2;

    this->pos_last = pos_next;
    this->timestamp_last = timestamp;

    return this->_toStdVector(pos_next);
}
