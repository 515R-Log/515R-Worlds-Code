#include "Autonomous-Functions/Quick-Roller.hpp"
#include "General/drivetrain.hpp"
#include "General/intakeLiftRoller.hpp"
/*-----------------------------------------------------------------------------
   ____        _      _        _____       _ _
  / __ \      (_)    | |      |  __ \     | | |
 | |  | |_   _ _  ___| | __   | |__) |___ | | | ___ _ __
 | |  | | | | | |/ __| |/ /   |  _  // _ \| | |/ _ \ '__|
 | |__| | |_| | | (__|   <    | | \ \ (_) | | |  __/ |
  \___\_\\__,_|_|\___|_|\_\   |_|  \_\___/|_|_|\___|_|

Created on 5/24/2022
Last Updated on 2/16/2022

Short-hand functions for spining the roller
Sets up tournament and skills autonomous roller commands

The process of scoring is as follows:
 1. Drive into roller while spinning spinning roller motor in reverse
 2. Wait for a constant time (100 for tourney / 200 for skills)
 3. Stop roller motor to lock new roller rotation

-----------------------------------------------------------------------------*/

// ----------------- QUICK ROLLER ----------------- //
// Tournanment Roller Code
// Condenses otherwise repetitive code with 1 line function call
void quickRoller(int leftSpeed, int rightSpeed, int startTime, int startSpeed){

  chassis.set_mode(DISABLE);

  setDrive(startSpeed);

  pros::delay(startTime+50);

  // Drive into roller
  quickRollerShort(leftSpeed, rightSpeed, 200);
}

// ----------------- QUICK ROLLER SHORT ----------------- //
// Alternative Tournanment Roller Code
// Useful for tournament rollers are way too easy to spin 
void quickRollerShort(int leftSpeed, int rightSpeed, int rollerTime){

  chassis.set_mode(DISABLE);

  // Drive into roller
  setDrive(leftSpeed,rightSpeed);
  setRoller(127);

  // Wait for roller to spin
  pros::delay(rollerTime);

  // Lock current roller rotation
  setDrive(0);
  setRoller(0);
}

// ----------------- SKILLS ROLLER ----------------- //
// Skills Roller Code
// Condenses otherwise repetitive code with 1 line function call
void skillsRoller(int leftSpeed, int rightSpeed, int startTime, int startSpeed){
  chassis.set_mode(DISABLE);

  setDrive(startSpeed);

  pros::delay(startTime);

  // Drive into roller
  setDrive(leftSpeed,rightSpeed);
  setRoller(127);

  pros::delay(300);

  bool stopped = false;
  short time_elapsed = 0;
  int start_time=pros::millis();

  while (start_time+350 > pros::millis() && opticalRoller.get_hue()>200){
    time_elapsed+=20;
    
    if(!stopped && time_elapsed>500){
      stopped=true;
      setRoller(0);
      time_elapsed=0;
    }
    else if(stopped && time_elapsed>50){
      stopped=false;
      setRoller(127);
      time_elapsed=0;
    }
    
    pros::delay(20);
  }

  setRoller(127);

  pros::delay(100);

  // Lock current roller rotation
  setDrive(0);
  setRoller(0);
}


// // ---------------- GET COLOR ---------------- //
// // Find the Color based on a given hue
// std::string getColor(double sensorHue){
//   // If the Hue is between 225 and 245, the color is Blue
//   if(fabs(sensorHue-235)<=10)
//     return "Blue";
//   // If the Hue is between 0 and 10, the color is Red
//   if(fabs(sensorHue-10)<=10 || fabs(sensorHue-347)<=10)
//     return "Red";

//   // If no colors are found, return "null"
//   return "null";
// }

// // ----------------- SKILLS ROLLER ----------------- //
// // Skills Roller Code
// // Condenses otherwise repetitive code with 1 line function call
// void skillsRoller(int leftSpeed, int rightSpeed, pros::Optical sensor){
//   // Drive Into Roller
//   setRoller(127);
//   setDrive(leftSpeed, rightSpeed);

//   // Declare counting systems
//   int rollerTime=0;
//   int rollerPulseTime=0;

//   short delta_time=10;

//   // While the roller is not owned and the timeout has not been reached
//   while(getColor(sensor.get_hue())!="Red" && rollerTime<500){

//     // If the roller is difficult to spin
//     if(rollerPulseTime>300){
//       // Turn off the drivetrain for 100ms
//       setDrive(0);
//       pros::delay(100);

//       // Resetart Drivetrain
//       setDrive(leftSpeed,rightSpeed);
//       rollerPulseTime=0; // Reset pulse time
//     }

//     pros::delay(delta_time); // Wait a set amount of time

//     rollerTime+=delta_time; // Add to overal time spent at roller
//     rollerPulseTime+=delta_time;
//   }

//   // Allow roller to spin extra time
//   pros::delay(320);

//   // Prevent Roller from coasting to a stop
//   setRoller(-127);
//   pros::delay(100);

//   // End Roller and Drivetrain
//   setRoller(0);
//   setDrive(0);
// }
