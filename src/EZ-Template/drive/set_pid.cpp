/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "main.h"

// Updates max speed
void Drive::set_max_speed(int speed) {
  max_speed = util::clip_num(abs(speed), 127, -127);
}

void Drive::reset_pid_targets() {
  headingPID.set_target(0);
  leftPID.set_target(0);
  rightPID.set_target(0);
  forward_drivePID.set_target(0);
  backward_drivePID.set_target(0);
  turnPID.set_target(0);
}

void Drive::set_angle(double angle) {
  headingPID.set_target(angle);
  reset_gyro(angle);
}

void Drive::set_mode(e_mode p_mode) {
  mode = p_mode;
}

void Drive::set_turn_min(int min) { turn_min = abs(min); }
int Drive::get_turn_min() { return turn_min; }

void Drive::set_swing_min(int min) { swing_min = abs(min); }
int Drive::get_swing_min() { return swing_min; }

e_mode Drive::get_mode() { return mode; }

void Drive::drive_pid(double target, int speed, bool slew_on, bool toggle_heading){
  set_drive_pid(target,speed,slew_on,toggle_heading);
  wait_drive();
}

// Set drive PID
void Drive::set_drive_pid(double target, int speed, bool slew_on, bool toggle_heading) {
  TICK_PER_INCH = get_tick_per_inch();

  // Print targets
  if (print_toggle) printf("Drive Started... Target Value: %f (%f ticks)", target, target * TICK_PER_INCH);
  if (slew_on && print_toggle) printf(" with slew");
  if (print_toggle) printf("\n");

  // Global setup
  set_max_speed(speed);
  heading_on = toggle_heading;
  bool is_backwards = false;
  l_start = left_sensor();
  r_start = right_sensor();

  double l_target_encoder, r_target_encoder;

  // Figure actual target value
  l_target_encoder = l_start + (target * TICK_PER_INCH);
  r_target_encoder = r_start + (target * TICK_PER_INCH);

  // Figure out if going forward or backward
  if (l_target_encoder < l_start && r_target_encoder < r_start) {
    auto consts = backward_drivePID.get_constants();
    leftPID.set_constants(consts.kp, consts.ki, consts.kd, consts.start_i);
    rightPID.set_constants(consts.kp, consts.ki, consts.kd, consts.start_i);
    is_backwards = true;
  } else {
    auto consts = forward_drivePID.get_constants();
    leftPID.set_constants(consts.kp, consts.ki, consts.kd, consts.start_i);
    rightPID.set_constants(consts.kp, consts.ki, consts.kd, consts.start_i);
    is_backwards = false;
  }

  // Set PID targets
  leftPID.set_target(l_target_encoder);
  rightPID.set_target(r_target_encoder);

  // Initialize slew
  slew_initialize(left_slew, slew_on, max_speed, l_target_encoder, left_sensor(), l_start, is_backwards);
  slew_initialize(right_slew, slew_on, max_speed, r_target_encoder, right_sensor(), r_start, is_backwards);

  // Run task
  set_mode(DRIVE);
}

// Turn PID
void Drive::turn_pid(double target, int speed){
  set_turn_pid(target,speed); // Run 'set' version of turn
  wait_drive(); // wait for turn to complete
}

// Set turn PID
void Drive::set_turn_pid(double target, int speed) {
  // Print targets
  if (print_toggle) printf("Turn Started... Target Value: %f\n", target);

  // Set PID targets
  turnPID.set_target(target);
  headingPID.set_target(target);  // Update heading target for next drive motion
  set_max_speed(speed);

  // Run task
  set_mode(TURN);
}

// Swing PID
void Drive::swing_pid(e_swing type, double target, int speed){
  set_swing_pid(type,target,speed); // Run 'set' version of swing
  wait_drive(); // wait for swing turn to complete
}

// Set swing PID
void Drive::set_swing_pid(e_swing type, double target, int speed) {
  // Print targets
  if (print_toggle) printf("Swing Started... Target Value: %f\n", target);
  current_swing = type;

  // Set PID targets
  swingPID.set_target(target);
  headingPID.set_target(target);  // Update heading target for next drive motion
  set_max_speed(speed);

  // Run task
  set_mode(SWING);
}

void Drive::add_point(double x, double y, double speed, bool backwards, point_snap snap, point_transition pointTransition){
  point_pid_build new_point = {x, y, speed, backwards, snap, pointTransition};
  targetPoints.push_back(new_point);
}

void Drive::point_pid(double speed){
  set_point_pid(speed);
  wait_drive();
}

// Set point PID
void Drive::set_point_pid(double speed) {

  // If there are no points to drive to
  if(targetPoints.empty())
    return;

  // If the user has inputed a speed
  if(speed!=-1){
    for(short i=0; i<targetPoints.size(); i++){
      point_pid_build pnt = targetPoints.at(i);
      pnt.speed=speed;
      targetPoints.at(i)=pnt;
    }
  }

  update_point_pid_conditions(targetPoints.at(0));

  // Run task
  set_mode(POINT);
}

// Set point PID
void Drive::point_pid(double x, double y, double speed, bool backwards, point_snap snap) {
  set_point_pid(x,y,speed,backwards,snap);
  wait_drive();
}

// Set point PID
void Drive::set_point_pid(double x, double y, double speed, bool backwards, point_snap snap) {
  add_point(x,y,speed,backwards,snap);
  set_point_pid();
}

void Drive::update_point_pid_conditions(point_pid_build pnt){
  // Set Max Speed
  set_max_speed(pnt.speed);

  // Set Snappyness of robot to line
  switch(pnt.snap){
    case SNAP:
      fwdScale=1;
      break;
    case FIT:
      fwdScale=5;
      break;
    case LOOSE:
      fwdScale=10;
      break;
  }
  fwdScale=fwdScale*0.0055556+0.944444; 

  switch (pnt.transition){
    case BEFORE_STRAIGHT:
      pointPIDSwitch=20;
      break;
    case BEFORE_TURN:
      pointPIDSwitch=7;
      break;
    case LAST_POINT:
      pointPIDSwitch=7;
      break;
  }
} 

void Drive::drive_to_point(double x, double y, double turn_speed, double drive_speed){
    point2 target(x,y);
    double globalTargetTheta = util::radToDeg(util::calcAngle(chassis.odom.pos,target)); // Determine Basic Target Angle
    double thetaError = util::wrapAngle(globalTargetTheta-chassis.odom.imu.get_rotation()); // Account for 360 deg -> 0 deg overlap

    turn_pid(chassis.odom.imu.get_rotation()+thetaError,turn_speed);

    point_pid(target.x,target.y,drive_speed);
}

void Drive::set_drive_to_point(double x, double y, double turn_speed, double drive_speed){
    point2 target(x,y);
    double globalTargetTheta = util::radToDeg(util::calcAngle(chassis.odom.pos,target)); // Determine Basic Target Angle
    double thetaError = util::wrapAngle(globalTargetTheta-chassis.odom.imu.get_rotation()); // Account for 360 deg -> 0 deg overlap

    turn_pid(chassis.odom.imu.get_rotation()+thetaError,turn_speed);

    set_drive_pid(util::calcHypo(chassis.odom.pos,target),drive_speed);
}
