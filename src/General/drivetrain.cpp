#include "EZ-Template/api.hpp"
#include "EZ-Template/Odometry/point2.hpp"
#include "pros/motors.hpp"
#include "General/general.hpp"

/*-----------------------------------------------------------------------------
  ____         _              _                _
 |  _ \  _ __ (_)__   __ ___ | |_  _ __  __ _ (_) _ __
 | | | || '__|| |\ \ / // _ \| __|| '__|/ _` || || '_ \
 | |_| || |   | | \ V /|  __/| |_ | |  | (_| || || | | |
 |____/ |_|   |_|  \_/  \___| \__||_|   \__,_||_||_| |_|

Created on 5/21/2022
Last Updated on 3/10/2023

Drivetrain Commands in Driver and Autonomous Periods
Sets the Speeds of the Drive Motors
Uses the Controller input

-----------------------------------------------------------------------------*/

// ---------------- DRIVETRAIN ---------------- //
// Base Motor is disabled due to inetegration of EZ Template Commands
Drive chassis(
  // EZ-Template Setup
  {-3, -2, -5}, // Left Drivetrain Motor Ports
  {7, 9, 10}, // Right Drivetrain Motor Ports
  13, // IMU Port
  3.25, // Drivetrain Wheel Diameter
  600, // Motor Cartridge
  1.3333333333, // External Drive Ratio

  // 515R Odometry Setup
  'A','C', // Top Port for Quad Encoders
  4, // 3-Wire Extender Port for Quad Encoders
  2.7359, 2.7359, // Tracking Wheel Diameters
  0.7008, -1.7525 // Tracking Wheel Offsets
);

// ----------------- DRIVETRAIN VELOCITY ----------------- //
// Set the velocity of the Drivetrain motors
// The Speed is set to the given left and right speeds, respectively
void setDrive(double left, double right){
  left*=1.05;
  right*=1.05;

  left*=1.05;
  right*=1.05;

  chassis.set_tank(left,right);
}

// Sets the velocity of the Drivetrain motors
// All Drive Motors are set to the same speed
void setDrive(double speed){
  speed*=1.05;
  chassis.set_tank(speed,speed);
}

// ---------------- SET DRIVE MOTORS ---------------- //
// Deals with all driver control commands for the drivetrain
void setDriveMotors(){
  chassis.tank(); // Use EZ Template Tank Drive
}

void motorHold(){
  chassis.set_drive_brake(pros::E_MOTOR_BRAKE_HOLD);
}

void motorCoast(){
  chassis.set_drive_brake(pros::E_MOTOR_BRAKE_COAST);
}

void driveDiscs(){
  chassis.set_exit_condition(chassis.drive_exit,10,  100, 10,  300, 50000000, 50000000);
  chassis.set_exit_condition(chassis.turn_exit, 20, 6,   50, 14,   50000000, 50000000);
}

void driveGoal(){
  chassis.set_exit_condition(chassis.drive_exit,80,50,300,150,500,500);
  chassis.set_exit_condition(chassis.turn_exit,  100, 3,  500, 7,   500, 500);
}

// ---------------- CONTROLLER DRIVETRAIN ---------------- //
// Calculates the desired velocity of the Drivetrain using the Controller
point2 setDriveJoystics(){
  // Set the velocity of each side using TANK control
  double leftPow = controller.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
  double rightPow = controller.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

  // If the speed is less than 10, brake the motors (In case we toggle brake type hold on)
  if (fabs(leftPow) < 10)
    leftPow = 0;
  if (fabs(rightPow) < 10)
    rightPow = 0;

  point2 speedSet(leftPow, rightPow, 0);
  return speedSet;
}
