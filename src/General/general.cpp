#include "pros/misc.hpp"
#include "pros/distance.hpp"
#include "pros/optical.hpp"
#include "PotMgr/PotMgr.hpp"
/*-----------------------------------------------------------------------------
   _____                           _
  / ____|                         | |
 | |  __  ___ _ __   ___ _ __ __ _| |
 | | |_ |/ _ \ '_ \ / _ \ '__/ _` | |
 | |__| |  __/ | | |  __/ | | (_| | |
  \_____|\___|_| |_|\___|_|  \__,_|_|

Created on 5/20/2022
Last Updated on 3/10/2022

Initializes niche sensors and devices

A Complete list of all of our devices is as follows:
• Controller - 1
• Motors - 8
• Pneumatic Pistons - 2
• Distance Sensors - 5
• Inertial Sensor - 1
• Quad Encoder Sensors - 2
• Potentiometer - 1

External Sensor Count: 9
Total Sensor Count: 13

-----------------------------------------------------------------------------*/

const short MAX_MTR_POWER = 127;

// ---------------- CONTROLLER ---------------- //
// Controller                                   //
pros::Controller controller(pros::E_CONTROLLER_MASTER);

// ---------------- DISTANCE ---------------- //
pros::Distance distIndex (14);
pros::Distance distStack (20);
pros::Distance distStackBack (18);

pros::Distance distIntake (1);

pros::Distance distWallLeft (12);
pros::Distance distWallRight (8);

pros::Optical opticalRoller(17);

// ---------------- POTENTIOMETER ---------------- //
// PotMgr autoSwitch('H',true,8.9,1);

// ---------------- OPTICAL SENSOR ---------------- //
// port - Optical Sensor Name
// 6   - Left Roller Color
// 5   - Right Roller Color
pros::Optical leftRollerColor  (6);
pros::Optical rightRollerColor (5);
