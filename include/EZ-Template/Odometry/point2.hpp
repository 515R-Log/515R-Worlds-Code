
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

#include <vector>
#pragma once

class point2{
public:
  double x=0;
  double y=0;
  double theta=0;
  point2(double new_x, double new_y, double new_theta=0);

  point2();

  void setPosition(std::vector<double> position);

  std::vector<double>getPosition();
};
