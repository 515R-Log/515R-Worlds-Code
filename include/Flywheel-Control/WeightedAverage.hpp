#include <vector>
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



class WeightedAverage{
private:
  std::vector<double> list;

public:
  WeightedAverage(const int size);

  void setSize(int size);

  int getSize();

  void addValue(const double value);

  double getWeightedAverage();
};
