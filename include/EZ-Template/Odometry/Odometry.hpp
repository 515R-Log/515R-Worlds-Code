#include "api.h"
#include "EZ-Template/Odometry/point2.hpp"
#include "EZ-Template/Odometry/tracking.hpp"
#include "okapi/impl/device/rotarysensor/adiEncoder.hpp"
#pragma once
/** @file FeedFoward.hpp
  * @brief PID Adapated to Flywheel Control
  *
  * Contains two components : Voltage Kick-off ; Basic PID
  *
  * Voltage Kick-off sets the system to max power when the error(distance from target) is above a certain degree. This allows for quick accelleration without concern of overshooting.
  * Once the value becomes nearer to the desired target, the control switches to basic PID.
  *
  * Basic PID that uses the PIDControl class. This is useful for precise changes in velocity, especially when disc firing comes into play.
  *
  * @author Logan Daugherty
*/


class Odometry{
// private:
public:
  point2 pos;
  tracking offset;
  tracking wheelDiam;
  tracking rotation;

  okapi::ADIEncoder fwd;
  okapi::ADIEncoder back;

  pros::Imu imu;

  double offSetAngle=0;

  double degToRad(double deg);

  void update(tracking new_rotation);

// public:
  Odometry(const char fwd_top_port, const char back_top_port, const int extender_port, const short imu_port, const double fwd_wheel_diameter, const double back_wheel_diameter, const double fwd_wheel_offset, const double back_wheel_offset);

  void setDimensions(const double fwd_wheel_offset, const double back_wheel_offset);

  void setPosition(const double x, const double y);
  void setPosition(const double x, const double y, const double theta);

  point2 getPosition();

  double getX();
  void setX(double x);

  double getY();
  void setY(double y);

  double getTheta();
  void setTheta(double theta);

  void iterate();
};
