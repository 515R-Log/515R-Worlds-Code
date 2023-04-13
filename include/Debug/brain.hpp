#pragma once
/** @file Debug/brain.hpp
  * @brief Display Information on Brain
  *
  * Displays a set of information to the Brain 
  * Updates every 20ms in the main task update
  * 
  * @author Logan Daugherty
*/

/** @brief Displays information to Brain
  * 
  * When Debugging, we change the contents of this function to adjust the information displayed
  * Runs in the main.cpp global task
  * 
*/
void brainPosAyncIterate();
