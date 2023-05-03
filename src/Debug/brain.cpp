#include "Debug/brain.hpp"
#include "General/api.hpp"
/*-----------------------------------------------------------------------------
  ____            _          __  __       _
 |  _ \          (_)        |  \/  |     (_)
 | |_) |_ __ __ _ _ _ __    | \  / | __ _ _ _ __
 |  _ <| '__/ _` | | '_ \   | |\/| |/ _` | | '_ \
 | |_) | | | (_| | | | | |  | |  | | (_| | | | | |
 |____/|_|  \__,_|_|_| |_|  |_|  |_|\__,_|_|_| |_|

Created on 5/20/2022
Last Updated on 8/18/2022

Print information on the Brain Screen
The information is chosen through the main.cpp initialization function
Information printed includes...
• Position
• Motor Temperature
• Encoder Values
• Debug Information

-----------------------------------------------------------------------------*/

// ----------------- BRAIN DISPLAY POSITION ----------------- //
// Display on the Brain screen the current encoder values and Odometry position
// Print the encoder values on lines 2-4
// Print the Odometry position on lines 5-7
pros::Motor fsfs(18);

void brainPosAyncIterate(){
	pros::lcd::print(0,"515R Revision"); // Print Title on line 1
  // pros::lcd::print(1,"Speed:           %f",fsfs.get_actual_velocity()); // Print right encoder value on line 4
  pros::lcd::print(1,"Angle:           %f",chassis.imu.get_rotation()); // Print right encoder value on line 4
  // pros::lcd::print(2, "XPosition(in):  %f", chassis.odom.getX()); // Print X position on line 5
  // pros::lcd::print(3, "YPosition (in): %f", chassis.odom.getY()); // Print Y position on line 6
  // pros::lcd::print(4, "Angle:          %f", chassis.odom.getTheta()); // Print theta on line 7
}
