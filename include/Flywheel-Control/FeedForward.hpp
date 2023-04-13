#pragma once
#include "EZ-Template/PID.hpp"
#include "Flywheel-Control/WeightedAverage.hpp"
/** @file FeedFoward.hpp
  * @brief PID Adapated to Flywheel Control
  *
  * Contains two components : Voltage Kick-off ; Basic PID
  *
  * Voltage Kick-off sets the system to max power when the error(distance from target) is above a certain degree.
  * This allows for quick accelleration without concern of overshooting.
  * Once the value becomes nearer to the desired target, the control switches to basic PID.
  *
  * Basic PID that uses the PIDControl class.
  * This is useful for precise changes in velocity, especially when disc firing comes into play.
  *
  * This class is specifically designed for flywheel RPM. Any other application is likely to fail.
  *
  * @author Logan Daugherty
*/

class FeedForward {
private:
  // Voltage kick-off threshold
  double kP;
  double kI;
  double kD;
  double kV;

  double sumError;
  double lastError;

  // Smooth out changes in power with a weighted average
  WeightedAverage stabilize;

public:

  double target=800;

  /** @brief Sets up the Feed Forward Class with Constants and sampling size
    *
    * @param p
    *        Proportional gain of system. Typically ranges from 3-10
    * @param i
    *        Integral gain of system. Typically ranges from 0.001-0.005
    * @param d
    *        Integral gain of system. Typically ranges from 0.01-0.05
    * @param v
    *        Voltage kick-off range. Typically ranges from 30-80
    * @param sample_size
    *        Amount of consideration for Weighted Average. The higher the number, the smoother changes in voltage are.
    *        Default value is 10
  */
  FeedForward(const double p, const double i, const double d, const double v=600, const int sample_size=10);

  /** @brief Allows for redifinitions of PID constants and voltage kick-off
    *
    * @param p
    *        Proportional gain of system. Typically ranges from 3-10
    * @param i
    *        Integral gain of system. Typically ranges from 0.001-0.005
    * @param d
    *        Integral gain of system. Typically ranges from 0.01-0.05
    * @param v
    *        Voltage kick-off range. Typically ranges from 30-80
  */
  void setConstants(const double p, const double i, const double d, const double v=600);

  /** @brief Sets the Weighted Average sampling size
    *
    * @param sample_size
    *        Amount of consideration for Weighted Average. The higher the number, the smoother changes in voltage are.
    *        Default value is 10
  */
  void setSampleSize(const int sample_size);

  /** @brief Sets the target RPM to reach
    *
    * @param new_target
    *        Desired RPM value to reach in loop. All numbers are automatically set to positive.
  */
  void set_velocity(const double new_target);

  /** @brief Runs through the Feed forward commands ONCE
    *
    * @param value
    *        Current RPM of motor. Usually given by the motor command '.get_actual_velocity()'
    * @param delta_time
    *        Time it takes in-between each iteration. This is extremely important for PID commands
  */

  double calcPID(double currRPM, const short delta_time);

  double iterate(double value, const int delta_time);
};
 