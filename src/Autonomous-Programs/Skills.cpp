#include "Autonomous-Functions/api.hpp"
#include "General/api.hpp"
#include "Flywheel-Control/api.hpp"
/*-----------------------------------------------------------------------------
  _____  _     _  _  _                            _         
 / ____|| |   (_)| || |              /\          | |        
| (___  | | __ _ | || | ___         /  \   _   _ | |_  ___  
 \___ \ | |/ /| || || |/ __|       / /\ \ | | | || __|/ _ \ 
 ____) ||   < | || || |\__ \      / ____ \| |_| || |_| (_) |
|_____/ |_|\_\|_||_||_||___/     /_/    \_\\__,_| \__|\___/

Created on 4/8/2023
Last Updated on 4/19/2023

Only used in the Skills Portion of Competition

60 Second Autonomous Program    -    294 Anticipated Point Total

 Rollers - 4
 Discs - 34
 Tiles - 28

 Score - 294

-----------------------------------------------------------------------------*/

// Steps 2-3 and 5-6
void skillsSection(){

    // ----- INITIALIZE ----- //
    chassis.odom.setPosition(72,9,89); // Set up robot position`

    // ----- PICK UP DISCS ----- //
    setFlywheel(460); // Set Flywheel Target to 460 RPM
	enableAutoIntake(); // Enable 4 disc detection

    // Drive in front of stack of discs
    chassis.add_point(38.9,23.4,90,true);
    chassis.point_pid(51.0,30.9,60);

    chassis.drive_to_point(77,50); // Collect stack of discs

    // ----- SCORE DISCS ----- //
    chassis.turn_pid(130); // Turn towards goal
    disableAutoIntake(); // Turn off Intake

    hailMarySlow(3); // Score discs

    // ----- PICK UP DISCS ----- //
    setFlywheel(475); // Set Flywheel Target to 475 RPM
    enableAutoIntake(); // Turn on intake

    // Drive through row of discs
    chassis.add_point(89.7,60,80); // First disc location
    chassis.add_point(115.6,80.4,70); // Final disc location
    chassis.point_pid(); // Run route

    // ----- SCORE DISCS ----- //
    chassis.turn_pid(174.4); // Turn towards goal
    disableAutoIntake(); // Turn off intake

    hailMarySlow(3); // Score discs

    // ----- PICK UP DISCS ----- //
    setFlywheel(475); // Set Flywheel Target to 475 RPM
    chassis.turn_pid(5); // Turn towards stack of discs
    enableAutoIntake(); // Turn on intake

    chassis.point_pid(120.2,124.6,65); // Drive throught stack of discs

    // ----- CLAIM TOP ROLLER ----- //
    chassis.turn_pid(180); // Turn towards roller
    disableAutoIntake(); // Turn off intake

    // Score one disc
    setRoller(-127);
    pros::delay(200);
    setRoller(0);

    skillsRoller(-50,0,350,-50); // Claim roller
    
    // ----- RESET ----- //
    chassis.drive_pid(5,100); // Drive away from roller

    point2 reset_y=quickReset(); // Get distance from wall
    chassis.odom.setTheta(-187.3+chassis.odom.getTheta()); // Reset robot angle
    chassis.odom.setY(11+reset_y.x); // Reset robot Y

    // ----- PICK UP DISC ----- //
    chassis.turn_pid(-35,130); // Turn towards disc
    enableAutoIntake(); // Turn on intake
    chassis.drive_pid(27,130); // Drive through disc

    // ----- CLAIM RIGHT ROLLER ----- //
    chassis.turn_pid(87,130); // Turn towards roller
    disableAutoIntake(); // Turn off Intake
    setIntake(0); // Ensure Intake is turned off

    skillsRoller(0,-50,450,-40); // Claim roller

    // ----- RESET ----- //
    chassis.drive_pid(5,100); // Drive away from roller
    point2 reset_x=quickReset(); // Get distance away from wall
    chassis.odom.setX(2+reset_x.x); // Reset robot X
}

// Main Program
void Skills(){

    // ----- INITIALIZE AUTONOMOUS ----- //
    motorHold();
    setFlywheel(420);
 
    hailMarySlow(9); // Score match load discs

    skillsSection(); // Run steps 2 and 3

    // ----- SCORE DISCS ------ //
    setFlywheel(420); // Set Flywheel Target to 420 RPM

    // Generate and run (X Y) path to the match load station
    chassis.add_point(38,22);
    chassis.add_point(64,15); 
    chassis.point_pid(74,15.5);

    chassis.turn_pid(89-7.3,130); // Turn towards goal

    hailMarySlow(10); // Score match load discs

    skillsSection(); // Run steps 5 and 6

}
