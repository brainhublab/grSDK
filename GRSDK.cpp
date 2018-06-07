//#include "GRT/GRT.h"
//#include "./inc/grAlgorithm.h"
#include "./inc/grDevManager.h"
//#include "./inc/grConnection.h"
#include "./inc/grDevice.h"
//#include "./inc/grTrajectory.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <map>
#include <unordered_map>
#include <ncurses.h>
#include <node.h>

#include <bluetooth/hci_lib.h>
namespace GR
{

    using v8::Exception;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Number;
    using v8::Object;
    using v8::String;
    using v8::Value;

    void Test(const FunctionCallbackInfo<Value>& args)
    {
        Isolate* isolate=args.GetIsolate();

        GRDevManager devManager;

        std::unordered_map<int, device_t> devices;

        devices =  devManager.getAvalibleDevices();

       int devId = -1;
        for(std::unordered_map<int, device_t>::iterator it=devices.begin(); it!=devices.end(); it++)
        {
            if(it->second.name == "GR[R]")
            {
                devId = it->first;
            }
        }

        if(devId == -1)
        {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "no devices found")));
            return;
        }

        Local<Number> num = Number::New(isolate, devId); 

        args.GetReturnValue().Set(num);       


    }

    void Init(Local<Object> exports)
    {
        NODE_SET_METHOD(exports, "test", Test);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, Init)
}
