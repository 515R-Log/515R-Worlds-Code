#include "Autonomous-Functions/api.hpp"
#include "Flywheel-Control/api.hpp"
#include "General/api.hpp"
/*-----------------------------------------------------------------------------
 _             __  _          _____  _      _                             _         
| |           / _|| |        / ____|(_)    | |               /\          | |        
| |      ___ | |_ | |_      | (___   _   __| |  ___         /  \   _   _ | |_  ___  
| |     / _ \|  _|| __|      \___ \ | | / _` | / _ \       / /\ \ | | | || __|/ _ \ 
| |____|  __/| |  | |_       ____) || || (_| ||  __/      / ____ \| |_| || |_| (_) |
|______|\___||_|   \__|     |_____/ |_| \__,_| \___|     /_/    \_\\__,_| \__|\___/

Created on 4/6/2023
Last Updated on 4/18/2023

15 Second Autonomous Program    -    50-55 Anticipated Point Gain

Best used when our alliance partner who excells in Right Side Autonomous  

We have two alternative Left-side Support Programs. 
  The difference between this two routes is from one using a disc from the auto line

 Version 1        Version 2 (w/ auto. line disc)

 Rollers - 1      Rollers - 1
 Discs - 9        Discs - 8
 Score - 55       Score - 50

 Remember : This does not take our partner's score into account!

-----------------------------------------------------------------------------*/

void Left_Side(){    
    // ----- INITIALIZE AUTONOMOUS ----- //
    motorHold();
    setFlywheel(570);

    // ----- CLAIM BOTTOM ROLLER ----- //
    quickRoller(-50,0); // Run Roller Claim function

    // ----- SCORE DISCS ----- //
	enableAutoIntake(); // Enable 4 disc detection
    chassis.swing_pid(RIGHT_SWING,-35,130); // Turn away from roller
    chassis.drive_pid(3.5,130,false); // Drive towards autonomous line disc 
    
    chassis.drive_pid(-2,130,false); // Drive away from autonomous line
    chassis.turn_pid(57,130); // Turn to a scoring position
    disableAutoIntake(); // Disable 4 disc detection

    setIntake(0);
    chassis.drive_pid(18,130,false); // Drive towards scoring position
    chassis.turn_pid(-15,110); // Turn towards goal

    hailMary(3, MID_DROPOFF); // Score 3 discs

    // ----- PICK UP DISCS ----- //
    setFlywheel(550); // Set flywheel to 590RPM
    chassis.turn_pid(45,120); // Turn to 54 degrees
    enableAutoIntake(); // Enable 4 disc detection

    chassis.set_pid_constants(&chassis.headingPID, 11, 0, 65, 0); // Prevent whiplash from turn
    chassis.set_drive_pid(32, 100,false); // Drive into discs
    chassis.wait_until(7); // wait until robot has driven 8 inches
    
    chassis.set_max_speed(25); // Slow down to collect stack of discs
    chassis.wait_drive(); // Wait for robot to complete drive movement
    chassis.set_pid_constants(&chassis.headingPID, 11, 0, 20, 0); // Return heading to default state

    // ----- SCORE DISCS ----- //
    chassis.drive_pid(-5, 100,false); // Drive into discs

    chassis.turn_pid(-29,120); // Turn towards goal
    pros::delay(200); // Wait for disc to settle in robot
    disableAutoIntake(); // Disable four disc detection

    hailMary(3, SMALL_DROPOFF); // Score 3 discs 

    pros::delay(1000000);

    // ----- PICK UP DISC ----- // 
    setFlywheel(546); // Set flywheel target to 556RPM
    enableAutoIntake(); // Enable 4 disc detection
    chassis.drive_pid(12,90); // Pick up disc from auto line

    chassis.drive_pid(-3,90); // Drive away from auto line
    pros::delay(1500); // Wait for disc to fully enter robot
    disableAutoIntake(); // Disable 4 disc detection

    hailMary(2); // Shoot 2 discs ( there should only be one in robot but just in case :] )
    setFlywheel(900); // Set Flywheel to max speed to allow flywheel to not stop during disablement

    pros::delay(100000000); // Prevent moving on from autonomous function
}

void Left_Side_Auto_Stack(){    
    
}
