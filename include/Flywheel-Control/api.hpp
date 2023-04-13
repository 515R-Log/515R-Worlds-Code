#pragma once
/** @file Flywheel-Control/api.hpp
  * @brief Extension for our flywheel control
  * 
  * Our flywheel control consists of two main components
  *  
  * 1. Speed Adaptation - Changing Voltage sent to motor based on target RPM
  * 2. Shoot Disc - Actually indexing a disc into the flywheel with adjustments in target when needed
  * 
  * @author Logan Daugherty
*/
 
#include "Flywheel-Control/FeedForward.hpp" // Feed Forward
#include "Flywheel-Control/shootDisc.hpp" // Score Discs
