#include "grTrajectory.h"

GRTrajectory::GRTrajectory() //constructor
{
    this->first_call = true;
    this->timestamp_last = 0;
    this->pos_last(0) = 0;
    this->pos_last(1) = 0;
    this->pos_last(2) = 0;

    this->velocity_last(0) = 0;
    this->velocity_last(1) = 0;
    this->velocity_last(2) = 0;
    _setupGravityMatrices();
}
/*destructor
 * */
GRTrajectory::~GRTrajectory()
{}

/*Setup matrix needed for callibration of acceleromater
 * it' booll method which sets default values on matrices for each axis
 */
bool GRTrajectory::setupGravityMatrices() // if u want to see your matrix paste flag TODO need to be implement
{
    for(int i=0; i<4; i++)
    {
       this->_desiredMatrix(3, i) = 1;
       this->_realMatrix(3, i) = 1;
    }
    _desiredMatrix(2, 0) = G;
    _desiredMatrix(2, 1) = -G;
    _desiredMatrix(0, 2) = G;
    _desiredMatrix(1, 3) = G;
    
    return 1;

   // std::cout<<desiredMatrix<<std::endl;
}

/*Calculation of correction matrix by multiplying woth inversed real data matrix
 */
bool GRTrajectory::calculateCorrectionMatrix()
{
    //std::cout << "Inversing matrixx \n " << realMatrix;

    _realMatrix = (_realMatrix.inverse()).eval();
    _correctionMatrix = _desiredMatrix * _realMatrix; 
  
    return 1;
}

/*Private mathod which calculate trajectory in two steps 
 * It's take two arguments "acclerometer raw data " and "timestamp"
 * 1) Current velocity
 * 2) position by velocity & time
 */
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

/* Private method which convert's raw accelerometer data units to G
 * mg/LSB -> G
 * It's take one argument Eigen::Vector3d with raw accelerometer data
 * and return Eigen::Vector3d with accelerometer data in G units
 */
Vector3d GRTrajectory::_convertAccToG(Vector3d in) //TODO need to be unified 
{
    Vector3d out;
    out = ((in * ACC_MULT));// / 1000);// * G;
   // std::cout<<"in acc vector"<<in<<" out acc to G "<<out<<std::endl;
    return out;
}

/* Private method for rotation accelerometer vector by quaternion calculated in
 * Madgwick algorith for the same data frame
 * This is needed for removing of gravity from calculations
 * It's take two arguments EIgen::Vector3d woth raw accelerometer data and
 * Eigen::Quaterniond with quaternion and it's return rotated Eigen::Vector3d vector
 */ 
Vector3d GRTrajectory::_rotateAcc(Vector3d acc, Quaterniond q)
{
    
    Quaterniond acc_q, inversed_q;
   q = q.conjugate(); 
   inversed_q = q;
   inversed_q = inversed_q.conjugate();
    
    acc_q.w() = 0;
    acc_q.vec() = acc;
    
    Quaterniond out_q = (q * acc_q) * inversed_q;
    Vector3d out = out_q.vec();
    // std::cout<<"out quanternion ----------> "<<out_q.vec()<<std::endl;
    // std::cout<<"out vector ------->>>>>"<<out<<std::endl;
    return out;
}

/* Method which callibrate matrix accelerometer data with callibration matrix
 * Takes two arguments accelerometer raw data vector in different positions and
 * iterator for each position. Maximum of iteration are 4.
 * Need to be call in loop with increase iterator in each iteration and position
 * Positions are Z, -Z, X, Y;
 */
bool GRTrajectory::calibrateGravityMatrix(std::vector<double> acc, int j)
{   

    Vector3d acc_e = _toVector3d(acc);
    acc_e = _convertAccToG(acc_e);
    acc_e = acc_e * G;
    for(int i = 0; i < 3; i++)
    {
        realMatrix(i, j) = acc_e(i);
    }
    //std::cout << "Now realMatrix is = \n" << realMatrix << std::endl;
    return 1;
}

/* Private method which takes two arguments and calculate new position by double integration 
 * parameters are Eigen::Vector3d with raw accelerometer data, timestamp, and
 * Eigen::Quaterniond calculated in Madgwick for current data frame
 * return Eigen::Vector3d with new coordinates
 */
Vector3d GRTrajectory::_getNewPosByIntegrating(Vector3d acc, unsigned long timestamp, Quaterniond q)
{
    Vector4d acc4d;
    Vector3d velocity, pos_next, acc_tmp;
    double dt = (timestamp - this->timestamp_last) / 1000.f;
    acc_tmp = acc;
    acc = acc - acc_last;
    acc = this->_convertAccToG(acc);

   // acc = _rotateAcc(acc, q); //rotation 

    acc = acc * G; //converting from G units to M/s^2
    // acc(2) = acc(2) - G;
    
    velocity = this->velocity_last + acc* dt;
    pos_next = this->pos_last + (velocity * dt + 0.5 * acc * (dt * dt));

    this->velocity_last = velocity;//this->velocity_last + acc_ms * dt;
    acc_last = acc_tmp;
    this->pos_last = pos_next;
    this->timestamp_last = timestamp;
    

    return pos_next;
}

/* Private method for converting of Eigen::Vector3d to sdt::vector
 */
vector<double> GRTrajectory::_toStdVector(Vector3d in)
{
    vector<double> out = {in(0), in(1), in(2)};
    return out;
}

/*Private method for converting of std::vector to Eigen::Vector3d
 */
Vector3d GRTrajectory::_toVector3d(vector<double> in)
{
    Vector3d out(in[0], in[1], in[2]);
    return out;
}

/*Private method for converting std::vector with Madgwick quaternion to 
 * Eigen::Quaterniond
 */
Quaterniond GRTrajectory::_toQuaterniond(vector<double> in)
{
    Quaterniond out(in[0], in[1], in[2], in[3]);
    return out;
}

/*Private method for gravity compensation it's take two arguments quaternion of rotation and
 * raw accelerometer data
 */
Vector3d GRTrajectory::_gravity_compensate(vector<double> q, vector<double> acc)
{
    std::vector<double> g;
    g.push_back(0);
    g.push_back(0);
    g.push_back(0);


    // return _toVector3d(result);
}


/* Public method return new position by two step calculation of new positio
 * 1) calculation of velocity
 * 2) calculation of position
 * Takes three arguments accelerometer raw data quaternion of ratation and timestamp
 */
vector<double> GRTrajectory::getNewPosByVelocity(vector<double> acc, vector<double> q, unsigned long timestamp)
{
    Vector3d acc_v, acc_fixed, pos_next;
    Quaterniond q_q;
    acc_fixed = gravity_compensate(q, acc);

    pos_next = this->_getNewPosByVelocity(acc_fixed, timestamp);
    this->pos_last = pos_next;
    this->timestamp_last = timestamp;

    return this->_toStdVector(pos_next);
}

/*Public method for calculating new position by double integrating
 * takes three arguments vector with raw accelerometer data quaternion and timestamp
 */
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

/*Public method which returns position with using both metods for getting trajectory
 */
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
