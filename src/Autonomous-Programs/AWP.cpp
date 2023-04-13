#include "Autonomous-Functions/api.hpp"
#include "Flywheel-Control/api.hpp"
#include "General/api.hpp"
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

  setFlywheel(585); // Prepare flywheel to score
  disableAutoIntake();

  // ----- CLAIM BOTTOM ROLLER ----- //
  setDrive(-40,0); // Drive into roller
  pros::delay(100); // Wait for robot to reach roller
  quickRollerShort(-40,0); // Run Roller Claim function

  // ----- SCORE PRELOADS ----- //
  chassis.drive_pid(6,120,false); // Drive away from roller
  chassis.turn_pid(45,120); // Turn away from roller
  chassis.drive_pid(8.5,120); // Drive to a scoring position

  disableAutoIntake();
  chassis.turn_pid(-13,120); // Turn towards the goal

  hailMary(2,NO_DROPOFF); // Score preloads
  
  setFlywheel(560);

  // ----- PICK UP DISCS ----- //
  chassis.turn_pid(45, 120); // Turn towards stack of discs
  enableAutoIntake();

  setRoller(127); // Prepare lift to pick of discs
  chassis.drive_pid(39, 40); // Drive into discs
  pros::delay(400);

  // ----- SCORE DISCS ----- //
  chassis.set_turn_pid(-33,120); // Turn towards goal
  pros::delay(200);
  disableAutoIntake();
  chassis.wait_drive();

  hailMary(3,NO_DROPOFF); // Score discs

  setFlywheel(580);
  setRoller(0);

  // ----- PICK UP DISCS ----- //
  chassis.turn_pid(43, 120); // Turn towards row of discs
  enableAutoIntake();

  // Make the drivetrain settle quicker
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 65, 0);
  chassis.set_exit_condition(chassis.drive_exit, 80, 50, 300, 150, 500, 500);

  chassis.drive_pid(78, 80); // Move through discs
  chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0);
  
  disableAutoIntake();

  // ----- CLAIM TOP ROLLER ----- //
  chassis.turn_pid(-90,120); // Turn towards roller
  
  setRoller(0); // Stop lift
  chassis.set_mode(ez::DISABLE);

  quickRoller(0,-60,450,-50); // Run Roller Claim function

  // ----- SCORE PRELOADS ----- //
  chassis.drive_pid(12,120,false); // Drive away from roller

  chassis.turn_pid(-76.2,120); // Turn towards goal
  hailMary(3,NO_DROPOFF); // Score row of discs

}

void Awp_Auto_Stack(){

}