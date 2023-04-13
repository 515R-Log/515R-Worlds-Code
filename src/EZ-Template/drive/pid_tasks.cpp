/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"
#include "pros/misc.hpp"

using namespace ez;

void Drive::ez_auto_task() {
  while (true) {
  if (odomEnabled)
    odom.iterate();

    switch (get_mode()) {
      case DRIVE:
      drive_pid_task();
      
      break;  case TURN:
      turn_pid_task();
      
      break;  case SWING:
      swing_pid_task();
      
      break;  case POINT:
      point_pid_task();
      break;
    }

    if (pros::competition::is_autonomous() && !util::AUTON_RAN)
      util::AUTON_RAN = true;
    else if (!pros::competition::is_autonomous())
      set_mode(DISABLE);

    pros::delay(util::DELAY_TIME);
  }
}

void Drive::enable_odometry(){
  odomEnabled=true;
}

void Drive::disable_odometry(){
  odomEnabled=false;
}

// Drive PID task
void Drive::drive_pid_task() {
  // Compute PID
  leftPID.compute(left_sensor());
  rightPID.compute(right_sensor());
  headingPID.compute(get_gyro());

  // Compute slew
  double l_slew_out = slew_calculate(left_slew, left_sensor());
  double r_slew_out = slew_calculate(right_slew, right_sensor());

  // Clip leftPID and rightPID to slew (if slew is disabled, it returns max_speed)
  double l_drive_out = util::clip_num(leftPID.output, l_slew_out, -l_slew_out);
  double r_drive_out = util::clip_num(rightPID.output, r_slew_out, -r_slew_out);

  // Toggle heading
  double gyro_out = heading_on ? headingPID.output : 0;

  // Combine heading and drive
  double l_out = l_drive_out + gyro_out;
  double r_out = r_drive_out - gyro_out;

  // Set motors
  if (drive_toggle)
    set_tank(l_out, r_out);
}

// Turn PID task
void Drive::turn_pid_task() {
  // Compute PID
  turnPID.compute(get_gyro());

  // Clip gyroPID to max speed
  double gyro_out = util::clip_num(turnPID.output, max_speed, -max_speed);

  // Clip the speed of the turn when the robot is within StartI, only do this when target is larger then StartI
  if (turnPID.constants.ki != 0 && (fabs(turnPID.get_target()) > turnPID.constants.start_i && fabs(turnPID.error) < turnPID.constants.start_i)) {
    if (get_turn_min() != 0)
      gyro_out = util::clip_num(gyro_out, get_turn_min(), -get_turn_min());
  }

  // Set motors
  if (drive_toggle)
    set_tank(gyro_out, -gyro_out);
}

// Swing PID task
void Drive::swing_pid_task() {
  // Compute PID
  swingPID.compute(get_gyro());

  // Clip swingPID to max speed
  double swing_out = util::clip_num(swingPID.output, max_speed, -max_speed);

  // Clip the speed of the turn when the robot is within StartI, only do this when target is larger then StartI
  if (swingPID.constants.ki != 0 && (fabs(swingPID.get_target()) > swingPID.constants.start_i && fabs(swingPID.error) < swingPID.constants.start_i)) {
    if (get_swing_min() != 0)
      swing_out = util::clip_num(swing_out, get_swing_min(), -get_swing_min());
  }

  if (drive_toggle) {
    // Check if left or right swing, then set motors accordingly
    if (current_swing == LEFT_SWING)
      set_tank(swing_out, 0);
    else if (current_swing == RIGHT_SWING)
      set_tank(0, -swing_out);
  }
}

// Point PID task
void Drive::point_pid_task() {

  // ----- GENERAL ----- //
  point2 target_point (targetPoints.at(0).x,targetPoints.at(0).y);
  double fwdError = util::calcHypo(odom.pos,target_point);

  // ----- REVERSED ----- //
  int baseFwd=max_speed; // Store the max speed as the forward speed
  double theta=imu.get_rotation(); // Store the angle
    if(targetPoints.at(0).backwards){ // If the pursuit is set to drive backwards
      theta-=180; // Flip the angle to turn backwards
      baseFwd*=-1; // Set the base speed to be backwards
    }

  // ----- POWER OUTPUT ----- //
  // Find difference in angle to target point
  double globalTargetTheta = util::radToDeg(util::calcAngle(odom.pos,target_point)); // Determine Basic Target Angle
  double thetaError = util::wrapAngle(globalTargetTheta-theta); // Account for 360 deg -> 0 deg overlap

  pTurnPID.compute(thetaError); // Determine the amount of power to turn

  // Set motor power of drivetrain to heading correction + standard power
  double left_out = left_out = util::clip_num(baseFwd*fwdScale - pTurnPID.output, max_speed, -max_speed);
  double right_out = right_out = util::clip_num(baseFwd*fwdScale + pTurnPID.output, max_speed, -max_speed);

  // ----- TARGET REACHED ----- //
  // If the robot has driven its target point
  if(fwdError < pointPIDSwitch){
  
    // If there are no points remaining
    if(targetPoints.size() < 2){
      // If the drivetrain is set to backwards, reverse the distance
      if(targetPoints.at(0).backwards)  fwdError*=-1;
      // Switch pid systems to straight-forwards movement
      set_drive_pid(fwdError,max_speed,false,false);
    }

    // If there are still points left
    else
      // Update all relevant drive parameters to that of the point
      update_point_pid_conditions(targetPoints.at(1));

    // Remove the current point from consideration
    targetPoints.erase(targetPoints.begin());
  }

  // If the robot is allowed to move, move.
  if (drive_toggle)
    set_tank(left_out, right_out);

}