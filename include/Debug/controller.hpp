#pragma once
#include <string>
/** @file Debug/controller.hpp
  * @brief Controller Debug
  *
  * Custom display and control of controller for debugging and autonomous selection
  * 
  * Contains the following features
  * 1. 
  * 
  * @author Logan Daugherty
*/

/** @brief Print to Controller
  * 
  * Simplified version of the controller screen print
  * Used mainly in the debugging function of controller
  * 
  * @param row
  *        Y Axis. Ranges from 0 to 2. 0 Is the top of the screen
  * @param startPoint
  *        X Axis. Ranges from 0 to ?. 0 Is the far left of the screen
  * @param str
  *        Text that is actually being printed
  * 
*/
void printRow(int row, int startPoint, std::string str);

/** @brief Print to Controller
  * 
  * Simplified version of the controller screen print
  * Used mainly in the debugging function of controller
  * Text is automatically positioned to the far left.
  * 
  * @param row
  *        Y Axis. Ranges from 0 to 2. 0 Is the top of the screen
  * @param str
  *        Text that is actually being printed
  * 
*/
void printRow(int row, std::string str);

/** @brief Centered Print to Controller
  * 
  * Simplified version of the controller screen print
  * Used mainly in the debugging function of controller
  * Text is automatically centered on a line
  * 
  * @param row
  *        Y Axis. Ranges from 0 to 2. 0 Is the top of the screen
  * @param str
  *        Text that is actually being printed
  * 
*/
void printRowCenter(int row, std::string str);

extern bool ctrlerDebugOn;

/** @brief Enables Controller Display
  * 
  * Switches the state of aysncronous display to on
  * 
*/
void enableCntrlDebug();

void extraButtons();