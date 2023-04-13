#pragma once
/** @file Debug/api.hpp
  * @brief Debugging Display Options
  *
  * We use the following following mediums of debugging
  * 
  * 1. Brain - LCD Emulator Display on the brain
  * 2. Computer - Print to terminal on computer. Can be applied for Live Graphs and Live Field Displays
  * 3. Controller - LCD Display on controller. Used for showing autonomous selection before a match
  * 
  * @author Logan Daugherty
*/

#include "Debug/brain.hpp" // Brain Display
#include "Debug/computer.hpp" // Computer Display
#include "Debug/controller.hpp" // Controller Display
#include "Debug/motors.hpp" // Motor Disconnect
