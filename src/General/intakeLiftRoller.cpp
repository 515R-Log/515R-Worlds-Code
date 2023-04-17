#include "General/intakeLiftRoller.hpp"
#include "pros/misc.h"

#include "debug/controller.hpp"

/*-----------------------------------------------------------------------------
  _____       _        _               __    _      _  __ _          __    _____       _ _
 |_   _|     | |      | |             / /   | |    (_)/ _| |        / /   |  __ \     | | |
   | |  _ __ | |_ __ _| | _____      / /    | |     _| |_| |_      / /    | |__) |___ | | | ___ _ __
   | | | '_ \| __/ _` | |/ / _ \    / /     | |    | |  _| __|    / /     |  _  // _ \| | |/ _ \ '__|
  _| |_| | | | || (_| |   <  __/   / /      | |____| | | | |_    / /      | | \ \ (_) | | |  __/ |
 |_____|_| |_|\__\__,_|_|\_\___|  /_/       |______|_|_|  \__|  /_/       |_|  \_\___/|_|_|\___|_|

Created on 5/20/2022
Last Updated on 3/10/2022

Sets up the subsystems of intake lift and roller
Due to all of these subsystems running off of the same motor, they are run from the same file as well
Contains general commands for these subsystems that are utilized in both driver and autonomous

-----------------------------------------------------------------------------*/

// ---------------- SUBSYSTEMS ---------------- //
pros::Motor rollerMtr  (21, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES);
pros::ADIAnalogIn clog_sensor ('E');
// pros::Motor rollerMtr  (16, pros::E_MOTOR_GEARSET_18, false, pros::E_MOTOR_ENCODER_DEGREES); // OLD ROBOT

// ---------- SET INTAKE / LIFT / ROLLER ---------- //
void setSubsystem(double power){
  rollerMtr=power; // Set the motor based on given power (-127 to 127)
}

// ----- SET INTAKE ----- //
void setIntake(double power){
  setSubsystem(power); // Ports over to main subsystem with given power
}

// ----- SET LIFT ----- //
void setLift(double power){
  setSubsystem(power); // Ports over to main subsystem with given power
}

// ----- SET ROLLER ----- //
void setRoller(double power){
  setSubsystem(power); // Ports over to main subsystem with given power
}

bool indexing=false;

// ---------------- SET INTAKE & LIFT & ROLLER MOTOR ---------------- //
// Set up driver input to motor
void setRollerMotor(){

  // Place-holder condition for four disc detection during driver control
  bool fullLoad=false;

  // Define the speed of the Lift
  int power;

  // Set the speed based on driver input
  power=127*(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)&&!controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)&&!fullLoad)-127*(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)&&!controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2));
  power-=127*(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)&&!controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)&&fullLoad);
  power+=1000*(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1)&&controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2));

  // Apply the velocity of the the Lift
  setRoller(power);
}

// ---------------- ANTI-JAM LIFT ---------------- //
// Prevent Jamming by reviewing intake velocity
// If the velocity is 0, then the intake is jammed
// Reverse the intake for a set amount of time and resume

// Define different modes of lifting task
enum lMode {L_DISABLE=0, L_INIT=1, L_NEUTRAL=2, L_DETECT=3, L_REMOVE=4, L_RESET=5};
enum lDetect {L_DETECT_TOP=400, L_DETECT_BOTTOM=200};
enum lRemove {L_REMOVE_TOP=300, L_REMOVE_BOTTOM=100};

lMode liftMode = L_DISABLE; // Set the default mode as disabled
lDetect lift_detect = L_DETECT_TOP;
lRemove lift_removal_time = L_REMOVE_TOP;
short liftTime = 0; // Declare the intake time as 0 

// Create a function to simplify switching modes of lift task
void updateLiftMode(lMode newMode){
  liftTime=0; // Reset time
  liftMode = newMode; // Update mode
}

// Function runs once for each iteration of the task it runs in
void antiJamAsyncIterate(){
  // If this aync function has been enabled
  if(liftMode!=L_DISABLE){

    bool detected = clog_sensor.get_value()<2300 || rollerMtr.is_over_current();

    liftTime+=20; // Add 20ms to the current time of lifitng

    switch (liftMode){

      // ----- INIITIALIZE -- 300ms for Confidence -- //
      case L_INIT:
      if(liftTime>300)
        liftMode = L_NEUTRAL; // Update mode

      // ----- DETECT JAM -- 0ms for Confidence- //
      break; case L_NEUTRAL:
      if(detected){
        
        lift_detect=L_DETECT_TOP;
        lift_removal_time=L_REMOVE_TOP;

        if(rollerMtr.is_over_current()){
          lift_detect=L_DETECT_BOTTOM;
          lift_removal_time=L_REMOVE_BOTTOM;
        }
        updateLiftMode(L_DETECT); // Begin spinning intake in reverse
      }

      // ----- DETECTING DISC -- Time may vary -- //
      break; case L_DETECT:
      if(!detected)
        liftMode = L_NEUTRAL; // Update mode
      else if(liftTime>lift_detect)
        updateLiftMode(L_REMOVE); // Start intake up again

      // ----- REMOVE DISC -- 200ms until Complete -- //
      break; case L_REMOVE:
      setLift(-127); // Spin in reverse

      if(lift_detect==L_DETECT_BOTTOM && rollerMtr.is_over_current() && liftTime>=20)
        liftTime-=20;

      else if(liftTime>lift_removal_time)
        updateLiftMode(L_RESET); // Start intake up again

      // ----- RESTART INTAKE -- 400ms until Complete -- //
      break; case L_RESET:
      if(liftTime>400)
        updateLiftMode(L_NEUTRAL); // Reset mode to the default setting
      break;
    }
  }

  // If the function is disabled an the timer is not reset
  else if(liftTime!=0)
    liftTime=0; // Reset the timer
}

// ---------------- ANTI-FOUR STACK ---------------- //
// Prevents robot from holding four discs for longer than 1.5 seconds
// If the robot contains a fourth disc. It immediately shoots a disc out of the robot
// Runs during autonomous control and is usually disabled when scoring

// enum iMode {I_NEUTRAL=0, I_THREE=1, I_REMOVE_2LEFT=2, I_REMOVE_3LEFT=3, I_DISABLE=4};
iMode intakeMode = I_DISABLE; // Set the default mode of anti-four stack to disabled

int intakeTime=0; // Declare the confidence time of a three stack

// ----- SETUP ----- //
const short fourStackDistMax = 230; // Four Stack Distance Threshold (Ranges from 230mm to 243mm)
const short threeStackDistMax = 255; // Three Stack Distance Threshold (Ranges from 247mm to 260mm)
const short distIntakeThsh = 94; // Intake Disc Detection ( Distance under 95mm means disc found )


// Iterate the anti-four stack function
void antiFourAsyncIterate(){
  // If the anti-four stack is not disabled
  if(intakeMode!=I_DISABLE){

    // ---------- CHECK THREE DISCS ----------- //
    // If there were not three discs in the flyhweel tray 20ms ago
    switch (intakeMode){
      case I_NEUTRAL:
      // Detect three discs in the flywheel tray
      if(distStackBack.get()<threeStackDistMax)
        intakeTime+=20; // Add to the confidence timer 
      
      // If there are not three discs in the flywheel tray
      else
        intakeTime=0; // Reset the timer

      // If there have been discs in the flywheel tray for 150ms
      if(intakeTime>150)
        intakeMode=I_THREE; // Confidently say there are three discs in the tray 

      // ---------- DETECT FOURTH DISC ----------- //
      // Are there 3 discs in the robot? If so...
      break;  case I_THREE:
    
      // If a new disc is found in the intake,
      if(distIntake.get()<distIntakeThsh)

        // Remove one from the tray and pick this new one up
        intakeMode=I_REMOVE_2LEFT;
      
      // If a fourth disc is found in the robot,
      else if(distStackBack.get()<fourStackDistMax-2)

        // Remove one disc from the robot.
        intakeMode=I_REMOVE_3LEFT;
      
      break;
    }

    // ---------- REMOVE FOURTH DISC ----------- //
    // If the robot wants to remove a disc
    if(intakeMode==I_REMOVE_2LEFT || intakeMode==I_REMOVE_3LEFT){
      setIntake(-127); // Remove a disc
    }

    // ---------- WAIT FOR THREE REMAIN ----------- //
    // If we want 2 discs left and the distance sensor verifies there are 0-2,
    if( (intakeMode==I_REMOVE_2LEFT && (distStackBack.get()>threeStackDistMax-10 || distStackBack.get()<(200-2))) 
        || // OR, if we want 3 discs left and the distance sensor verifies there are 0-3, 
        (intakeMode==I_REMOVE_3LEFT && (distStackBack.get()>fourStackDistMax || distStackBack.get()<(200-2))) ){

      // Reset disc-checking variables
      intakeMode=I_NEUTRAL;
      intakeTime=0;
    }
  }

  // If the task is disabled and the time is not reset
  else if (intakeTime!=0)
    intakeTime=0; // Reset the time
}

// ---------------- INTAKE/LIFT ASYNC TASK ---------------- //
// Main hub for function tasks of four-stack-detection and anti-jamming
// Sets a default speed of forwards
void liftAsyncIterate(){
  if(liftMode!=L_DISABLE || intakeMode!=I_DISABLE){
    setLift(127); // Set Intake to Spin Forwards
    antiFourAsyncIterate(); // Detect 4 Discs in Robot
    antiJamAsyncIterate(); // Detect Jamming in Robot
  }
}

// Enable lift tasks
void enableAutoIntake(){
  liftMode=L_INIT;
  intakeMode=I_NEUTRAL;
}

// Disable lift tasks
void disableAutoIntake(){
  liftMode=L_DISABLE;
  intakeMode=I_DISABLE;
}