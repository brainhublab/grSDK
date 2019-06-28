#include "grUiSrv.h"

GRUiSrv::GRUiSrv()
{
    memset(this->_ans, 0, sizeof(this->_ans));
    this->_serverFd = 0;
    this->_clientFd = 0;
    this->_bytesRecieved = 0;
    this->_ret = 0;
    this->_defaultSockPath = DEFAULT_PATH;

    /*    this->_clients["uipinky"] = nullptr;
          this->_clients["uiring"] = nullptr;
          this->_clients["uimiddle"]= nullptr;
          this->_clients["uiindex"] = nullptr;
          this->_clients["uithumb"] = nullptr;
          this->_clients["uipalm"] = nullptr;
          this->_clients["uirotations"] = nullptr;

          this->_clients["iopinky"] = nullptr;
          this->_clients["ioring"] = nullptr;
          this->_clients["iomiddle"] = nullptr;
          this->_clients["ioindex"] = nullptr;
          this->_clients["iodatathumb"] = nullptr;
          this->_clients["iodatapalm"] = nullptr;
          this->_clients["iorotations"] = nullptr;

          this->_dataType["pinky"] = 0;// DataType::PINKY;
          this->_dataType["ring"] = 1;//DataType::RING;
          this->_dataType["middle"] = 2;//DataType::MIDDLE;
          this->_dataType["index"] = 3;//DataType::INDEX;
          this->_dataType["thumb"] = 4;//DataType::THUMB;
          this->_dataType["palm"] = 5;//DataType::PALM;

          this->_dataTypeParam.push_back("bytearray");
          this->_dataTypeParam.push_back("string");
          this->_dataTypeParam.push_back("digit");
          */
    this->_dataType[DataType::BYTE] = "byte";
    this->_dataType[DataType::DIGIT] = "digit";
    this->_dataType[DataType::STRING] = "string";

    this->_dataCmd[DataCmd::STREAM_DATA] = "STREAM_DATA";
    this->_dataCmd[DataCmd::STREAM_ROTATIONS] = "STREAM_ROTATIONS";
    this->_dataCmd[DataCmd::START_STREAM] = "START_STREAM";
    this->_dataCmd[DataCmd::PAUSE_STREAM] = "PAUSE_STREAM";
    this->_dataCmd[DataCmd::STOP_STREAM] = "STOP_STREAM";

    GRAlgorithm::setupMadgwick(140, 140, 140, 140, 140, 220);
}

GRUiSrv::GRUiSrv(const std::string sockPath)
{
    memset(this->_ans, 0, sizeof(this->_ans));
    this->_defaultSockPath = sockPath;
}

GRUiSrv::~GRUiSrv()
{
    for(auto& client: _clients)
    {
        if(client.second != nullptr) 
        {
            client.second->destroy();
        }
    }
}

bool GRUiSrv::run()
{
    std::thread rn(&GRUiSrv::_runInThread, this);
    rn.detach();
}
bool GRUiSrv::_runInThread()
{
    try{
        libsocket::unix_stream_server srv(this->_defaultSockPath);

        std::cout<<"beforeWHile"<<std::endl;
        std::string res;
        res.resize(128);
        //        srv.destroy();
        std::cout<<"THE FDD"<<srv.getfd()<<std::endl;
        //   while(true)
        // {
        // cli = srv.accept2();
        while(srv.getfd() > 0)
        {
            try{
               // std::unique_ptr<libsocket::unix_stream_client> tmpStreamCli;
               // tmpStreamCli = srv.accept2();
               // *tmpStreamCli>>res;
                dataClient tmpDataCli;
                tmpDataCli.streamClient = srv.accept2(); 
                *tmpDataCli.streamClient>>res;

                //std::cout<<res<<std::endl;
                std::vector<std::string> params = _splitBySpace(&res);
                if(params.size() == 3)
                {
                    //TODO need to veriify all params
                    if(!_clients.count(params[1]))
                    {

                        res.resize(5);
                        res = "KO";
                        *tmpDataCli.streamClient<<res;
                        std::cout<<"there is no sensor with name: "<<params[1]<<std::endl;
                    }
                    else
                    {

                        tmpDataCli.dataCmd = params[0];
                        tmpDataCli.imuId = params[1];
                        tmpDataCli.dataType = params[2];
                        //tmpDataCli.
                        res.resize(6);
                        res = "OK";
                        std::cout<<"ON NEW CLIENT"<<params[1]<<std::endl;
                        *tmpDataCli.streamClient<<res;
                        //         _clients[cmd[1]] = nullptr;
                        
                       // _clients[params[1]] = std::move(tmpCli);
                        this->_dataClients.push_back(std::move(tmpDataCli));
                    }
                }
                else
                {
                    std::cout<<"ERROR: To much data parameters recieved :"<<
                        res<< 
                        "need to be: STREAM_DATA [SENSOR_NAME] [DATA_FORMAT]"<<std::endl;
                }
                res.resize(128);
            }
            catch (const libsocket::socket_exception& inexc)
            {
                std::cerr<<inexc.mesg;
                return false;
            }

            std::cout<<"iteration --------"<<std::endl;
        }

        std::cout<<"afterWhile"<<std::endl;
        if(srv.getfd() > 0)
        {
            srv.destroy();
        }
    }
    catch (const libsocket::socket_exception& exc)
    {
        return false;
    }
    return true;
    }
    void GRUiSrv::writeData(GRMessage* msg)
    {
        //        std::cout<<"IN CALLBACK UI"<<std::endl;
        GRAlgorithm::madgwickUpdate(msg, &this->_algMsg);
        for(auto& client : this->_clients)
        {
            if(client.second != nullptr && client.second->getfd()>0)
            {
                if(client.first.compare(0, 2, "ui")==0)
                {
                    if(client.first != "uirotations")
                    {
                        try
                        {
                            std::cout<<this->eulerRotationsToStr(GRAlgorithm::getEulerRotations(this->_algMsg));
                            //std::string key = client.first.substr(2, client.first.size());
                            //std::cout<<std::to_string(this->_dataType[key])<<"=============="<<std::endl;
                            *client.second<<msg->imus[client.first
                                .substr(2, client.first.size())]->imuByteData;//getAsStr();
                        }
                        catch(const libsocket::socket_exception& exc)
                        {
                            std::cerr<<exc.mesg;
                        }
                    }
                    else 
                    {   
                        try
                        {
                            //    *client.second<<this->//_algiMsg

                            *client.second<<this->eulerRotationsToStr(GRAlgorithm::getEulerRotations(this->_algMsg));
                        }
                        catch(const libsocket::socket_exception& exc)
                        {
                            cerr<<exc.mesg;
                        }


                    }
                }
                else if(client.first.compare(0, 2, "io")==0)
                {
                    if(client.first != "iorotations")
                    {
                        try
                        {
                            *client.second<<msg->imus[client.first.substr(2, client.first.size())]
                                ->getAsStr();
                        }
                        catch(const libsocket::socket_exception& exc)
                        {
                            std::cerr<<exc.mesg;
                        }
                    }
                    else
                    {

                    }
                }
                /*  else
                    {

                    }*/   
            }
        }
    }
    std::vector<std::string> GRUiSrv::_splitBySpace(std::string* inp)
    {
        //TODO need to fix vector size its put the terminating zero as an element of vector
        std::regex reg("\\s+");
        return std::vector<std::string>{
            std::regex_token_iterator<std::string::iterator>(inp->begin(),
                    inp->end(),
                    reg,
                    -1),{}
        };
    }
    std::string GRUiSrv::eulerRotationsToStr(unordered_map<std::string, std::vector<double>> inpRotations)
    {
        std::vector<std::string> tmpV;
        tmpV.resize(6);
        std::string out;
        for(auto& imuAngles: inpRotations)
        { 
            tmpV.at((int)this->_imuNameToInt8[imuAngles.first]) = vectorDoubleToStr(imuAngles.second);
        }
        for(auto& s: tmpV)
        {
            out += s;
        }
        return out;
    }
    std::string GRUiSrv::quaternionToStr(std::unordered_map<std::string, Eigen::Quaterniond> inRot)
    {
        std::vector<std::string> tmpV;
        tmpV.resize(6);
        std::string out;
        //TODO funalize and make it unified;

    }
    std::string GRUiSrv::vectorDoubleToStr(std::vector<double> v)
    {
        std::stringstream res;
        std::copy(v.begin(), v.end(), 
                std::ostream_iterator<int16_t>(res, ","));
        return res.str();

    }

