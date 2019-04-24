

#include "grDevice.h"

GRDevice::GRDevice()
{
}
GRDevice::GRDevice(std::string host)
{
//    this->_deviceSocket = std::move(libsocket::inet_stream(host, "23", LIBSOCKET_IPv4));
}

/*Select device by ID and fill message with one peace of data
 * Need to be use in loop
 */
bool GRDevice::getData(GRMessage* message)
{


   return true;
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

/*Make connection to socket of device
*/
bool GRDevice::connectSocket()
{
   }
