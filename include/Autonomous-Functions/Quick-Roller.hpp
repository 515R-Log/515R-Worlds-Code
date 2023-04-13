#pragma once
#include "pros/optical.h"
#include "pros/optical.hpp"
#include "pros/rtos.hpp"
/** @file Autonomous-Functions/Quick-Roller.hpp
  * @brief Spins Rollers during skills and match autonomous
  *
  * Uses time-based motion to spin rollers during autonomous
  * 
  * Drivetrain motors move at different speeds to prevent turning at the goal
  * 
  * The amount of time each roller is spun depends on the length to spin and the resistence of the roller
  * Since worlds has new rollers, our values be based on new rollers :)
  * 
  * @author Logan Daugherty
*/


/** @brief Spins World-Championship Roller
  * 
  * Designed for World-Competition Rollers that are set up in between the blue and red color
  * 
  * Drives into the roller at a given speed and spins the roller at max speed
  * System waits 250ms before exiting
  * 
*/
void quickRoller(int leftSpeed, int rightSpeed, int startTime=100, int startSpeed=-40);

/** @brief Spins Home-Base Roller 
  * 
  * Designed for well-used rollers that are set up in between the blue and red color
  * Because well-used rollers are easier to spin than worlds, we will store both values in different functions
  * 
  * Drives into the roller at a given speed and spins the roller at max speed
  * System waits 200ms before exiting
  * 
*/
void quickRollerShort(int leftSpeed, int rightSpeed, int rollerTime=200);

/** @brief Spins Skills Roller
  * 
  * Designed for skills rollers that start fully on the blue color
  * 
  * Drives into the roller at a given speed and spins the roller at max speed
  * System waits until the color sensor senses the roller is claimed.
  *     This allows the robot to successfully claim rollers that take longer/shorter than expected 
  * 
  * Currently, we do not use this function because time-based roller play for skills is good enough
  * 
*/
// void skillsRoller(int leftSpeed, int rightSpeed, pros::Optical sensor);
void skillsRoller(int leftSpeed, int rightSpeed, int startTime, int startSpeed);
