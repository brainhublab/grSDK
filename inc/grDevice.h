#ifndef GR_DEVICE
#define GR_DEVICE

#include <iostream>
#include <deque>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <unordered_map>
#include <stdlib.h>

/**
 * gr devices names needed for easier findign of avalible devices
 */
struct GRDevNames
{
    public:
        std::string left;
        std::string right;
        std::string test;

        GRDevNames();

        std::string get_left();
        void set_left(std::string n_left);

        std::string get_right();
        void set_right(std::string n_right);

        std::string get_test();
        void set_test(std::string n_test);
};

struct GRImu
{
    public:
        /**
         * @brief timespamp
         */
        unsigned long time_stamp;

        /**
         * @brief boolean
         */
        bool is_connected;

        /**
         * @brief gyroscope data
         */
        std::vector<double> gyro;

        /**
         * @brief accelerometer data
         */
        std::vector<double> acc;

        /**
         * @brief magnetometr data
         */
        std::vector<double> mag;

        /**
         * @brief constructor
         */
        GRImu();

        /**
         * @brief checks if imu has no data
         */
        bool empty();

        /**
         * @brief checks if imu data is complite
         */
        bool is_complete();

        /**
         * @brief deletes data
         */
        bool clear();

        std::vector<double> get_gyro();
        void set_gyro(std::vector<double> n_gyro);
        void clear_gyro();

        std::vector<double> get_acc();
        void set_acc(std::vector<double> n_acc);
        void clear_acc();

        std::vector<double> get_mag();
        void set_mag(std::vector<double> n_mag);
        void clear_mag();

        unsigned long get_time_stamp();
        void set_time_stamp(unsigned long n_time_stamp);

        bool get_is_connected();
        void set_is_connected(bool n_is_connected);
};

struct GRMessage
{
    public:
        int id;
        GRImu pinky;
        GRImu ring;
        GRImu middle;
        GRImu index;
        GRImu thumb;
        GRImu palm;

        std::unordered_map<std::string, GRImu*> imus;

        GRMessage();
        bool clear();

        int get_id();
        void set_id(int);

        GRImu get_pinky();
        void set_pinky(GRImu);

    bool empty()
    {
        if(pinky.empty() || ring.empty() || middle.empty() || index.empty() || thumb.empty() ||
         (pinky.empty() && ring.empty() && middle.empty() && index.empty() && thumb.empty()) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

//=======
        GRImu get_ring();
        void set_ring(GRImu);

        GRImu get_middle();
        void set_middle(GRImu);

        GRImu get_index();
        void set_index(GRImu);

        GRImu get_thumb();
        void set_thumb(GRImu);

        GRImu get_palm();
        void set_palm(GRImu);

        std::unordered_map<std::string, GRImu*> get_imus();
//>>>>>>> rotations
};

struct GRDevice
{
    public:
        int id = 0;
        std::string name = "";
        std::string address = "";

        GRDevice();
        GRDevice& operator=(const GRDevice& dev);

        void clear_attributes();

        int get_id();
        void set_id(int);

        std::string get_name();
        void set_name(std::string);

        std::string get_address();
        void set_address(std::string);
};

struct GRAlgMessage
{
    public:
        /**
         * @brief quaternion for pinky
         */
        std::vector<double>  pinky;
        /**
         * @brief quaternion for ring
         */
        std::vector<double>  ring;
        /**
         * @brief quaternion for middle
         */
        std::vector<double>  middle;
        /**
         * @brief quaternion for index
         */
        std::vector<double>  index;
        /**
         * @brief quaternion for thumb
         */
        std::vector<double>  thumb;
        /**
         * @brief quaternion for palm
         */
        std::vector<double>  palm;
        /**
         * @brief map for iterating through quaternions
         */
        std::unordered_map<std::string, std::vector<double>*> nodes;

        /**
         * @brief constructor
         */
        GRAlgMessage();

        /**
         * @brief cleaner
         */
        bool clear();

        std::vector<double> get_pinky();
        void set_pinky(std::vector<double> n_pinky);

        std::vector<double> get_ring();
        void set_ring(std::vector<double> n_ring);

        std::vector<double> get_middle();
        void set_middle(std::vector<double> n_middle);

        std::vector<double> get_index();
        void set_index(std::vector<double> n_index);

        std::vector<double> get_thumb();
        void set_thumb(std::vector<double> n_thumb);

        std::vector<double> get_palm();
        void set_palm(std::vector<double> n_palm);

        std::vector<double>* get_node(std::string key);
};

#endif
