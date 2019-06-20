#ifndef GR_UI_SRV
#define GR_UI_SRV


#include <iostream>
#include <string.h>
#include <memory>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//socket headers
#include <exception.hpp>
#include <unixclientstream.hpp>
#include <unixserverstream.hpp>
#include <libunixsocket.h>
#include <grDataStructs.h>
#include <regex>
#include <array>
#include <unordered_map>
#include <thread>

#include "Eigen/Dense"
#include "Eigen/Geometry"

#include "grAlgorithm.h"
#include "grDataStructs.h"
#define DEFAULT_PATH "/tmp/grsock"

enum class DataType {BYTE, DIGIT, STRING};
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
        std::unique_ptr<libsocket::unix_stream_client> cli;
        std::vector<std::string> _splitBySpace(std::string* );
        std::unordered_map<std::string, std::unique_ptr<libsocket::unix_stream_client>> _clients;
        std::unordered_map<std::string, uint8_t> _dataType;
        GRAlgMessage _algMsg;

        std::string eulerRotationsToStr(std::unordered_map<std::string, std::vector<double>>);
        std::string quaternionToStr(std::unordered_map<std::string, Eigen::Quaterniond>);
        std::string vectorDoubleToStr(std::vector<double>);

        std::unordered_map<std::string, 
            std::unordered_map<std::string, 
            std::unique_ptr<libsocket::unix_stream_client>>> _avalibeClients;


};
#endif
