#ifndef GR_UI_SRV
#define GR_UI_SRV


#include <iostream>
#include <string.h>
#include <memory>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <csignal>

//socket headers
#include <exception.hpp>
#include <unixclientstream.hpp>
#include <unixserverstream.hpp>
#include <libunixsocket.h>
#include <select.hpp>
#include <socket.hpp>

#include <grDataStructs.h>
#include <regex>
#include <array>
#include <unordered_map>
#include <thread>
#include <algorithm>

#include "Eigen/Dense"
#include "Eigen/Geometry"

#include "grAlgorithm.h"
#include "grDataStructs.h"
#define DEFAULT_PATH "/tmp/grsock"

//enum class DataType {BYTE, DIGIT, STRING};
//struct DataType{
//        std::vector<std::string>i 
//}
//
enum class DataType {BYTE, STRING};
enum class DataCmd { STREAM_DATA, STREAM_ROTATIONS, START_STREAM, PAUSE_STREAM, STOP_STREAM };
struct dataClient
{

    std::string dataCmd; 
    std::string imuId;
    std::string dataType;
    std::string UUID;
    std::unique_ptr<libsocket::unix_stream_client> streamClient;
};
//enum class 
class GRUiSrv: public GRAlgorithm
{
    public:
        GRUiSrv();
        GRUiSrv(const std::string);
        ~GRUiSrv();


        bool SetUp();
        bool startListening();
        bool pollConnections();
        bool run();
        bool _runInThread();

        void writeByteData(GRMessage*);
        void writeData(GRMessage*);
        //    private:
        char _ans[128];
        int _serverFd;
        int _clientFd;
        int _bytesRecieved;
        int _ret;
        std::string _defaultSockPath;
//        std::unique_ptr<libsocket::unix_stream_client> cli;
        std::vector<std::string> _splitBySpace(std::string* );
        std::unordered_map<std::string, std::unique_ptr<libsocket::unix_stream_client>> _clients;
        std::unordered_map<std::string, uint8_t> _imuNameToInt8;
        GRAlgMessage _algMsg;

        std::string eulerRotationsToStr(std::unordered_map<std::string, std::vector<double>>);
        std::string quaternionToStr(std::unordered_map<std::string, Eigen::Quaterniond>);
        std::string vectorDoubleToStr(std::vector<double>);

        //std::unordered_map<std::string*, dataClient> _dataClients;
        std::vector<dataClient> _dataClients;
        std::unordered_map<std::string, DataCmd> _dataCmd;
        std::unordered_map<std::string, DataType> _dataType;

        void _writeRawDataToCli(dataClient*, GRMessage*);
        void _writeRotationsDataToCli(dataClient*, GRMessage*);
        std::vector<std::string> _imuIds = {"pinky", "ring", "middle", "index", "thumb", "palm", "none"}; 
        std::string _generateUUID(std::unique_ptr<libsocket::unix_stream_client>::pointer);
        bool _checkIfClientExist(std::string*);
        bool _checkParams(std::vector<std::string>*);
        void _asignClient(dataClient*, std::vector<std::string>*, bool);

};
#endif
