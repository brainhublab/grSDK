

#include "grDevice.h"

GRDevice::GRDevice()
{
    this->_defaultPort.resize(2);
    this->_defaultPort = "23";
}
GRDevice::GRDevice(std::string n, std::string hw, std::string h, int id)
{
    this->_defaultPort.resize(2);
    this->_defaultPort = "23";
    this->_name = n;
    this->_hwAddr = hw;
    this->_host = h;

    this->_deviceCmd["callibrate"] = "calib\r\n";
    this->_deviceCmd["suspend"] = "susp\r\n";

}
/*GRDevice::GRDevice(const GRDevice& cDevice)
  {

  }
  GRDevice& GRDevice::operator=(const GRDevice& neDev)
  {

  }

  GRDevice::~GRDevice()
  {

  }
  */
int GRDevice::getBatteryLevel()
{
    return this->_batteryLevel;
}
std::string GRDevice::getHwAddr() const
{
    return this->_hwAddr;
}

std::string GRDevice::getName() const
{
    return this->_name;
}

void GRDevice::setHwAddr(std::string newHwAddr)
{
    this->_hwAddr = newHwAddr;
}

void GRDevice::setName(std::string newName)
{
    this->_name = newName;
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
void GRDevice::subscribe(GRMessage* msg, std::function<void(GRMessage* msg)> cb)
{
    std::thread gd(&GRDevice::_getData, this, msg, cb );
    gd.detach();
}
void GRDevice::_getData(GRMessage* msg, std::function<void(GRMessage* cmsg)> cb) 
{
    std::string raw;
    std::string cumulative;
    std::string buf;
    raw.resize(1);

    // raw.resize(120);
    libsocket::inet_stream deviceSocket(this->_host, _defaultPort, LIBSOCKET_IPv4);
    //std::cout<<this->_host<<_hwAddr<<"---"<<_defaultPort<<std::endl;
    //  this->deviceSocket.connect(this->_host.c_str(), _defaultPort.c_str(), LIBSOCKET_IPv4);
    deviceSocket<<"gd\r\n";
    int count = 0;
    while(raw.size()>0)
    {

        //raw.clear();
        deviceSocket>>raw;
        if(buf.size()== 2)
        {
            buf.at(0) = buf.at(1);
            buf.at(1) = raw.at(0);

        }
        else
        {

            buf.push_back(raw.at(0));
        }
        if(buf != "\r\n")
        {
            cumulative.push_back(raw.at(0));
        }
        else
        {
            //        std::cout<<cumulative<<std::endl;
            //        std::cout<<cumulative.size()<<"-------------------------------"<<std::endl;
            if(cumulative.size() == 116)
            {
                // msg->imuByteData = cumulative;
                this->_deserialize(_splitMessage(&cumulative), msg);
                msg->batteryLevel = this->_batteryLevel;
            } 
//            msg->print();
            cb(msg);
            //std::cout<<std::endl;
            msg->clear();
            cumulative.clear();
        }
        if(pendingCmd)
        {
            for(int i=0; i<this->_cmdQueue.size(); i++)
            {

                deviceSocket<<this->_cmdQueue.front();
                this->_cmdQueue.pop();
            }
            pendingCmd = false;
        } 
    }
    deviceSocket<<"sd";
    deviceSocket.shutdown();

}

bool GRDevice::_sendCmd(std::string cmd)
{

    this->_cmdQueue.push(this->_deviceCmd[cmd]);//"calib\r\n";//_deviceCmd[cmd];
    pendingCmd = true;
}

std::vector<std::string> GRDevice::_splitBySlash(std::string* inp)
{
    //TODO need to fix vector size its put the terminating zero as an element of vector
    std::regex reg("\\ /");
    return std::vector<std::string>{
        std::regex_token_iterator<std::string::iterator>(inp->begin(),
                inp->end(),
                reg,
                -1),{}
    };
}
std::vector<std::string> GRDevice::_splitMessage(std::string* inp)
{
    std::vector<std::string> result;
    int s=0;
    int e=19;
    inp->pop_back();
    this->_batteryLevel = static_cast<uint16_t>(inp->at(114));
//    std::cout<<_batteryLevel<<" battery "<<std::endl;
    inp->pop_back();
    //  std::cout<<"----------------------------------------------"<<inp->size()<<std::endl;
    for(int i=0; i<6; i++)
    {
        result.push_back(inp->substr(s, 19));
        s+=19;
    }
    return result;
}
void GRDevice::_deserialize(std::vector<std::string> inp, GRMessage* msg)
{
    std::string imuData;
    //std::cout<<inp.size()<<"<-----------------------vector size"<<std::endl;
    /* 
       for(int i=0; i< inp.size();i++)
       {
       std::cout<<inp[i].at(2)<<std::endl;
       }
       */  
    //std::cout<<"INPSIZE   "<<inp.size();
    //    std::cout<<"-------------------EOF VEC"<<std::endl;


    for(int i=0; i< inp.size(); i++)
    {
        if(i == 0)
        {
            if((msg->pinky.isConnected = (bool)inp[i].front()))
            {
                this->_extractImuDataFromString(&inp[i], &msg->pinky);
                inp[i].erase(0, 1);
                /* for(int j=1; j<18; j+=2)
                   {
                // if(j!=17)

                inp[i].insert(j, ",");
                }*/
                msg->pinky.imuByteData = inp[i];

            }
            else
            {
                msg->pinky.isConnected = false;
            }
        }
        else if(i ==1)
        {
            if((msg->ring.isConnected = (bool)inp[i].front()))
            {
                this->_extractImuDataFromString(&inp[i], &msg->ring);
                inp[i].erase(0, 1);
                msg->ring.imuByteData = inp[i];
            }
            else
            {
                msg->ring.isConnected = false;

            }
        }
        else if(i == 2)
        {
            if((msg->middle.isConnected = (bool)inp[i].front()))
            {
                this->_extractImuDataFromString(&inp[i], &msg->middle);
                inp[i].erase(0, 1);
                msg->middle.imuByteData = inp[i];
            }
            else
            {
                msg->middle.isConnected = false;
            }

        }
        else if(i == 3)
        {
            if((msg->index.isConnected = (bool)inp[i].front()))
            {
                this->_extractImuDataFromString(&inp[i], &msg->index);
                inp[i].erase(0, 1);
                msg->index.imuByteData = inp[i];
            }
            else
            {
                msg->index.isConnected = false;
            }

        }
        else if(i == 4)
        {
            if((msg->thumb.isConnected =(bool)inp[i].front()))
            {
                this->_extractImuDataFromString(&inp[i], &msg->thumb);
                inp[i].erase(0, 1);
                msg->thumb.imuByteData = inp[i];
            }
            else
            {
                msg->thumb.isConnected = false;
            }

        }
        else if(i == 5)
        {
            if((msg->palm.isConnected = (bool)inp[i].front()))
            {
                this->_extractImuDataFromString(&inp[i], &msg->palm);
                inp[i].erase(0, 1);
                msg->palm.imuByteData = inp[i];
            }
            else
            {
                msg->palm.isConnected = false;
            }

        }
    }
}

void GRDevice::_extractImuDataFromString(std::string* imuData, GRImu* imu )
{

    // std::cout<<"IMUDATASIZE"<<imuData->size()<<std::endl;
    int j=0;
    for(int i = 1; i<imuData->size() -1; i+=2)
    {
        if(j==3)
        {
            j=0;
        }
        if(i<6)
        {
            imu->acc[j] = this->_convertBytes(imuData->at(i), imuData->at(i+1));
            j++;
            //   std::cout<<"inExtraction:  "<<*imuData<<convertBytes(imuData->at(i), imuData->at(i+1))<<std::endl;
        }else if(i<12 && i>6)
        {
            imu->gyro[j] = this->_convertBytes(imuData->at(i), imuData->at(i+1));
            j++;
        }
        else if(i>12 && i<19)//TODO have something on the end 
        {
            imu->mag[j] = this->_convertBytes(imuData->at(i), imuData->at(i+1));
            j++;
        }
        else 
        {
            std::cout<<(int)imuData->at(i)<<"<<<<<<---------------"<<std::endl;
        }
    }
}

int16_t GRDevice::_convertBytes(char hb, char lb)
{
    return  (int16_t)((hb & 0xFF) << 8 | (lb & 0xFF)  );
}
/* Return local time stamp which starts with starting of program
*/
double GRDevice::_getTimeStamp()
{
    return this->_timeStamp;
}



/* Iterate raw message and check which imu modules are connectd and write boolean flag of each imu in message
*/
bool GRDevice::_checkConnectedImus(std::string rawMessage, GRMessage* msg)
{
}

