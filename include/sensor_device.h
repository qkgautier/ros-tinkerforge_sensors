#ifndef SENSOR_DEVICE_H
#define SENSOR_DEVICE_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <stdint.h>
#include "ros/ros.h"
#include "bricklet_ambient_light.h"
#include "bricklet_ambient_light_v2.h"
#include "bricklet_distance_ir.h"
#include "bricklet_distance_us.h"
#include "bricklet_gps.h"
#include "brick_imu.h"
#include "brick_imu_v2.h"
#include "bricklet_temperature.h"

#define IMU_V2_MAGNETIC_DEVICE_IDENTIFIER 400

enum class SensorClass {TEMPERATURE, HUMIDITY, LIGHT, IMU, RANGE, GPS, MAGNETIC, MISC};
enum class ParamType {NONE,INT,DOUBLE,STRING,BOOL};

struct SensorParam
{
  ParamType type;
  std::string value_str;
  int value_int;
  float value_double;

  SensorParam()
  {
    type = ParamType::NONE;
    value_str = "";
    value_int = 0;
    value_double = 0.0;
  }
};

class SensorDevice
{
public:
  //! Constructor
  SensorDevice(void *dev, std::string uid, std::string topic, uint16_t type, SensorClass sclass, uint8_t rate)
  {
    this->dev = dev;
    this->uid = uid;
    this->seq = 0;
    this->type = type;
    this->sclass = sclass;
    this->rate = rate;
    this->frame = "base_link";

    if (topic.size() == 0)
      buildTopic(this);
    else
      this->topic = topic;
  };
  //! Destructor
  ~SensorDevice()
  {
    std::cout << "Destructor for:" << uid << std::endl;
  };

  //! build a default sensor topic if not given
  static std::string buildTopic(SensorDevice *sensor)
  {
    std::string topic;
    std::stringstream stream;

    dev_counter[(int)sensor->sclass]++;
    stream << SensorDevice::dev_counter[(int)sensor->sclass];
    switch (sensor->sclass)
    {
      case SensorClass::GPS:
        topic = std::string("/") + std::string("tfsensors") + std::string("/") + std::string("gps") + std::string(stream.str());
      break;
      case SensorClass::HUMIDITY:
        topic = std::string("/") + std::string("tfsensors") + std::string("/") + std::string("humidity") + std::string(stream.str());
      break;
      case SensorClass::IMU:
        topic = std::string("/") + std::string("tfsensors") + std::string("/") + std::string("imu") + std::string(stream.str());
      break;
      case SensorClass::LIGHT:
        // conversion not working with my compiler
        // std::to_string(dev_counter[AMBIENT_LIGHT_DEVICE_IDENTIFIER])
        topic = std::string("/") + std::string("tfsensors") + std::string("/") + std::string("illuminance") + std::string(stream.str());
      break;
      case SensorClass::MAGNETIC:
        topic = std::string("/") + std::string("tfsensors") + std::string("/") + std::string("magnetic") + std::string(stream.str());
      break;
      case SensorClass::RANGE:
        topic = std::string("/") + std::string("tfsensors") + std::string("/") + std::string("range") + std::string(stream.str());
      break;
      case SensorClass::TEMPERATURE:
        topic = std::string("/") + std::string("tfsensors") + std::string("/") + std::string("temperature") + std::string(stream.str());
      break;
    }
    sensor->topic = topic;
    return topic;
  };
  //! get a sensor parameter from parameter map
  SensorParam getParam(std::string param)
  {
    std::map<std::string, SensorParam>::iterator it;
    SensorParam sp;

    it = this->params.find(param);
    if (it != this->params.end())
      return it->second;
    return sp;
  }
public:
  void *getDev() { return dev; }
  std::string getUID() { return uid; }
  std::string getTopic() { return topic; }
  std::string getFrame() { return frame; }
  uint32_t getSeq() { seq++; return seq; }
  ros::Publisher getPub() { return pub; }
  uint16_t getType() { return type; }
  SensorClass getSensorClass() { return sclass; }
  std::map<std::string, SensorParam> params;

  void setTopic(std::string topic) { this->topic = topic; }
  void setPub(ros::Publisher pub) { this->pub = pub; }
  void setParams(std::map<std::string, SensorParam> params)
  { 
    this->params = params;
    // search and set frame_id
    SensorParam frame_id = getParam("frame_id");
    if (frame_id.type == ParamType::STRING)
      this->frame = frame_id.value_str;
  }
  static int dev_counter[10];

private:
  void *dev;
  std::string uid;
  std::string topic;
  std::string frame;
  uint32_t seq;
  uint16_t type;
  uint8_t rate;
  SensorClass sclass;
  ros::Publisher pub;
};
#endif
