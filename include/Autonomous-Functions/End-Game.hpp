/** @file Autonomous-Functions/End-Game.hpp
  * @brief Automated Endgame Expansion
  *
  * To expand at the last possible second in skills, we use a function that does the waiting for us.
  * 
  * There are two compenents to auto endgame expansions: Initialize, Wait 
  * Initialize - Set a time in which to deploy at in units of ms
  * Wait - Run a function over and over that checks if the time has been met. When it has, expand!
  * 
  * @author Logan Daugherty
*/

/** @brief Defines the time in which to expand endgame in skills
  * 
  * Calclated using the following equation: 
  * t=c+s-r 
  * t = target time | c = current time | s = skills duration time | r = remaining time after expansion
  * t = ? | c = pros::millis() | s = 60,000ms | r = 30ms
  * 
  * Runs only once for each autonomous run
*/
void endGameDeployInit();


/** @brief Wait for expansion time
  * 
  * if the current time is equal or above the target time, run a command to expand endgame 
  * 
  * Function runs once every 20ms
*/
void endGameDeploy();
