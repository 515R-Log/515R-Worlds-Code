#include "Autonomous-Functions/api.hpp"
#include "Flywheel-Control/api.hpp"
#include "General/api.hpp"
#include "Debug/controller.hpp"

#include "Autonomous-Programs/api.hpp"
/*-----------------------------------------------------------------------------
  _____         _                   __          __ _____                        _         
 / ____|       | |                /\\ \        / /|  __ \          /\          | |        
| (___    ___  | |  ___          /  \\ \  /\  / / | |__) |        /  \   _   _ | |_  ___  
 \___ \  / _ \ | | / _ \        / /\ \\ \/  \/ /  |  ___/        / /\ \ | | | || __|/ _ \ 
 ____) || (_) || || (_) |      / ____ \\  /\  /   | |           / ____ \| |_| || |_| (_) |
|_____/  \___/ |_| \___/      /_/    \_\\/  \/    |_|          /_/    \_\\__,_| \__|\___/ 

Created on 4/8/2023
Last Updated on 4/19/2023

15 Second Autonomous Program    -    55-60 Anticipated Point Gain

Best used when our alliance partner cannot spin a roller

We have two alternative Solo AWP Programs. 
  The routes differ in that one utilizes a disc from the center line

 Version 1        Version 2 (w/ auto. line disc)

 Rollers - 2      Rollers - 2
 Discs - 8        Discs - 7
 Score - 60       Score - 55

-----------------------------------------------------------------------------*/

void Awp(){

  setFlywheel(665); // Prepare flywheel to score
  disableAutoIntake();

  // ----- CLAIM BOTTOM ROLLER ----- //
  setDrive(-40,0); // Drive into roller
  pros::delay(100); // Wait for robot to reach roller
  quickRollerShort(-40,0); // Run Roller Claim function

  // ----- SCORE PRELOADS ----- //
  chassis.drive_pid(7,120,false); // Drive away from roller
  chassis.turn_pid(45,120); // Turn away from roller
  chassis.drive_pid(8.5,120); // Drive to a scoring position

  disableAutoIntake();
  chassis.turn_pid(-15-isRightSet*3,120); // Turn towards the goal

  pros::delay(400);

  hailMary(2,NO_DROPOFF); // Score preloads
  
  setFlywheel(550);

  // ----- PICK UP DISCS ----- //
  chassis.turn_pid(45, 120); // Turn towards stack of discs
  enableAutoIntake();

  setRoller(127); // Prepare lift to pick of discs

    chassis.set_pid_constants(&chassis.headingPID, 11, 0, 65, 0); // Prevent whiplash from turn
    chassis.set_drive_pid(39, 140); // Drive into discs
    chassis.wait_until(6); // wait until robot has driven 8 inches
    
    chassis.set_max_speed(22); // Slow down to collect stack of discs
    chassis.wait_drive(); // Wait for robot to complete drive movement
    chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0); // Return heading to default state


  // chassis.drive_pid(39, 40); // Drive into discs

  // ----- SCORE DISCS ----- //
  disableAutoIntake();
  chassis.turn_pid(-35-isRightSet*2,120); // Turn towards goal

  hailMary(3,NO_DROPOFF); // Score discs

  setFlywheel(568);
  setRoller(0);

  // ----- PICK UP DISCS ----- //
  chassis.turn_pid(45, 120); // Turn towards row of discs
  // enableAutoIntake();
  setRoller(127);

  // Make the drivetrain settle quicker
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 65, 0);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);

  chassis.drive_pid(79, 70); // Move through discs
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  
  disableAutoIntake();

  // ----- CLAIM TOP ROLLER ----- //
  chassis.turn_pid(-90,120); // Turn towards roller
  
  setRoller(0); // Stop lift
  chassis.set_mode(ez::DISABLE);

  quickRoller(0,-60,450,-50); // Run Roller Claim function

  // ----- SCORE PRELOADS ----- //
  chassis.drive_pid(12,120,false); // Drive away from roller

  chassis.turn_pid(-77.2,120); // Turn towards goal
  hailMary(3,NO_DROPOFF); // Score row of discs

}

void Awp_Auto_Stack(){
  // setFlywheel(470);
  // pros::delay(2000);
  // setIntake(-127);
  // pros::delay(800);
  // setIntake(0);
  // chassis.drive_pid(19,200);
  // setIntake(127);
  // chassis.drive_pid(20,40);
}