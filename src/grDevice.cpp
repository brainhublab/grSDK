

#include "grDevice.h"

GRDevice::GRDevice()
{
}
GRDevice::GRDevice(std::string n, std::string hw, std::string h, int id)
{
    this->name = n;
    this->hwAddr = hw;
    this->_host = h;
    /*    try{
          std::cout<<"IN CONSTRUCTOR"<<std::endl;
          _deviceSocket.connect(h, "23", LIBSOCKET_IPv4);


          }catch(const libsocket::socket_exception& exc)
          {
          std::cerr<<exc.mesg<<std::endl;
          }
          */
}
/*
   GRDevice::GRDevice(GRDevice &&dev): _deviceSocket(std::move(dev._deviceSocket))
   {
   this->name = dev.name;
   this->hwAddr =  dev.hwAddr;
   this->_host = dev._host;
   }

   GRDevice& GRDevice::operator=(GRDevice&& dev)

   {
   if(this != &dev)
   {
   this->name = dev.name;
   this->hwAddr = dev.hwAddr;
   this->_host = dev._host;
//        _deviceSocket = (std::move(dev._deviceSocket));

}
return *this;
}
*/
/*Select device by ID and fill message with one peace of data
 * Need to be use in loop
 */
void GRDevice::getData(GRMessage* msg) 
{
    std::string asd;
    std::string cumulative;
    asd.resize(1);
    //asd.resize(121);
    libsocket::inet_stream _deviceSocket(this->_host, "23", LIBSOCKET_IPv4);
    _deviceSocket<<"gd\n";
    int i=0;
    int j=0;
    char* c;

int count = 0;
    //_deviceSocket>>asd;
    while(asd.size()>0)
    {


        _deviceSocket>>asd;
        if(asd == "\n" && cumulative.size() >= 120)
        {
            //std::cout<<_splitBySlash(&cumulative)[0]<<std::endl;
            _deserialize(_splitBySlash(&cumulative), msg);
          //  std::cout<<"message: "<<msg->pinky.acc[1]<<std::endl;//" "<<msg->palm.acc[0]<<std::endl;
            msg->print();
            std::cout<<"----------------------------------------------| frame "<<count<<std::endl;
            count++;
            cumulative.clear();
        }
        //std::cout<<asd;
        if((asd != "\0") && (!asd.empty()) && (asd !="\n"))
        {
            cumulative += asd;//.at(0);
          // cumulative.push_back(asd.c_str()[0]);
        }

    }
    _deviceSocket<<"sd";
    _deviceSocket.shutdown();

}
std::vector<std::string> GRDevice::_splitBySlash(std::string* inp)
{
    //TODO need to fix vector size its put the terminating zero as an element of vector
    std::regex reg("\\/");
    return std::vector<std::string>{
        std::regex_token_iterator<std::string::iterator>(inp->begin(),
                inp->end(),
                reg,
                -1),{}
    };
}
void GRDevice::_deserialize(std::vector<std::string> inp, GRMessage* msg)
{
    std::string imuData;
    //    std::vector<std::string>& inpRef;

//    imuData = inp[0];
/*
    std::cout<<inp[0][0]<<"data"<<std::endl;
   // inp.pop_back(); //TODO need to fix REGEX in vector splitting
    std::cout<<inp.size()<<"<-----------------------vector size"<<std::endl;
    for(int i=0; i< inp.size();i++)
    {
        std::cout<<inp[i]<<std::endl;
    }
    std::cout<<"-------------------EOF VEC"<<std::endl;
*/
    for(int i=0; i< inp.size(); i++)
    {
        if(i == 0)
        {
            if((msg->pinky.isConnected = (int)inp[i][0]-'0'))
            {
                _extractImuDataFromString(&inp[i], &msg->pinky) ;

            }
        }else if(i ==1)
        {
            if((msg->ring.isConnected = (int)inp[i][0] -'0'))
            {
                _extractImuDataFromString(&inp[i], &msg->ring);
            }
        }else if(i == 2)
        {
            if((msg->middle.isConnected = (int)inp[i][0] -'0'))
            {
                _extractImuDataFromString(&inp[i], &msg->middle);
            }

        } else if(i == 3)
        {
            if((msg->index.isConnected = (int)inp[i][0] -'0'))
            {
                _extractImuDataFromString(&inp[i], &msg->index);
            }

        } else if(i == 4)
        {
            if((msg->thumb.isConnected = (int)inp[i][0] -'0'))
            {
                _extractImuDataFromString(&inp[i], &msg->thumb);
            }

        } else if(i == 5)
        {
            if((msg->palm.isConnected = (int)inp[i][0] -'0'))
            {
                _extractImuDataFromString(&inp[i], &msg->palm);
            }

        }
    }
}

void GRDevice::_extractImuDataFromString(std::string* imuData, GRImu* imu )
{

  //  std::cout<<"IMUDATASIZE"<<imuData->size()<<std::endl;
    int j=0;
    for(int i = 1; i<imuData->size(); i+=2)
    {
        if(j==3)
        {
            j=0;
        }
        if(i<7)
        {
            imu->acc[j] = convertBytes(imuData->at(i), imuData->at(i+1));
            j++;
         //   std::cout<<"inExtraction:  "<<*imuData<<convertBytes(imuData->at(i), imuData->at(i+1))<<std::endl;
        }else if(i<13 && i>6)
        {
            imu->gyro[j] = convertBytes(imuData->at(i), imuData->at(i+1));
            j++;
        }
        else if(i>12 && i<19)//TODO have something on the end 
        {
            imu->mag[j] = convertBytes(imuData->at(i), imuData->at(i+1));
            j++;
        }
    }
    // std::cout<<"bytes  "<<convertBytes(imuData->at(1), imuData->at(2))<<std::endl;
}

int16_t GRDevice::convertBytes(char hb, char lb)
{
    return  (int16_t)((hb & 0xFF) << 8 | (lb & 0xFF)  );
}
/*Split raw device data in peaces and push them into imu structure vars
*/
bool GRDevice::_splitData(std::string data, GRImu* sensor)
{
}

/* Return local time stamp which starts with starting of program
*/
double GRDevice::_getTimeStamp()
{
    return _timeStamp;
}

/* Asigning device with socke and redurn socket
*/
int GRDevice::_asignDeviceWithSocket()
{
}

/* Assigning raw message with concret imu mofule by finger index and split data for it
*/
bool GRDevice::_asignMessageWithImu(std::string rawMessage, GRMessage* message)
{
}

/* Iterate raw message and check which imu modules are connectd and write boolean flag of each imu in message
*/
bool GRDevice::_checkConnectedImus(std::string rawMessage, GRMessage* msg)
{

}


