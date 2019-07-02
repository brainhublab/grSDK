#include "grUiSrv.h"

GRUiSrv::GRUiSrv()
{
    memset(this->_ans, 0, sizeof(this->_ans));
    this->_serverFd = 0;
    this->_clientFd = 0;
    this->_bytesRecieved = 0;
    this->_ret = 0;
    this->_defaultSockPath = DEFAULT_PATH;

    this->_dataType["bitearray"] = DataType::BYTE;
    this->_dataType["string"] = DataType::STRING;

    this->_dataCmd["STREAM_DATA"] = DataCmd::STREAM_DATA;
    this->_dataCmd["STREAM_ROTATIONS_DATA"] = DataCmd::STREAM_ROTATIONS;

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
    try
    {
        //create a server socket
        libsocket::unix_stream_server srv(this->_defaultSockPath);

        //create selectset for server
        libsocket::selectset<libsocket::unix_stream_server> srvSet;
        srvSet.add_fd(srv, LIBSOCKET_READ);

        std::cout<<"beforeWHile"<<std::endl;
        std::string res;
        res.resize(128);
        std::cout<<"THE FDD"<<srv.getfd()<<std::endl;
        while(srv.getfd() > 0)
        {
            try
            {
                std::cout<<"CALLED SELECT"<<std::endl;
                libsocket::selectset<libsocket::unix_stream_server>::ready_socks readyPair;
                readyPair = srvSet.wait();

                libsocket::unix_stream_server* readySrv = dynamic_cast<libsocket::unix_stream_server*>(
                        readyPair.first.back());

                readyPair.first.pop_back();

                dataClient tmpDataCli;
                tmpDataCli.streamClient = readySrv->accept2(); 
                *tmpDataCli.streamClient>>res;
                std::vector<std::string> params = _splitBySpace(&res);

                if(this->_checkParams(&params))
                {
                    switch(int paramLen = params.size())
                    {
                        std::cout<<paramLen<<"----------------------------------PARAM LEN"<<std::endl;
                        case 0:
                        break;
                        case 1:
                        std::cout<<"ROTATION CLIENT"<<std::endl;
                        break;
                        case 2:
                        break;
                        case 3:
                        this->_asignClient(&tmpDataCli, &params, true);
                        break;
                        case 4:
                        if(this->_checkIfClientExist(&params[3]))
                        {

                            this->_asignClient(&tmpDataCli, &params, false);
                        }
                        else
                        {
                            this->_asignClient(&tmpDataCli, &params, true);
                        }
                        break;
                    }
                }
                else
                {
                    std::cout<<"ERROR: Wrong arguments"<<std::endl;
                }
                res.resize(128);
            }
            catch (const libsocket::socket_exception& inexc)
            {
                std::cout<<inexc.mesg;
                return false;
            }
        }
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
void GRUiSrv::_asignClient(dataClient* dataCli, std::vector<std::string>* params, bool isNew)
{
    std::string res;
    if(isNew)
    {
        dataCli->dataCmd = params->at(0);
        dataCli->imuId = params->at(1);
        dataCli->dataType = params->at(2);
        dataCli->UUID = this->_generateUUID(dataCli->streamClient.get());
        res.resize(127);
        res = "OK " + this->_generateUUID(dataCli->streamClient.get());
        *dataCli->streamClient<<res;
        this->_dataClients.push_back(std::move(*dataCli));

    }
    else
    {
        auto it = std::begin(this->_dataClients);
        while(it != std::end(this->_dataClients))
        {
            if((*it).UUID == params->at(3))
            {
                res.resize(5);
                res = "OK";
                dataCli->dataCmd = params->at(0);
                dataCli->imuId = params->at(1);
                dataCli->dataType = params->at(2);
                dataCli->UUID = params->at(3);
                *dataCli->streamClient<<res;
                (*it) = std::move(*dataCli);
            }
            it++;
        }
    }
}
void GRUiSrv::writeData(GRMessage* msg)
{
    //        std::cout<<"IN CALLBACK UI"<<std::endl;
    //   GRAlgorithm::madgwickUpdate(msg, &this->_algMsg);
    auto it = std::begin(this->_dataClients);
    while(it != std::end(this->_dataClients))
    {
        if((*it).streamClient->getfd() < 0)
        {
            it = this->_dataClients.erase(it);
            std::cout<<"Erase element from Clients"<<std::endl;
            //continue;
        }

        switch(DataCmd c=this->_dataCmd[(*it).dataCmd])
        {
            case DataCmd::STREAM_DATA:
                // std::cout<<"STREAM BYTE DATA SWITCH "<< (*it).imuId<<std::endl;
                _writeRawDataToCli(&(*it), msg);
                break;
            case DataCmd::STREAM_ROTATIONS:
                // std::cout<<"STREAM ROTATION BYTE SWITCH"<<std::endl;
                //_writeRotationsDataToCli(&client, msg);
                break;
            case DataCmd::START_STREAM:
                // std::cout<<"START_STREAM BYTE SWITCH";
                _writeRawDataToCli(&(*it), msg);
                break;
            case DataCmd::PAUSE_STREAM:
                //  std::cout<<"PAUSE STREAM BYTE SWITCH";
                break;
            case DataCmd::STOP_STREAM:
                // std::cout<<"STOP STREAM BYTE SWITCH";
                break;
        }
        it++;
    }
}
void GRUiSrv::_writeRawDataToCli(dataClient* cli, GRMessage* msg)
{
    switch(DataType dt = this->_dataType[cli->dataType])
    {
        case DataType::BYTE:
            // std::cout<<"BYTE DATA IN SWITCH"<<std::endl;
            try
            {
                if(cli != nullptr)
                {
                    cli->streamClient->snd(msg->imus[cli->imuId]->imuByteData.c_str(), 18, MSG_NOSIGNAL); 
                }
                //    *cli->streamClient<<msg->imus[cli->imuId]->imuByteData;
            }
            catch(const libsocket::socket_exception& exc)
            {
                std::cout<<exc.err<<" <-ERR";
                //if(exc.err == 32)
               // {
                    cli->streamClient->destroy();     
               // }
            }
            break;
        case DataType::STRING:
            try
            {
                *cli->streamClient<<msg->imus[cli->imuId]->getAsStr();
            }
            catch(const libsocket::socket_exception& exc)
            {
                std::cout<<exc.mesg;
            }
            break;
    }
}
void GRUiSrv::_writeRotationsDataToCli(dataClient* cli, GRMessage* msg)
{
    GRAlgorithm::madgwickUpdate(msg, &this->_algMsg);
    try
    {
        *cli->streamClient<<
            this->eulerRotationsToStr(GRAlgorithm::getEulerRotations(this->_algMsg));
    }
    catch(const libsocket::socket_exception& exc)
    {
        std::cout<<exc.mesg;
    }

}

std::string GRUiSrv::_generateUUID(std::unique_ptr<libsocket::unix_stream_client>::pointer cliAddr)
{
    std::stringstream ss;
    ss << cliAddr;
    return ss.str();
}
bool GRUiSrv::_checkIfClientExist(std::string* uuid)
{
    auto it = std::begin(this->_dataClients);
    while(it != std::end(this->_dataClients))
    {
        if((*it).UUID == *uuid){
            return true;
        }
        it++;
    }
    return false;
}
bool GRUiSrv::_checkParams(std::vector<std::string>* params)
{
    int paramsLen = params->size();
    if(paramsLen > 4)
    {
        return false;
    }
    switch(paramsLen)
    {
        case 0:
            std::cout<<"ERROR: few params"<<std::endl;
            return false;
            break;
        case 1:
            if(!this->_dataCmd.count(params->at(0)))
            {
                std::cout<<"ERROR: there is no data command: "<<params->at(0)<<std::endl;
                return false;
            }
            return true;
            break; 
        case 2:
            std::cout<<"ERROR: to few params"<<std::endl;
            return false;
            break;
        case 3:
            if(!this->_dataCmd.count(params->at(0)))
            {
                std::cout<<"ERROR: there is no data command: "<<params->at(0)<<std::endl;
                return false;

            }
            if(auto it = std::find(this->_imuIds.begin(),
                        this->_imuIds.end(), params->at(1)) == this->_imuIds.end())
            {
                std::cout<<"ERROR: There is no imu with id: "<<params->at(1)<<std::endl;
                return false; 
            }
            if(!this->_dataType.count(params->at(2)))
            {
                std::cout<<"ERROR: There is no dataType: "<<params->at(2);
                return false;
            }
            return true;
            break;
        case 4:
            break;

    }
    return true;
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

