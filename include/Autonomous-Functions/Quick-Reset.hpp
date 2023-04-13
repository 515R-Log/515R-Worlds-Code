#pragma once
#include "EZ-Template/Odometry/point2.hpp"
/** @file Autonomous-Functions/Quick-Reset.hpp
  * @brief Reset odometry using perimeter walls
  *
  * Beams two lasers against the wall and detect 
  * 
  * There are two compenents to auto endgame expansions: Initialize, Wait 
  * Initialize - Set a time in which to deploy at in units of ms
  * Wait - Run a function over and over that checks if the time has been met. When it has, expand!
  * 
  * @author Logan Daugherty
*/


/** @brief Stores reset variables under a common name 
  * 
  * Very nice for expanding to multiple reset system to reset on multiple walls at the same time
  * As it turns out, we only have space to mount one reset system, so this is pretty unoptimized lol
  * 
  * Stores the following information: 
  * Given distance sensor reading | Distance between distance sensors | Distance from sensors to center of rotation 
*/
class resetSide{
public:

  // Left Distance Sensor Reading
  double leftFront;

  // Right Distance Sensor Reading
  double rightBack;

  // Distance between distance sensors
  double distanceApart;

  // Distance between distance sensors and center of rotation
  double offSetCenter;

  resetSide(); // Basic instantiation for a basic boi

  // Instantiation for all relevant information of reseting
  resetSide(double newLeftFront, double newRightBack, double newDistanceApart, double newOffsetCenter);
};

/** @brief Finds Distance and Angle from wall
  * 
  * Uses Trigonometry to calculate the distance and angle away from the wall
  * 
  * @param a
  *        Left Distance Sensor Value
  * @param b
  *        Right Distance Sensor Value
  * 
*/
point2 findDistance(double a, double b);

void findPoint(point2 sensorIndex,point2 offsets);
void setPosition(double x, double y, double theta);

void findPosition();
void test();

extern int displaySet[6];

point2 quickReset();
