#pragma once
/** @file Autonomous-Functions/api.hpp
  * @brief All functions exclusive for Autonomous
  *
  * Three main functions only used for autonomous : Auto Endgame, Quick Reset, Quick Roller
  * 
  * Auto Endgame - Deploys the endgame expansion at the very last second of endgame. This is useful if skills is a bit slower than expected 
  * Quick Reset - Resets Odometry position based on distance sensor readings against the wall. Used in skills when claiming rollers 
  * Quick Roller - Match and Skills spinning rollers code 
  * 
  * @author Logan Daugherty
*/

#include "Autonomous-Functions/End-Game.hpp" // End Game
#include "Autonomous-Functions/Quick-Reset.hpp" // Quick Reset
#include "Autonomous-Functions/Quick-Roller.hpp" // Quick Roller
