#include "General/flywheel.hpp"
#include "General/drivetrain.hpp"
#include "General/intakeLiftRoller.hpp"
#include "Flywheel-Control/shootDisc.hpp"
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

PotMgr autoSwitches2('H'); // Initialize Autonomous Selection Potentiometer

void hailMarySlow(short discs, double indexRPM, int time_out){

  int start_time=pros::millis();

  scoreState = S_WAITING;

  autoSwitches2.set_max(100);
	autoSwitches2.set_min(-50);
	autoSwitches2.set_direction(false);

  short reset_speed = 127;
  setRoller(reset_speed);

  short index_rotation = 39.8;
  short reset_rotation = autoSwitches2.get_value();

  if(indexRPM==-1)
    indexRPM=getFlywheelTarget();

  // Turn everything on the drivebase off
  motorHold();
  disableAutoIntake();

  // Delare a timeout for the given time to score
  int endTime=pros::millis()+time_out;

  // While all discs have NOT been shot and there is still time remaining
  while(distStack.get()<175 && (pros::millis()<endTime || time_out == -1)){

    // switch (scoreState)
    // {
    //   case S_WAITING:
    //   if(shooterMtr.get_velocity()>indexRPM)
    //     updateShootSettings(S_OUTWARDS,-127);
      
    //   break;  case S_OUTWARDS:
    //   if(start_rotation-index_rotation>rollerMtr.get_position())
    //     updateShootSettings(S_RESET,reset_speed);

    //   break;  case S_RESET:
    //   if(start_rotation+reset_rotation<rollerMtr.get_position())
    //     updateShootSettings(S_WAITING,reset_speed);

    //   break;
    // }

    switch (scoreState)
    {
      case S_WAITING:
      if(shooterMtr.get_velocity()>indexRPM)
        updateShootSettings(S_OUTWARDS,-127);
      
      break;  case S_OUTWARDS:
      if(distIndex.get()<90)
        updateShootSettings(S_RESET,reset_speed);

      break;  case S_RESET:
      if(start_rotation+reset_rotation<rollerMtr.get_position())
        updateShootSettings(S_WAITING,reset_speed);

      break;
    }

    // OG VERSION
    // // If the flywheel is ready to score, send the indexer to score
    // if(shooterMtr.get_velocity()>indexRPM)
    //   indexPower=-127;

    // // If the disc is about to touch the flywheel, turn off the index.
    // // This prevents the indexer to coast a second disc into the flywheel
    // if(distIndex.get()<90){
    //   indexPower=20;
    //   discScoring=true;
    // }
    // else if(distIndex.get()>96 && discScoring){
    //   discScoring=false;
    //   discCount++;
    // }

    // setRoller(indexPower);

    // timeseres+=10;
    pros::delay(20);
  }

  std::cout << "SHOOTING DONE" << endl;

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

// ----------------- HAIL MARY DRIVER ----------------- //
// Scores 1-3 disks at a given speed
// Controller input will force-exit function
// Used Exclusively for the Match Load Station
void hailMaryDrive(int tarShots, int tarSpeed){
  // Turn off Drivetrain and Lift
  motorHold();
  setLift(0);

  setFlywheel(440);

  // Loop through each desired shot
  for(int i=0; i<tarShots; i++){
    point2 driveJoystics=setDriveJoystics(); // Check controller input

    bool lastShot= (i>=tarShots-4);
    while(shooterMtr.get_velocity()+25-3*lastShot<getFlywheelTarget());
    setRoller(-127);
    waitButJoystick(230 + 100*lastShot);

    setRoller(127);
    waitButJoystick(140);
    if(fabs(driveJoystics.x)>0 || fabs(driveJoystics.y)>0){
      motorCoast();
      return;
    }
}
motorCoast();

}


// // ----------------- HAIL MARY ----------------- //
// // Scores a given number of discs
// // Uses pre-determined RPM as the target for scoring
// void hailMarySlow(short discs, double indexRPM, int time_out){
//   // Turn everything on the drivebase off
//   motorHold();
//   disableAutoIntake();

//   // Initialize disc counting variables
//   int discCount=0;
//   bool discScoring=false;

//   // Delare a timeout for the given time to score
//   int endTime=pros::millis()+time_out;

//   int timeseres=0;

//   // While all discs have NOT been shot and there is still time remaining
//   while(discCount<discs && (pros::millis()<endTime || time_out == -1)){
 
//     int indexPower=50; // Declare the power to set the Index

//     // If the flywheel is ready to score, send the indexer to score
//     if(shooterMtr.get_velocity()>indexRPM)
//       indexPower=-127;

//     // If the disc is about to touch the flywheel, turn off the index.
//     // This prevents the indexer to coast a second disc into the flywheel
//     if(distIndex.get()<90){
//       indexPower=20;
//       discScoring=true;
//     }
//     else if(distIndex.get()>96 && discScoring){
//       discScoring=false;
//       discCount++;
//     }

//     setRoller(indexPower);

//     timeseres+=10;
//     pros::delay(10);
//   }

//   std::cout << "" << endl;
//   std::cout << "" << endl;
//   std::cout << "" << endl;
//   std::cout << discScoring << endl;
//   std::cout << discCount << endl;

//   // Unlock drivetrain and return flywheel RPM back to normal
//   motorCoast();
// }