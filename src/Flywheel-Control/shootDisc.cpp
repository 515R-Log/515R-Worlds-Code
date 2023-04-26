#include "General/flywheel.hpp"
#include "General/drivetrain.hpp"
#include "General/intakeLiftRoller.hpp"
#include "Flywheel-Control/shootDisc.hpp"
#include "Debug/controller.hpp"

/*-----------------------------------------------------------------------------
  _____  _              _____
 |  __ \(_)            / ____|
 | |  | |_ ___  ___   | (___   ___ ___  _ __ ___
 | |  | | / __|/ __|   \___ \ / __/ _ \| '__/ _ \
 | |__| | \__ \ (__    ____) | (_| (_) | | |  __/
 |_____/|_|___/\___|  |_____/ \___\___/|_|  \___|

Created on 5/24/2022
Last Updated on 3/10/2023

Primary source for scoring disks in goals
Sets up autonomous and commands to scoring disks
Contains user-selected disc count and speed to score

The process of scoring disks is as follows:
 1. Set delivery motor to 127(max)
 2. Wait until target speed is reached
 3. Send index command to score disk and wait for disk to fire
 4. Loop steps 1-3 until all disks have been scored

 Variations of this are also created for nice cases

-----------------------------------------------------------------------------*/

// ----------------- HAIL MARY ----------------- //
// Scores a given number of discs
// Uses pre-determined RPM as the target for scoring
void hailMary(short discs, drop_off dropOff, int time_out){

  // Set Flywheel target to max temporarily
  double indexRPM=getFlywheelTarget()-dropOff;
  setFlywheel(9000);

  // Use slow scoring function with our max accelleration
  hailMarySlow(discs, indexRPM, time_out);

  // Scoring Is Completed //

  // Return Flywheel target to its original value
  setFlywheel(indexRPM+dropOff);
}

    // NO_DROPOFF=0,
    // SMALL_DROPOFF=5,
    // MID_DROPOFF=12,
    // BIG_DROPOFF=28

// ----------------- HAIL MARY ----------------- //
// Scores a given number of discs
// Uses pre-determined RPM as the target for scoring
enum sState {S_WAITING=0, S_OUTWARDS=1, S_RESET=2};
sState scoreState = S_WAITING;
int start_rotation=0;

short score_time=0;

void updateShootSettings(sState new_score_state, short index_speed){
  scoreState=new_score_state;
  start_rotation=rollerMtr.get_position();
  setRoller(index_speed);
  std::cout << scoreState << endl;
}

void hailMarySlow(short discs, double indexRPM, int time_out){

  int start_time=pros::millis();

  scoreState = S_WAITING;

  short reset_speed = 80;
  setRoller(reset_speed);

  // short reset_rotation = 4;
  short reset_rotation = 100;

  if(indexRPM==-1)
    indexRPM=getFlywheelTarget();

  // Turn everything on the drivebase off
  motorHold();
  disableAutoIntake();

  // Delare a timeout for the given time to score
  int endTime=pros::millis()+time_out;

  // While all discs have NOT been shot and there is still time remaining
  while(distStack.get()<180 && (pros::millis()<endTime || time_out == -1)){

    bool last_disc = distStack.get()>150;
    bool less_than_three = distStack.get()>145;

    switch (scoreState)
    {
      case S_WAITING:
      if(shooterMtr.get_velocity()>indexRPM - last_disc*8 + less_than_three*25);
        updateShootSettings(S_OUTWARDS,-90);
      
      break;  case S_OUTWARDS:
      if(distIndex.get()<95)
        updateShootSettings(S_RESET,127);

      break;  case S_RESET:
      if(start_rotation+reset_rotation<rollerMtr.get_position())
        updateShootSettings(S_WAITING,reset_speed);

      break;
    }
    pros::delay(20);
  }

  score_time=pros::millis()-start_time;

  // Unlock drivetrain and return flywheel RPM back to normal
  motorCoast();
}



// Replace wait-times with custom wait-times
void waitButJoystick(int time){
  int countTime=0; // Declare the time that has passed in function
  while(countTime<time){

    point2 driveJoystics=setDriveJoystics(); // Check controller input
    // If controller input is detected, end wait-time early
    if(fabs(driveJoystics.x)>0 || fabs(driveJoystics.y)>0)
      return;

    // Update time passed by 10 ms
    countTime+=10;
    pros::delay(10);
  }
}

PotMgr delay_rotation('H');

// ----------------- HAIL MARY DRIVER ----------------- //
// Scores 1-3 disks at a given speed
// Controller input will force-exit function
// Used Exclusively for the Match Load Station
void hailMaryMatchLoad(int time_out){
  delay_rotation.set_max(1000);
  delay_rotation.set_min(0);
  delay_rotation.set_direction(false);

  // printRowCenter(0,to_string(delay_rotation.get_value()));

  int start_time=pros::millis();
  int endTime=pros::millis()+time_out;
  int confidence_time=0;

  scoreState = S_WAITING;

  short reset_speed = 80;
  setRoller(reset_speed);

  // short reset_rotation = 4;
  short reset_rotation = delay_rotation.get_value();

  // Turn everything on the drivebase off
  motorHold();
  disableAutoIntake();

  // While all discs have NOT been shot and there is still time remaining
  while(confidence_time<161 && (pros::millis()<endTime || time_out == -1)){

    bool last_disc = distStack.get()>150;
    bool less_than_three = distStack.get()>145;

    if(distStack.get()>175)
      confidence_time+=10;
    else if(confidence_time!=0)
      confidence_time=0;

    switch (scoreState)
    {
      case S_WAITING:
      if(shooterMtr.get_velocity()>getFlywheelTarget() - last_disc*8 + less_than_three*25);
        updateShootSettings(S_OUTWARDS,-90);
      
      break;  case S_OUTWARDS:
      if(distIndex.get()<95)
        updateShootSettings(S_RESET,127);

      break;  case S_RESET:
      if(start_rotation+reset_rotation<rollerMtr.get_position())
        updateShootSettings(S_WAITING,reset_speed);

      break;
    }
    pros::delay(20);
  }
// motorCoast();

}


// ----------------- HAIL MARY DRIVER ----------------- //
// Scores 1-3 disks at a given speed
// Controller input will force-exit function
// Used Exclusively for the Match Load Station
void hailMaryDrive(int tarShots, int tarSpeed){
  int time_out = 100000000;
  delay_rotation.set_max(1000);
  delay_rotation.set_min(0);
  delay_rotation.set_direction(false);

  // printRowCenter(0,to_string(delay_rotation.get_value()));

  int start_time=pros::millis();
  int endTime=pros::millis()+time_out;
  int confidence_time=0;

  scoreState = S_WAITING;

  short reset_speed = 80;
  setRoller(reset_speed);

  // short reset_rotation = 4;
  short reset_rotation = 40;

  // Turn everything on the drivebase off
  motorHold();
  disableAutoIntake();

  // While all discs have NOT been shot and there is still time remaining
  while(confidence_time<161 && (pros::millis()<endTime || time_out == -1)){

    point2 driveJoystics=setDriveJoystics(); // Check controller input

    bool last_disc = distStack.get()>150;
    bool less_than_three = distStack.get()>145;

    if(distStack.get()>180)
      confidence_time+=10;
    else if(confidence_time!=0)
      confidence_time=0;

    switch (scoreState)
    {
      case S_WAITING:
      if(shooterMtr.get_velocity()>getFlywheelTarget() - last_disc*8 + less_than_three*25);
        updateShootSettings(S_OUTWARDS,-90);
      
      break;  case S_OUTWARDS:
      if(distIndex.get()<95){
        updateShootSettings(S_RESET,127);
        setFlywheel(452);
      }

      break;  case S_RESET:
      if(start_rotation+reset_rotation<rollerMtr.get_position())
        updateShootSettings(S_WAITING,reset_speed);

      break;
    }

    if(fabs(driveJoystics.x)>0 || fabs(driveJoystics.y)>0){
      motorCoast();
      return;
    }
    
    pros::delay(20);
  }

  motorCoast();

}
