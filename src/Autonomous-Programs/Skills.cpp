#include "Autonomous-Functions/api.hpp"
#include "General/api.hpp"
#include "Flywheel-Control/api.hpp"
#include "Debug/controller.hpp"
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
    setFlywheel(465); // Set Flywheel Target to 460 RPM
	enableAutoIntake(); // Enable 4 disc detection

    // Drive in front of stack of discs
    chassis.add_point(38.9,23.4,90,true);
    chassis.point_pid(47.0,30.9,60);

    chassis.set_drive_to_point(77,50); // Collect stack of discs
    chassis.wait_until(7);
    chassis.set_max_speed(25);
    chassis.wait_drive();

    // ----- SCORE DISCS ----- //
    chassis.turn_pid(127); // Turn towards goal
    disableAutoIntake(); // Turn off Intake

    hailMarySlow(3); // Score discs

    // ----- PICK UP DISCS ----- //
    setFlywheel(485); // Set Flywheel Target to 475 RPM
    enableAutoIntake(); // Turn on intake

    // Drive through row of discs
    chassis.add_point(91.7,60,60); // First disc location
    chassis.add_point(118.6,80.4,50); // Final disc location
    chassis.point_pid(); // Run route

    // ----- SCORE DISCS ----- //
    chassis.turn_pid(176.4); // Turn towards goal
    disableAutoIntake(); // Turn off intake

    hailMarySlow(3); // Score discs

    // ----- PICK UP DISCS ----- //
    setFlywheel(420); // Set Flywheel Target to 475 RPM
    // chassis.turn_pid(5); // Turn towards stack of discs
    enableAutoIntake(); // Turn on intake

    chassis.set_point_pid(123.2,122.6,50); // Drive throught stack of discs
    chassis.wait_until_y(100);
    chassis.set_max_speed(25);
    chassis.wait_drive();

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

    chassis.add_point(-14.9+123+2,11.3+23-3,80,false,LOOSE);
    chassis.add_point(-23.3+123+2,13.8+23-3,80,false,LOOSE);
    // chassis.add_point(-26.3,26.3,30,true);
    chassis.set_point_pid();
    chassis.wait_until_y(8);
    setIntake(127);
    chassis.wait_drive();
    pros::delay(20);
    chassis.turn_pid(90);
    setIntake(0);

    skillsRoller(-50,0,380,-40); // Claim roller

    // ----- RESET ----- //
    chassis.drive_pid(5,100); // Drive away from roller
    point2 reset_x=quickReset(); // Get distance away from wall
    chassis.odom.setX(2+reset_x.x); // Reset robot X
}

// Main Program
void Skills(){

    // ----- INITIALIZE AUTONOMOUS ----- //
    motorHold();

	setFlywheel(451);
	pros::delay(1500);
	hailMaryMatchLoad(10000000);

    skillsSection(); // Run steps 2 and 3

    // ----- SCORE DISCS ------ //
    setFlywheel(451); // Set Flywheel Target to 420 RPM

    // Generate and run (X Y) path to the match load station
    chassis.add_point(38,22);
    chassis.add_point(64,15.5); 
    chassis.point_pid(74,16);

    chassis.turn_pid(89-7.3,130); // Turn towards goal

	hailMaryMatchLoad(10000000);

    skillsSection(); // Run steps 5 and 6

    chassis.point_pid(26.7,82.7,90);

    chassis.turn_pid(-1);

    hailMarySlow(3);

    chassis.point_pid(24.8,34.4,90,true);

    chassis.turn_pid(45);

}
