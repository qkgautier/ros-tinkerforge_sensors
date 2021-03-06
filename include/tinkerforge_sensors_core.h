#ifndef TINKERFORGE_SENSORS_CORE_H
#define TINKERFORGE_SENSORS_CORE_H

// ROS includes
#include <list>
#include <map>
#include "ros/ros.h"
#include "ros/time.h"
#include "sensor_device.h"
#include "ip_connection.h"
#include "brick_imu.h"
#include "brick_imu_v2.h"
#include "bricklet_gps.h"
#include "bricklet_industrial_digital_in_4.h"
#include "bricklet_dual_button.h"
#include "bricklet_ambient_light.h"
#include "bricklet_humidity.h"
#include "bricklet_temperature.h"
#include "bricklet_temperature_ir.h"
#include "bricklet_distance_ir.h"
#include "bricklet_distance_us.h"
#include "bricklet_motion_detector.h"

#define M_PI	3.14159265358979323846  /* pi */

class TinkerforgeSensors
{
public:
  //! Constructur
  TinkerforgeSensors();

  //! Constructor with tinkerforge connection parameters
  TinkerforgeSensors(std::string host, int port);

  //! Destructor
  ~TinkerforgeSensors();

  //! Init
  bool init();

  //! Publish the IMU message
  void publishImuMessage(SensorDevice *sensor);

  //! Publish the NavSatFix message.
  void publishNavSatFixMessage(SensorDevice *sensor);

  //! Publish the MagneticField message
  void publishMagneticFieldMessage(SensorDevice *sensor);

  //! Publish the Humidity message
  void publishHumidityMessage(SensorDevice *sensor);

  //! Publish the Temperature message
  void publishTemperatureMessage(SensorDevice *sensor);

  //! Publish the Illuminance message
  void publishIlluminanceMessage(SensorDevice *sensor);

  //! Publish the Range message
  void publishRangeMessage(SensorDevice *sensor);

  //! Publish Sensors Messages
  void publishSensors();

  //! Store for sensor params
  std::map<std::string, std::map<std::string, SensorParam>> conf;
  //! Sensor list
  std::list<SensorDevice*> sensors;

private:
  //! Callback function for Tinkerforge ip connected .
  static void callbackConnected(uint8_t connect_reason, void *user_data);

  //! Callback function for Tinkerforge enumerate.
  static void callbackEnumerate(const char *uid, const char *connected_uid,
    char position, uint8_t hardware_version[3],
    uint8_t firmware_version[3], uint16_t device_identifier,
    uint8_t enumeration_type, void *user_data);

  //! Calculate deg from rad
  float rad2deg(float x)
  {
    return x*180.0/M_PI;
  }

  //! Calculate rad from deg
  double deg2rad(double x)
  {
    return x*M_PI/180.0;
  }
private:
  //! IP connection to Tinkerforge deamon.
  IPConnection ipcon;
  //! The Tinkerforge HOST
  std::string host;
  //! The Tinkerforge PORT
  int port;
  //! The IMU convergence_speed
  int imu_convergence_speed;
  //! Time to correct the imu orientation
  ros::Time imu_init_time;
};

#endif
