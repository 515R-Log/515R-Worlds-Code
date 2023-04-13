#include "Autonomous-Functions/End-Game.hpp"
#include "General/general.hpp"
#include "General/piston.hpp"
/*-----------------------------------------------------------------------------
  ______           _     _____
 |  ____|         | |   / ____|
 | |__   _ __   __| |  | |  __  __ _ _ __ ___   ___
 |  __| | '_ \ / _` |  | | |_ |/ _` | '_ ` _ \ / _ \
 | |____| | | | (_| |  | |__| | (_| | | | | | |  __/
 |______|_| |_|\__,_|   \_____|\__,_|_| |_| |_|\___|

Created on 9/24/2022
Last Updated on 2/16/2023

Deploys the end-game mechanism automatically in the final moments of each skills autonomous run
Waits until the last 50ms of the run to extend a piston out
This is useful because, if the program takes longer than expected, we will still exend at the same exact time

-----------------------------------------------------------------------------*/

// Define the time reading at the start of driver control
int startTime=-1;

// Store the length of the driver control depending on whether the robot is in skills or match play
int matchTime=60000;

// Define how many milliseconds from the end of the time the robot will deploy
int timeLeft=30;

// ----------------- IS IT OVER? ----------------- //
// Returns whether its time to deploy
// Waits until 50ms before the match ends to return true
// Until then, return false
bool isItOver(){
  return pros::millis()>startTime+matchTime-timeLeft;
}

// ----------------- END GAME DEPLOY INITIALIZE ----------------- //
// Chooses the match time based on the autonomous of the user's choice
// If Skills is chosen, the time is 1 min
// If a match play autonomous is chosen, the time is 1 min, 45 sec
void endGameDeployInit(){
  // Store the time the driver cotnrol started
  startTime=pros::millis();
}


// ----------------- END GAME DEPLOY MAIN ----------------- //
// Deploy the endgame mechanism if its time to
void endGameDeploy(){
  if(isItOver() && startTime!=-1)
    setPiston("Endgame",OUT);
}
