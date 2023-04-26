#include "Autonomous-Functions/api.hpp"
#include "General/api.hpp"
#include "Flywheel-Control/api.hpp"
#include "Debug/controller.hpp"

#include "Autonomous-Programs/api.hpp"
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

bool isRightSet=false;

// Steps 2-3 and 5-6
void skillsSection(){

    // ----- SETUP ----- //
    chassis.odom.setPosition(72,9,89); // Set up robot position`

    // ----- PICK UP DISCS ----- //
    setFlywheel(472); // Set Flywheel Target to 460 RPM
    setIntake(0); // Turn Intake off

    // Drive in front of stack of discs
    chassis.add_point(38.9,23.4,100,true);
    chassis.add_point(47.0,30.9,60);
    chassis.add_point(77,50,70); // Turn to stack and begin driving forwards
    chassis.set_point_pid();

    pros::delay(1500);

    // 
    
    chassis.wait_until_x(52); // Wait until the robot is just in front of the stack
    chassis.set_max_speed(20); // Slow down drivetrain for better collection

	enableAutoIntake(); // Enable 4 disc detection
    setIntake(127);
    chassis.wait_drive(); // Wait for robot to finish picking up discs

    // ----- SCORE DISCS ----- //
    chassis.set_turn_pid(129); // Turn towards goal
    pros::delay(100);
    chassis.wait_drive();

    disableAutoIntake(); // Turn off Intake
    hailMarySlow(3); // Score discs

    // ----- PICK UP DISCS ----- //
    setFlywheel(487); // Set Flywheel Target to 487 RPM
    enableAutoIntake(); // Turn on intake

    // Drive through row of discs
    chassis.add_point(91.7,60,60); // First disc location
    chassis.add_point(118,80.4,50); // Final disc location
    chassis.point_pid(); // Run route

    // ----- SCORE DISCS ----- //
    chassis.turn_pid(176); // Turn towards goal
    disableAutoIntake(); // Turn off intake

    hailMarySlow(3); // Score discs

    // ----- PICK UP DISCS ----- //
    setFlywheel(460); // Set Flywheel Target to 475 RPM
    setIntake(0);

    chassis.set_point_pid(123.2,122.6,70); // Drive through stack of discs
    chassis.wait_until_y(97); // Wait until right in front of the stack
    chassis.set_max_speed(20); // Slow down drivetrain

    // chassis.wait_until_y(99); // Wait until right in front of the stack
    enableAutoIntake(); // Turn on intake

    chassis.wait_drive(); // Wait for path to complete

    // ----- CLAIM TOP/BOTTOM ROLLER ----- //
    chassis.turn_pid(180); // Turn towards roller
    disableAutoIntake(); // Turn off intake
    setRoller(-127); // Begin shooting a disc

    if(distStack.get()<120)
        pros::delay(120); // Wait for disc to fire
    setRoller(0); // Stop indexer

    skillsRoller(-50,0,350,-50); // Claim roller
    
    // ----- RESET ----- //
    chassis.drive_pid(5,100); // Drive away from roller

    point2 reset_y=quickReset(); // Get distance from wall
    chassis.odom.setTheta(-187.3+chassis.odom.getTheta()); // Reset robot angle
    chassis.odom.setY(11+reset_y.x); // Reset robot Y

    // ----- PICK UP DISC ----- //
    // Drive through disc and towards right roller
    chassis.add_point(125,28,80,false,LOOSE); // Disc Location
    chassis.add_point(110.1,31.3,80,false,LOOSE); // Disc Location
    chassis.add_point(102.5,33.8,80,false,LOOSE); // Roller Location
    chassis.set_point_pid(); // Run path
    
    chassis.wait_until_y(27); // Wait until the robot is sufficiently away frommer
    setIntake(127); // Turn on intake
    chassis.wait_drive(); // Wait for path to complete
    
    // ----- CLAIM RIGHT/LEFT ROLLER ----- //
    pros::delay(20); // Wait for robot to update settle params
    chassis.turn_pid(90); // Turn towards roller
    setIntake(0); // Turn off intake

    skillsRoller(-50,0,450,-40); // Claim roller
    // quickRollerShort(-50,0,100);

    // ----- RESET ----- //
    // chassis.drive_pid(5,100); // Drive away from roller
    point2 reset_x=quickReset(); // Get distance away from wall
    chassis.odom.setX(2+reset_x.x); // Reset robot X
}

// Main Program
void Skills(){

    float end_time = 60-pros::millis()/1000.0; // Find out the time in which the program ends

    // ----- INITIALIZE AUTONOMOUS ----- //
    motorHold(); // Lock drivetrain, when braking
	setFlywheel(457); // Start spinnign flywheel to match load
	pros::delay(1500); // Wait for flywheel to spin up

    short start_time = pros::millis();

    // ----- SCORE DISCS ----- //
	hailMaryMatchLoad(10000000); // Match load discs until the tray is empty.

    printRowCenter(0,to_string(pros::millis()-start_time));


    // ----- COLLECT/SCORE DISCS ----- //
    skillsSection(); // Run steps 2 and 3

    // ----- SCORE DISCS ------ //
    setFlywheel(457); // Set Flywheel Target to 420 RPM

    // SEE IF MATCH AUTONOMOUS IS STILL WORKING

    // Generate and run (X Y) path to the match load station
    chassis.add_point(38,22); // Out of the way of rollers and walls
    chassis.add_point(64,16.5); // First contact with wall
    chassis.point_pid(73,16); // Resulting Location

    chassis.turn_pid(89-7.3-2+0.7,130); // Turn towards goal

	hailMaryMatchLoad(10000000);

    skillsSection(); // Run steps 5 and 6

    // ----- SCORE DISCS ----- //
    // If there is at least 7 seconds left of the run
    if(pros::millis()/1000.0 < end_time-7){
        setFlywheel(479); // Set the flywheel to shoot close to the goal
        setIntake(127);
        chassis.add_point(27,50,90); // Drive close to the goal
        chassis.point_pid(26.7,70,90); // Drive close to the goal
        
        chassis.turn_pid(-1); // Turn towards goal

        // setR

        hailMarySlow(3); // Score discs

        chassis.point_pid(24.8,34.4,127,true); // Drive to corner of the field


    }
    else{
        chassis.point_pid(24.8,34.4,127,false); // Drive to corner of the field
    }

    // // If there at least 4 Seconds left of the run
    // else if(pros::millis()/1000.0 < end_time-4){
    //     setFlywheel(600); // Set the flywheel to shoot far from the goal
    //     chassis.turn_pid(-3); // Turn towards 
    //     setIntake(-127);

    //     pros::delay(1500);
    // }

    // ----- DEPLOY ENDGAME ----- //
    chassis.turn_pid(45); // Turn to the opposite corner of the field
    setPiston("Endgame",OUT);
}
