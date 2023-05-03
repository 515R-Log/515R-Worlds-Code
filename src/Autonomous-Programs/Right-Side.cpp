#include "Autonomous-Functions/api.hpp"
#include "Flywheel-Control/api.hpp"
#include "General/api.hpp"
#include "Debug/controller.hpp"

#include "Autonomous-Programs/api.hpp"
/*-----------------------------------------------------------------------------
 _____   _         _      _          _____  _      _                             _ 
|  __ \ (_)       | |    | |        / ____|(_)    | |               /\          | |
| |__) | _   __ _ | |__  | |_      | (___   _   __| |  ___         /  \   _   _ | |_  ___
|  _  / | | / _` || '_ \ | __|      \___ \ | | / _` | / _ \       / /\ \ | | | || __|/ _ \
| | \ \ | || (_| || | | || |_       ____) || || (_| ||  __/      / ____ \| |_| || |_| (_) |
|_|  \_\|_| \__, ||_| |_| \__|     |_____/ |_| \__,_| \___|     /_/    \_\\__,_| \__|\___/ 
             __/ |
            |___/

Created on 4/7/2023
Last Updated on 4/18/2023

15 Second Autonomous Program    -    50-55 Anticipated Point Gain

Best used when our alliance partner who excells in Left Side Autonomous  

We have two alternative Right-side Support Programs. 
  In our second version, the robot collects discs from the Autonomous Line.

 Version 1        Version 2 (w/ auto. line disc)

 Rollers - 1      Rollers - 1
 Discs - 9        Discs - 8
 Score - 55       Score - 50

 Remember : Our partner also runs autonomous. Our total score could be much higher!

-----------------------------------------------------------------------------*/

void Right_Side(){
  // ----- INITIALIZE AUTONOMOUS ----- //
  setFlywheel(540); // Prepare flywheel to score

  // ----- CLAIM TOP ROLLER ----- //
  chassis.drive_pid(-21,130); // Drive towards Roller
  chassis.turn_pid(90); // Turn towards roller
  chassis.drive_pid(-6,130,false); // Drive into roller

  chassis.set_mode(ez::DISABLE);

  quickRoller(0,-30); // Run a command that spins the lift for a short time

  // ----- COLLECT DISC ----- //
  chassis.drive_pid(4,130); // Drive away from goal
  setRoller(127); // Prepare lift to pick up disc
  chassis.turn_pid(145,190); // Turn towards autonomous line disc

  enableAutoIntake();
  chassis.drive_pid(10,130); // Drive into disc

  // ----- SCORE DISCS ----- //
  chassis.drive_pid(-7.5,130,false); // Drive away from autonomus line
  chassis.turn_pid(95); // Turn towards goal
  hailMary(3); // Run a command that scores the row of discs

  disableAutoIntake();
  setRoller(0);

  // ----- COLLECT DISCS ----- //
  setFlywheel(525);
  enableAutoIntake();
  chassis.turn_pid(44,110); // Turn towards row of discs
  setRoller(127); // Prepare Intake to pick up discs

  chassis.drive_pid(61,65,false); // Drive into row of discs
  chassis.drive_pid(-17,130,false); // Drive into row of discs
  chassis.turn_pid(119); // Turn towards goal
  disableAutoIntake();

  hailMary(3); // Run a command that scores the row of discs
  setFlywheel(515); // Prepare flywheel to score

  setIntake(127);

  chassis.drive_pid(11,130);
  chassis.drive_pid(-6,130);

  chassis.turn_pid(-7); // Turn towards goal
  chassis.drive_pid(21,130);
  // chassis.drive_pid(-20,130);

  chassis.turn_pid(126.5); // Turn towards goal

  hailMary(1);
}

void Right_Side_Auto_Stack(){
    
    // ----- INITIALIZE AUTONOMOUS ----- //
    motorHold(); // Enable drivetrain locking
    setFlywheel(578); // Spin Flywheel at 580RM
    short resetAngle = -45; // Reset the Inertial Sensor

    // ----- PICK UP DISC ----- //
    setIntake(30); // Spin intake slowly
    chassis.set_drive_pid(25,130); // Drive into three stack
    chassis.wait_until(21); // Wait until the robot is right in front of the stack
    // enableAutoIntake(); // Enable 4 disc detection
    setIntake(127);

    chassis.wait_drive(); // Wait for the robot to pick up the one disc

    // ----- CLAIM RIGHT ROLLER ----- //
    chassis.drive_pid(-5,100); // Drive away from the autonomous line
    chassis.turn_pid(-102-resetAngle,100); // Turn towards the field roller
    chassis.drive_pid(-17.5,100); // Drive towards the field roller

    setRoller(0); // Turn off Roller Motor
    chassis.set_mode(ez::DISABLE); // Prevent EZ Template from overriding the following
    setDrive(0,-50); // Drive backwards at the roller

    pros::delay(200); // Wait for robot to reach roller
    quickRoller(0,-40,0); // Spin Roller

    // ----- SCORE DISCS ----- //
    chassis.drive_pid(6,100); // Drive away from roller
    chassis.turn_pid(-129-resetAngle,100); // Turn away from roller
    chassis.drive_pid(13,100); // Drive to a scoring position

    chassis.turn_pid(-73.5-resetAngle,100); // Turn towards the high goal
	disableAutoIntake(); // Disable 4 disc detection
    hailMary(3,MID_DROPOFF); // Score 3 Discs

    // ----- PICK UP DISCS ----- //
    setFlywheel(535); // Set flywheel to 530RPM
    chassis.turn_pid(-132-resetAngle,100); // Turn towards the row of three discs
    // enableAutoIntake(); // Enable 4 disc detection

    setIntake(127);
    chassis.drive_pid(47,50); // Pick up row of discs
    chassis.drive_pid(-19,130,false); // Drive into row of discs

    chassis.turn_pid(-13); // Turn towards goal 
    disableAutoIntake();

    hailMary(3); // Run a command that scores the row of discs
    setFlywheel(512); // Prepare flywheel to score

    setIntake(127);

    chassis.drive_pid(11,130);

    chassis.drive_pid(-3,130);

    pros::delay(1000);

    hailMary(1);

}
