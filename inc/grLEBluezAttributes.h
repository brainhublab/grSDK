#ifndef GR_BLUEZ_ATTRIBUTES
#define GR_BLUEZ_ATTRIBUTES

#include <iostream>
#include <vector>
#include <map>
#include <deque>
#
/*
 * structure for default bluezobject and aarameters 
 */
struct dbus_bluez
{
    std::string name;
    std::string path;
    std::string properties;
    std::string introspectable;
    dbus_bluez()
    {
        name = "org.bluez";
        path = "/org/bluez";
        properties = "org.freedesktop.DBus.Properties";
        introspectable = "org.freedesktop.DBus.Introspectable";

    }

};
/*
 * structure with default names of GR devices
 */
struct device_names
{
    std::string left;
    std::string right;
    device_names()
    {
        left = "Gestus[L]";
        right = "Gestus[R]";
    }

};
/*
 * default GR device UUID's setted inhardware 
 */
struct finger
{
    std::string gyro;
    std::string accelerometer;
    std::string mag;
};
struct device_uuids
{
    
    
    finger fingers[5];
    std::string gyro;
    std::string accelerometer;
    std::string magnet;

    std::string rx;
    std::string tx;

    std::string tmp_uuid;
    device_uuids()
    {
         gyro = "fced6418-c015-45ea-b50d-1de32a1c2f6d";
         accelerometer = "fced6419-c015-45ea-b50d-1de32a1c2f6d";
         magnet = "fced641a-c015-45ea-b50d-1de32a1c2f6d";

         rx = "";
         tx = "";

         tmp_uuid = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";

        fingers[0].gyro = "fced6409-c015-45ea-b50d-1de32a1c2f6d";
        fingers[0].accelerometer = "fced640a-c015-45ea-b50d-1de32a1c2f6d";
        fingers[0].mag = "fced640b-c015-45ea-b50d-1de32a1c2f6d";

        fingers[1].gyro = "fced640c-c015-45ea-b50d-1de32a1c2f6d";
        fingers[1].accelerometer = "fced640d-c015-45ea-b50d-1de32a1c2f6d";
        fingers[1].mag = "fced640e-c015-45ea-b50d-1de32a1c2f6d";

        fingers[2].gyro = "fced640f-c015-45ea-b50d-1de32a1c2f6d";
        fingers[2].accelerometer = "fced6410-c015-45ea-b50d-1de32a1c2f6d";
        fingers[2].mag = "fced6411-c015-45ea-b50d-1de32a1c2f6d";

        fingers[3].gyro = "fced6412-c015-45ea-b50d-1de32a1c2f6d";
        fingers[3].accelerometer = "fced6413-c015-45ea-b50d-1de32a1c2f6d";
        fingers[3].mag = "fced6414-c015-45ea-b50d-1de32a1c2f6d";

        fingers[4].gyro = "fced6415-c015-45ea-b50d-1de32a1c2f6d";
        fingers[4].accelerometer = "fced6416-c015-45ea-b50d-1de32a1c2f6d";
        fingers[4].mag = "fced6417-c015-45ea-b50d-1de32a1c2f6d";

    }
};
/*
 * structure for storing of device and adapter parameters
 */
struct device_t
{
   int id;
   std::string name;
   std::string path;

   std::map<std::string, std::string> interfaces;

   finger fingers[5];
   std::string gyro;
   std::string accelerometer;
   std::string magnet;
   device_t()
   {
       id = 0;
       name.clear();
       path.clear();
       gyro.clear();
       accelerometer.clear();
       magnet.clear();

       interfaces.clear();

       for(int i=0; i<5; i++)
       {
        fingers[i].gyro.clear();
        fingers[i].accelerometer.clear();
        fingers[i].mag.clear();
       }

    }

};

#endif
