#include <vector>
#pragma once
/** @file tracking.hpp
  * @brief Package for tracking wheel information
  *
  * Used for groups of information pertaining to the tracking system
  * Only compatable with the following configuration:
  *   Forward-Facing Wheel  |  Sideways-Facing Wheel  |  Inertial Sensor
  *
  * @author Logan Daugherty
*/


class tracking{
public:

  // Variable space for forward-facing tracking wheel
  double fwd=0;

  // Variable space for sideways-facing tracking wheel.
  double back=0;

  // Variable space for inertial sensor
  double imu=0;

  /** @brief Creates a storage of tracking wheel and sensor data
    *
    * NOTE : This class is to be used as a cubby-system. It does not have any access to the sensors themselves.
    *
    * @param new_fwd
    *        Information of forward-facing tracking wheel
    * @param new_back
    *        Recorded information of sideways-facing tracking wheel
    * @param new_imu
    *        Imported data of inertial sensor. Set to 0 as default
  */
  tracking(double new_fwd, double new_back, double new_imu=0);

  /** @brief Creates a storage of tracking wheel and sensor data
    *
    * NOTE : This class is to be used as a cubby-system. It does not have any access to the sensors themselves.
    *
    * Starts information at 0. Usually used when taking direct information from sensors
  */
  tracking();

  /** @brief Update information of tracking set
    *
    * @param rotation
    *        Vector of data to input information. Make sure to import information in curly brackets. example.setRotation({1,2,3});
  */
  void setRotation(std::vector<double> rotation);

  /** @brief Gets information in vector format
    *
  */
  std::vector<double>getRotation();

};
