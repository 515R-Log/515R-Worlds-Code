#include "sylib/sylib.hpp"
#include "General/drivetrain.hpp"
#include "General/general.hpp"
#include "General/piston.hpp"
#include "General/intakeLiftRoller.hpp"
#include "EZ-Template/api.hpp"

const int DRIVE_SPEED = 110;
const int TURN_SPEED  = 90;
const int SWING_SPEED = 90;

void default_constants() {
  chassis.set_slew_min_power(80, 80);
  chassis.set_slew_distance(7, 7);
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  chassis.set_pid_constants(&chassis.forward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.backward_drivePID, 0.45, 0, 5, 0);
  chassis.set_pid_constants(&chassis.turnPID, 5, 0.003, 35, 15);
  chassis.set_pid_constants(&chassis.swingPID, 7, 0, 45, 0);

  chassis.set_pid_constants(&chassis.pTurnPID, 5, 0.003, 35, 15);
}

void initializeGeneral(){

  ez::print_ez_template(); // Messed the EZ terminal branding, oops :)
  sylib::initialize();

  pros::delay(500); // Stop the user from doing anything while legacy ports configure.

  // Configure your chassis controls
  chassis.toggle_modify_curve_with_controller(true); // Enables modifying the controller curve with buttons on the joysticks
  chassis.set_active_brake(0); // Sets the active brake kP. We recommend 0.1.
  chassis.set_curve_default(3, 3); // Defaults for curve. If using tank, only the first parameter is used. (Comment this line out if you have an SD card!)
  default_constants(); // Set the drive to your own constants from autons.cpp!

  chassis.set_slew_distance(7, 7);
  chassis.set_slew_min_power(40, 40);

  chassis.set_joystick_threshold(5);

  // Initialize chassis and auton selector
  chassis.initialize();
  ez::as::initialize();
}



void initializeAutonomous(){

  chassis.reset_pid_targets(); // Resets PID targets to 0
  chassis.reset_gyro(); // Reset gyro position to 0
  chassis.reset_drive_sensor(); // Reset drive sensors to 0
  motorHold();
}

void initializeDriver(){
    motorCoast();

    setPiston("Deflector",DOWN); 

    disableAutoIntake();
}
