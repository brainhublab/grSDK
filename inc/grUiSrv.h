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
#define DEFAULT_PATH "/tmp/grsock"

enum DataType { NOTYPE, RAW_PALM, RAW_THUMB, RAW_INDEX, RAW_MIDDLE, RAW_RING, RAW_PINKY};
enum Command { NOCMD, CMD1, STREAM_DATA };


class GRUiSrv
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


};
#endif
