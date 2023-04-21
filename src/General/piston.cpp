#include "General/piston.hpp"
#include "pros/misc.h"
#include "General/general.hpp"
#include "Flywheel-Control/shootDisc.hpp"

/*-----------------------------------------------------------------------------
 _____ _     _
|  __ (_)   | |
| |__) | ___| |_ ___  _ __
|  ___/ / __| __/ _ \| '_ \
| |   | \__ \ || (_) | | | |
|_|   |_|___/\__\___/|_| |_|

Created on 5/20/2022
Last Updated on 3/10/2022

Sets up all custom pneumatic and motorized index commands
Used in Autonomous and Driver Control
Toggles between true and false for all pistons
Uses the controller as an input for all functions

-----------------------------------------------------------------------------*/

// Declare public variables used for flyhweel control
bool deflectorUp=false;

// -------------- PNEUMATIC PISTON -------------- //
// port - Piston Name
// 'H'  - Deflector Piston
// 'G'  - Endgame Pistons
pros::ADIDigitalOut deflectorPiston ('F');
pros::ADIDigitalOut endGameShootPiston('G');

void setPiston(std::string pistonType, piston_state dir){

  // ------ DEFELCTOR ----- //
  if(pistonType=="Deflector"){
    deflectorPiston.set_value(dir);
    deflectorUp=dir;
  }

  // ----- ENDGAME ----- //
  else if(pistonType=="Endgame")
    endGameShootPiston.set_value(dir);
}

bool triggeredDeflector=false;

// ---------------- SET PISTON ---------------- //
// Driver Control Usage of Pistons
void setPiston(){

  // ----- RAPID FIRE.EXE ------ //
  // Used at the match load station to autonomously shoot 10 discs
  if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
    // rapidFireExe(); // Score match load discs
    hailMaryDrive(15,100);
  }
 
  // ----- DEFLECTOR ----- //
  if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2)  && !controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
    if(!triggeredDeflector){ // If it had not previously been pressed
      piston_state state = static_cast<piston_state>(!deflectorUp);
      setPiston("Deflector", state); // Send piston code
      triggeredDeflector=true; // Determine that the button has been pressed
    }
  }
  else
    triggeredDeflector=false;

      // ----- ENDGAME ----- //
  if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)&&controller.get_digital(pros::E_CONTROLLER_DIGITAL_X)){
    setPiston("Endgame",OUT);
  }
}
