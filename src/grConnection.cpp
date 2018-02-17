#include <sys/socket.h>

#include "grConnection.h"

GRConnection::GRConnection()
{
}

GRConnection::GRConnection(GRDevice devInfo)
{
    this->_dev = devInfo;
    this->_asignDeviceWithSocket();

    this->_start = std::chrono::high_resolution_clock::now();
    firstIteration = true;
}


/*Select device by ID and fill message with one peace of data
 * Need to be use in loop
 */
bool GRConnection::getData(GRMessage* message)
{
    int sock, status, bytes_read;
    int id;
    char buf[1] = {0};

    std::string rawMessage;

    sock = this->_deviceSocket.sock; //= socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    bool messageAvalible = false;

    std::string readyMessage;

    int iter = 0;
    while(iter<6)
    {

       // std::cout << "started reading" << std::endl;
        bytes_read = read(sock, buf, sizeof(buf));
       // std::cout << "finished reading" << std::endl;
        if(bytes_read >0)
        {
            if(buf[0] != '\n')
            {
                rawMessage += buf[0];
            }
            else
            {
                messageAvalible = true;
                // std::cout<<"message "<<rawMessage<<std::endl;
                iter++;
                if(firstIteration)
                {
                    this->_checkConnectedImus(rawMessage, message);

                }
                else
                {
                    _asignMessageWithImu(rawMessage, message);
                }
                //std::cout<<rawMessage<<" message from connection"<<std::endl;
                rawMessage.clear();
            }
        }
    }
    if(firstIteration)
    {
        firstIteration = false;
    }

    return true;
}

/*Split raw device data in peaces and push them into imu structure vars
*/
bool GRConnection::_splitData(std::string data, GRImu* sensor)
{
    sensor->gyro.clear();
    sensor->acc.clear();
    sensor->mag.clear();
    int i = 0;
    int id;
    double n, arr[11];
    std::stringstream ss(data);
    // std::vector<double> gyro, acc, mag;

    //GRMessage msg;
    while(ss >> n && i<11)
    {
        arr[i] = n;
        i++;
    }


    for(int i=1;i<11;i++)
    {
        if(i <= 3)
        {
            sensor->gyro.push_back(arr[i]);
        }
        else if(i > 3 && i <= 6)
        {
            sensor->acc.push_back(arr[i]);
        }
        else if(i > 6 && i<=9)
        {
            sensor->mag.push_back(arr[i]);
        }
        else if(i > 9)
        {
            sensor->time_stamp = arr[i];
        }

    }
    //std::cout<<"from split data "<<sensor.gyro[0]<<std::endl;
    return 1;
}

/* Return local time stamp which starts with starting of program
*/
double GRConnection::_getTimeStamp()
{
    this->_timeStamp = 0.0;

    this->_end = std::chrono::high_resolution_clock::now();
    _timeStamp = std::chrono::duration_cast<std::chrono::milliseconds>
        (_end-_start).count();

    _timeStamp = (_timeStamp * 0.001f);

    return _timeStamp;
}

/* Asigning device with socke and redurn socket
*/
int GRConnection::_asignDeviceWithSocket()
{

    struct sockaddr_rc addr = { 0 };

    dev_socket deviceSocket;


    deviceSocket.sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1; //TODO maeby need to be changet with device id

    std::cout<<"Asigning device with addres: "<<this->_dev.address<<std::endl;

    str2ba(this->_dev.address.c_str(), &addr.rc_bdaddr);

    deviceSocket.addr = addr;
    this->_deviceSocket = deviceSocket;

    //connectSocket(devId);//TODO need to implement in another method

    std::cout<<"device with addres: "<<this->_dev.address<<"with socket"<<deviceSocket.sock<<std::endl;

    return deviceSocket.sock;
}

/* Assigning raw message with concret imu mofule by finger index and split data for it
 */
bool GRConnection::_asignMessageWithImu(std::string rawMessage, GRMessage* message)
{
    int id;
    bool messageAvalible = false;
    std::stringstream ss(rawMessage);

    ss >> id;
    if(id == 0)
    {
        _splitData(rawMessage, &message->pinky);
        //message->pinky().time_stamp = getTimeStamp;//TODO add if statement
    }
    else if(id ==1)
    {
        _splitData(rawMessage, &message->ring);
        //message->ring().time_stamp = getTimeStamp;
    }
    else if(id == 2)
    {
        _splitData(rawMessage, &message->middle);
        //message->middle().time_stamp = getTimeStamp;
    }
    else if(id == 3)
    {
        _splitData(rawMessage, &message->index);
        //message->index().time_stamp = getTimeStamp;
    }
    else if(id==4)
    {
        _splitData(rawMessage, &message->thumb);
        //message->thumb().time_stamp = getTimeStamp;
    }
    else if(id==5)
    {
        // std::cout<<rawMessage<<std::endl;
        _splitData(rawMessage, &message->palm);
        //message->palm().time_stamp = getTimeStamp;

    }
    //rawMessage.clear();
    //TODO need to comment
    if(!message->palm.gyro.empty())
    {
        //std::cout<<device->palm.data.front().gyro[1]<<std::endl;
        //device->palm.data.pop_front();
    }

}

/* Iterate raw message and check which imu modules are connectd and write boolean flag of each imu in message
 */
bool GRConnection::_checkConnectedImus(std::string rawMessage, GRMessage* msg)
{

    int id;
    bool messageAvalible = false;
    std::stringstream ss(rawMessage);

    ss >> id;
    if(id == 0)
    {
        msg->pinky.is_connected = true;
    }
    else if(id ==1)
    {
        msg->ring.is_connected = true;
    }
    else if(id == 2)
    {
        msg->middle.is_connected = true;
    }
    else if(id == 3)
    {
        msg->index.is_connected = true;
    }
    else if(id==4)
    {
        msg->thumb.is_connected = true;
    }
    else if(id==5)
    {
        msg->palm.is_connected = true;
    }
    return true;
}

/*Make connection to socket of device
*/
bool GRConnection::connectSocket()
{
    int status = 0;
    int attempts = 3;

    while(attempts != 0)
    {
        status = connect(this->_deviceSocket.sock,
                (struct sockaddr *)(this->_deviceSocket.getAddrRef()), sizeof(this->_deviceSocket.addr));

        if(status == -1)
        {
            attempts--;
            std::cout<<"Oh dear, something went wrong with connect! "<< strerror(errno)<<std::endl;
            std::cout<<"Trying to make new attempt for connection"<<std::endl;
        }
        else if(status == 0)
        {
            attempts *= 0;

        std::cout<<"Status of connection of socket: "<<status<<std::endl;
        }
    }
    return true;

}


#include <nbind/nbind.h>
NBIND_CLASS(GRConnection) {
    construct<>();
    construct<GRDevice>();

    method(getData);
    method(connectSocket);
}
