#include "main.h"
#include <string>
#include<iostream>
using namespace std;
#define stringify( name ) #name

/*-----------------------------------------------------------------------------
   _____            _             _ _              _      _
  / ____|          | |           | | |            | |    (_)
 | |     ___  _ __ | |_ _ __ ___ | | | ___ _ __   | |     _ _ __   ___
 | |    / _ \| '_ \| __| '__/ _ \| | |/ _ \ '__|  | |    | | '_ \ / _ \
 | |___| (_) | | | | |_| | | (_) | | |  __/ |     | |____| | | | |  __/
  \_____\___/|_| |_|\__|_|  \___/|_|_|\___|_|     |______|_|_| |_|\___|

Created on 9/30/2022
Last updated on 10/6/2022

Prints information to a given controller line
Works as the face of the controller algorithm; only printing and not storing

-----------------------------------------------------------------------------*/

bool ctrlerDebugOn=false;
void enableCntrlDebug(){
  ctrlerDebugOn=true;
}

// ----------------- PRINT ROW ----------------- //
// Prints text on a given row and at a given start point
void printRow(int row, int startPoint, std::string str){
    controller.set_text(row,0,"                                 "); // Reset the screen
    pros::delay(50);  // Wait for screen to update
    controller.set_text(row,startPoint, str);
}

// ----------------- PRINT ROW ----------------- //
// Print text on a row starting at the far left
void printRow(int row, std::string str){
  printRow(row, 0, str);
}

// ----------------- PRINT ROW CENTER ----------------- //
// Print text in the middle of the Controller line
void printRowCenter(int row, std::string str){
  int midText=str.length()/2; // Find middle of text
  int startPoint=9-midText; // Find place to put text
  printRow(row, startPoint, str);
}

	// i automas = static_cast<autoTitles>(std::floor(autoSwitch.get_value()));

void extraButtons(){
  if(ctrlerDebugOn){
    if(controller.get_digital(DIGITAL_A)){
      skillsRoller(-50,0,400,-50);
    }

    if(controller.get_digital(DIGITAL_Y)){
    point2 reset_x=quickReset(); // Get distance away from wall
    chassis.odom.setX(2+reset_x.x); // Reset robot X
    }
  }
}