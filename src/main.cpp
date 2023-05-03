#include "main.h"
#include "PotMgr/PotMgr.hpp"
#include <iostream>
#include <string>
#define stringify( name ) #name

/*-----------------------------------------------------------------------------
  __  __         _
 |  \/  |  __ _ (_) _ __
 | |\/| | / _` || || '_ \
 | |  | || (_| || || | | |
 |_|  |_| \__,_||_||_| |_|

Created on 5/15/2022
Last Updated on 3/20/2023

The Main function initializes and runs our code.
It sets up autonomous, driver control, and everything competiton.
Main is the most important program because it expands to all others.

-----------------------------------------------------------------------------*/

PotMgr autoSwitches1('H'); // Initialize Autonomous Selection Potentiometer
pros::ADIAnalogIn clogf_sensor ('E');

// ---------------- TASK ---------------- //
// Iterates all functions that are designed to run in the background
// Does not include the task of EZ-Template, to prevent unforseen merging issues
void taskFunctions(){
	int time_elapsed=0; // Declare the time in usable values

	pros::delay(3000);

	while(true){ // Loop Infinitely

		if(time_elapsed%300==0){ // For every 300ms

			// The following code will be toggleable at the competition
			if(ctrlerDebugOn){ // When Controller Debug is enabled

				// Print the currently-selected autonomous program to the controller
				int automas = std::floor(autoSwitches1.get_value()); // Get index of current autonomous option
				autoTitles currentMode = static_cast<autoTitles>(automas); // Store autonomous option with 'AutoTitles' data type
				printRow(1,stringify(currentMode)); // Print out the name of the autonomous 

				
			}

			brainPosAyncIterate();

		}

		liftAsyncIterate();

		setShooterAsyncIterate(); // Flywheel Feed-Forward and PID 

		detect_motor_disconnect();

		// Wait 20ms for sensor values to update
		time_elapsed+=20;
		pros::delay(20);
	}
}

pros::Task runTasks(taskFunctions); // Instantiate Task

// ---------------- INITIALIZE ---------------- //
// Sets up all sensors, motors and tasks
void initialize() {
	runTasks.suspend(); // Turn off task for now
	shooterMtr.set_voltage(0); // Reset Flywheel voltage output

	// Set up Potentiometer
	autoSwitches1.set_max(8.9);
	autoSwitches1.set_min(1);
	autoSwitches1.set_direction(false);

	initializeGeneral(); // Callibrate Sensors
}


// ---------------- DISABLED ---------------- //
// Period of program when robot is disabled
void disabled() {
	runTasks.suspend(); // Turn off task for now
}


// ---------------- COMPETITION INITIALIZE ---------------- //
// Initialize sensors and others at the start of a comp
// Don't start autonous until the encoders are callibrated
void competition_initialize() {}

// ---------------- AUTONOMOUS ---------------- //
// The robot drives on its own
// Starts autnomous from a chosen autonomous
void autonomous() {

	runTasks.resume(); // Enable useful tasks

	// Run initialization code for autonomous
	initializeAutonomous();

	// Find the currently selected auto from the potentiomter selector.
  	int automas = std::floor(autoSwitches1.get_value());

  	// Check through each possible autonomous to find the correct one
  	switch (automas) {

    // ------ SKILLS AUTONOMOUS ----- //
    case 1:
		chassis.enable_odometry();
		// chassis.set_exit_condition(chassis.turn_exit,  0, 3,  500, 7,   500, 500);
		// chassis.set_exit_condition(chassis.drive_exit, 20,  50, 300, 150, 500, 500);
    	Skills();
      	break;

    // ----- SKILLS AUTONOMOUS VARIENT ----- //
    case 2:
      	Skills();
      	break;

    // ----- WIN POINT ----- //
    case 3:
		chassis.set_exit_condition(chassis.turn_exit,  30, 3,  500, 7,   500, 500);
      	Awp();
      	break;

    // ----- WIN POINT VARIENT ----- //
    case 4:
	// chassis.set_exit_condition(chassis.turn_exit,  0, 3,  500, 7,   500, 500);
	chassis.set_exit_condition(chassis.drive_exit, 20,  50, 300, 150, 500, 500);
	chassis.enable_odometry();

      	Awp_Auto_Stack();
      	break;

    // ----- LEFT SIDE SUPPORT ----- //
    case 5:
      	Left_Side();
      	break;

    // ----- LEFT SIDE SUPPORT VARIENT ----- //
    case 6:
      	Left_Side_Auto_Stack();
      	break;

    // ----- RIGHT SIDE SUPPORT ----- //
    case 7:
	chassis.set_exit_condition(chassis.turn_exit,  30, 3,  500, 7,   500, 500);

      	Right_Side();
      	break;

    // ----- RIGHT SIDE SUPPORT + AUTO STACK ----- //
    case 8:
      	Right_Side_Auto_Stack();
      	break;
  }
}

// ---------------- DRIVER CONTROL ---------------- //
// Robot is driven by a human
// Run the robot based on joystick and button inputs
// Display Odometry details
void opcontrol() {

	runTasks.resume(); // Enable useful tasks

	initializeDriver(); // Initialize Driver

	disableAutoIntake();

	// ----- DEBUG COMMANDS (uncomment to use) ----- //
	// enableCntrlDebug();
	// enableAutoIntake();
	// chassis.disable_odometry();
	// chassis.odom.setPosition(0,0); 
	// --------------------------------------------- //
	
	// ----- SKILLS DRIVER (uncomment to use) ----- //
	// setFlywheel(425); // Start spinnign flywheel to match load
	// pros::delay(1500); // Wait for flywheel to spin up
	// hailMaryDrive(9,100); // Begin match loading
	// if(false)
	// -------------------------------------------- // 

	// ----- SKILLS AUTONOMOUS (uncomment to use) ----- //
	// chassis.enable_odometry();
	// pros::Task runSkills(Skills);
	// while(true){
    // 	point2 driveJoystics=setDriveJoystics(); // Check controller input
	//     if(fabs(driveJoystics.x)>0 || fabs(driveJoystics.y)>0){
    //   		motorCoast();
    //   		break;
    // 	}
	// 	pros::delay(10);
	// }
	// runSkills.suspend();
	// runSkills.remove();
	// if(false)
	// ------------------------------------------------ //

	// ----- Initialize Defector Pt.1 ----- //
	setPiston("Deflector",UP); // Send piston upwards 
	bool deflectorInit=false; // Determine if the deflector has been initialized
	short deflectorInitTime=0; // Declare amount of time that has been elapsed in initialization


	while (true) {
		// ----- Initialize Deflector Pt.2 ----- //
		// Ensures that the Deflector starts 
		if(!deflectorInit){ // If the deflector has not been initialized
			if(deflectorInitTime>150){
				setPiston("Deflector",DOWN);
				deflectorInit=true; 
			}
			deflectorInitTime+=10; // Add 10ms to initialization time
		}

		// ----- Drivetrain ----- //
		setDriveMotors();

		// ----- Delivery ----- //
		setShooterMotor();

		// ----- Intake/Lift/Roller ----- //
		setRollerMotor();

		// ----- Piston ----- // 
		setPiston();

		extraButtons(); // Run some testing buttons if the debug is enabled

		pros::delay(10); // Wait 10ms for sensors to update
	}
} 
