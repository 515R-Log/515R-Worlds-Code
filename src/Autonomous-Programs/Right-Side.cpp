#include "Autonomous-Functions/api.hpp"
#include "Flywheel-Control/api.hpp"
#include "General/api.hpp"
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
}

void Right_Side_Auto_Stack(){
    
    // ----- INITIALIZE AUTONOMOUS ----- //
    motorHold(); // Enable drivetrain locking
    setFlywheel(580); // Spin Flywheel at 580RM
    short resetAngle = -45; // Reset the Inertial Sensor

    // ----- PICK UP DISC ----- //
    setIntake(30); // Spin intake slowly
    chassis.set_drive_pid(25,130); // Drive into three stack
    chassis.wait_until(20); // Wait until the robot is right in front of the stack
    enableAutoIntake(); // Enable 4 disc detection

    chassis.wait_drive(); // Wait for the robot to pick up the one disc

    // ----- CLAIM RIGHT ROLLER ----- //
    chassis.drive_pid(-5,100); // Drive away from the autonomous line
    chassis.turn_pid(-102-resetAngle,100); // Turn towards the field roller
    chassis.drive_pid(-16,100); // Drive towards the field roller

    setRoller(0); // Turn off Roller Motor
    chassis.set_mode(ez::DISABLE); // Prevent EZ Template from overriding the following
    setDrive(0,-50); // Drive backwards at the roller

    pros::delay(200); // Wait for robot to reach roller
    quickRoller(0,-40,0); // Spin Roller

    // ----- SCORE DISCS ----- //
    chassis.drive_pid(6,100); // Drive away from roller
    chassis.turn_pid(-127-resetAngle,100); // Turn away from roller
    chassis.drive_pid(13,100); // Drive to a scoring position

    chassis.turn_pid(-72.5-resetAngle,100); // Turn towards the high goal
	disableAutoIntake(); // Disable 4 disc detection
    hailMary(3,MID_DROPOFF); // Score 3 Discs

    // ----- PICK UP DISCS ----- //
    setFlywheel(530); // Set flywheel to 530RPM
    chassis.turn_pid(-132-resetAngle,100); // Turn towards the row of three discs
    enableAutoIntake(); // Enable 4 disc detection

    chassis.drive_pid(47,90); // Pick up row of discs

    // ----- SCORE DISCS ----- //
    chassis.turn_pid(-47-resetAngle,100); // Turn to High Goal
	disableAutoIntake(); // Disable 4 disc detection
    hailMary(3,MID_DROPOFF); // Score 3 Discs

}
