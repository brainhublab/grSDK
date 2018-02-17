#include "grDevice.h"


/*
 * GRDevNames
 */
GRDevNames::GRDevNames()
{
    this->left = "GR[L]";
    this->right = "GR[R]";
    this->test = "HC-06";
}

std::string GRDevNames::get_left()
{
    return this->left;
}
void GRDevNames::set_left(std::string n_left)
{
    this->left = n_left;
}

std::string GRDevNames::get_right()
{
    return this->right;
}
void GRDevNames::set_right(std::string n_right)
{
    this->right = n_right;
}

std::string GRDevNames::get_test()
{
    return this->test;
}
void GRDevNames::set_test(std::string n_test)
{
    this->test = n_test;
}

/*
 * GRImu
 */
GRImu::GRImu()
{
    this->gyro.clear();
    this->acc.clear();
    this->mag.clear();
    this->time_stamp = 0.0;
    this->is_connected = false;
}

bool GRImu::empty()
{
    return (this->gyro.empty() && this->acc.empty() && this->mag.empty()) ||
            this->gyro.empty() || this->acc.empty() || this->mag.empty();
}

bool GRImu::is_complete()
{
    return (this->gyro.size() == 3) && (this->acc.size() == 3) && (this->mag.size() == 3);
}

bool GRImu::clear()
{

}

std::vector<double> GRImu::get_gyro()
{
    return this->gyro;
}
void GRImu::set_gyro(std::vector<double> n_gyro)
{
    this->gyro = n_gyro;
}

std::vector<double> GRImu::get_acc()
{
    return this->acc;
}
void GRImu::set_acc(std::vector<double> n_acc)
{
    this->acc = n_acc;
}

std::vector<double> GRImu::get_mag()
{
    return this->mag;
}
void GRImu::set_mag(std::vector<double> n_mag)
{
    this->mag = n_mag;
}
void GRImu::clear_mag()
{
    this->mag.clear();
}

unsigned long GRImu::get_time_stamp()
{
    return this->time_stamp;
}
void GRImu::set_time_stamp(unsigned long n_time_stamp)
{
    this->time_stamp = n_time_stamp;
}

bool GRImu::get_is_connected()
{
    return this->is_connected;
}
void GRImu::set_is_connected(bool n_is_connected)
{
    this->is_connected = n_is_connected;
}




/*
 * GRMessage
 */
GRMessage::GRMessage()
{
    this->id = 0;

    this->imus["pinky"] = &(this->pinky);
    this->imus["ring"] = &(this->ring);
    this->imus["middle"] = &(this->middle);
    this->imus["index"] = &(this->index);
    this->imus["thumb"] = &(this->thumb);
    this->imus["palm"] = &(this->palm);

    this->palm.set_is_connected(true);
}

bool GRMessage::clear()
{
    this->pinky.clear();
    this->ring.clear();
    this->middle.clear();
    this->index.clear();
    this->thumb.clear();
    this->palm.clear();

    return true;
}

int GRMessage::get_id()
{
    return this->id;
}
void GRMessage::set_id(int n_id)
{
    this->id = n_id;
}

GRImu GRMessage::get_pinky()
{
    return this->pinky;
}
void GRMessage::set_pinky(GRImu n_pinky)
{
    this->pinky = n_pinky;
}

GRImu GRMessage::get_ring()
{
    return this->ring;
}
void GRMessage::set_ring(GRImu n_ring)
{
    this->ring = n_ring;
}

GRImu GRMessage::get_middle()
{
    return this->middle;
}
void GRMessage::set_middle(GRImu n_middle)
{
    this->middle = n_middle;
}

GRImu GRMessage::get_index()
{
    return this->index;
}
void GRMessage::set_index(GRImu n_index)
{
    this->index = n_index;
}

GRImu GRMessage::get_thumb()
{
    return this->thumb;
}
void GRMessage::set_thumb(GRImu n_thumb)
{
    this->thumb = n_thumb;
}

GRImu GRMessage::get_palm()
{
    return this->palm;
}
void GRMessage::set_palm(GRImu n_palm)
{
    this->palm = n_palm;
}

std::unordered_map<std::string, GRImu*> GRMessage::get_imus()
{
    return this->imus;
}


/*
 * GRDevice
 */
GRDevice::GRDevice() {
    this->id = 0;
}

GRDevice& GRDevice::operator=(const GRDevice& dev)
{
    this->id = dev.id;
    this->name = dev.name;
    this->address = dev.address;

    return *this;
}

void GRDevice::clear_attributes()
{
    this->id = 0;
    this->name.clear();
    this->address.clear();
}

int GRDevice::get_id()
{
    return this->id;
}

void GRDevice::set_id(int n_id)
{
    this->id = n_id;
}

std::string GRDevice::get_name()
{
    return this->name;
}

void GRDevice::set_name(std::string new_name)
{
    this->name = new_name;
}

std::string GRDevice::get_address()
{
    return this->address;
}

void GRDevice::set_address(std::string new_address)
{
    this->address = new_address;
}

/*
 * GRAlgMessage
 */
GRAlgMessage::GRAlgMessage()
{
    this->nodes["pinky"] = &(this->pinky);
    this->nodes["ring"] = &(this->ring);
    this->nodes["middle"] = &(this->middle);
    this->nodes["index"] = &(this->index);
    this->nodes["thumb"] = &(this->thumb);
    this->nodes["palm"] = &(this->palm);
}

bool GRAlgMessage::clear()
{
    this->pinky.clear();
    this->ring.clear();
    this->middle.clear();
    this->index.clear();
    this->thumb.clear();
    this->palm.clear();

    return true;
}

std::vector<double> GRAlgMessage::get_pinky()
{
    return this->pinky;
}
void GRAlgMessage::set_pinky(std::vector<double> n_pinky)
{
    this->pinky = n_pinky;
}

std::vector<double> GRAlgMessage::get_ring()
{
    return this->ring;
}
void GRAlgMessage::set_ring(std::vector<double> n_ring)
{
    this->ring = n_ring;
}

std::vector<double> GRAlgMessage::get_middle()
{
    return this->middle;
}
void GRAlgMessage::set_middle(std::vector<double> n_middle)
{
    this->middle = n_middle;
}

std::vector<double> GRAlgMessage::get_index()
{
    return this->index;
}
void GRAlgMessage::set_index(std::vector<double> n_index)
{
    this->index = n_index;
}

std::vector<double> GRAlgMessage::get_thumb()
{
    return this->thumb;
}
void GRAlgMessage::set_thumb(std::vector<double> n_thumb)
{
    this->thumb = n_thumb;
}

std::vector<double> GRAlgMessage::get_palm()
{
    return this->palm;
}
void GRAlgMessage::set_palm(std::vector<double> n_palm)
{
    this->palm = n_palm;
}

std::vector<double>* GRAlgMessage::get_node(std::string key)
{
    return this->nodes[key];
}



#include <nbind/nbind.h>
NBIND_CLASS(GRDevNames) {
    construct<>();

    method(get_left);
    method(set_left);

    method(get_right);
    method(set_right);

    method(get_test);
    method(set_test);
}

NBIND_CLASS(GRImu) {
    construct<>();

    method(empty);

    method(is_complete);

    method(clear);

    method(get_gyro);
    method(set_gyro);

    method(get_acc);
    method(set_acc);

    method(get_mag);
    method(set_mag);

    method(get_time_stamp);
    method(set_time_stamp);

    method(get_is_connected);
    method(set_gyro);
}

NBIND_CLASS(GRMessage) {
    construct<>();

    method(clear);
    method(get_id);
    method(set_id);
    method(get_pinky);
    method(set_pinky);
    method(get_ring);
    method(set_ring);
    method(get_middle);
    method(set_middle);
    method(get_index);
    method(set_index);
    method(get_thumb);
    method(set_thumb);
    method(get_palm);
    method(set_palm);
}

NBIND_CLASS(GRDevice) {
    construct<>();
    method(clear_attributes);
    method(get_id);
    method(set_id);
    method(get_name);
    method(set_name);
    method(get_address);
    method(set_address);
}

NBIND_CLASS(GRAlgMessage) {
    construct<>();

    method(clear);
    method(get_pinky);
    method(set_pinky);
    method(get_ring);
    method(set_ring);
    method(get_middle);
    method(set_middle);
    method(get_index);
    method(set_index);
    method(get_thumb);
    method(set_thumb);
    method(get_palm);
    method(set_palm);
    method(get_node);
}
