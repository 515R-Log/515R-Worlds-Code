#include "Autonomous-Functions/Quick-Reset.hpp"
#include "General/general.hpp"
#include <cmath>
/*-----------------------------------------------------------------------------
  ____               _  _
 |  _ \  ___   __ _ | |(_)  __ _  _ __
 | |_) |/ _ \ / _` || || | / _` || '_ \
 |  _ <|  __/| (_| || || || (_| || | | |
 |_| \_\\___| \__,_||_||_| \__, ||_| |_|
                           |___/
Created on 10/10/2022
Last Updated 10/14/2022

Quickly reset the Robot's position using the back side of the Robot
Uses Trigonometry functions to calculate the following:
• Theta
• Distance From Wall

-----------------------------------------------------------------------------*/

// --------------- THETA CALCULATION --------------- //
// Calculates Theta from given base and height
//             Diagram
//
//              base(known)
//           ____________ Theta(unknown)
//          |           /
//          |          /
//          |        /
//   height |      /
//  (known) |    /
//          |  /
//          |/
//
// tan(Theta) = height/base
// Theta = inverse tan(height/base)  Isolate Theta
double getThetaVal(double bVal, double hVal){
  return (atan(hVal/bVal));
}


// --------------- X/Y CALCULATION --------------- //
// Calculates X/Y from given hypotenuse and angle
//             Diagram
//           _____________
//          |            /
//          |          /
//          |        /
//   height |      / Hypotenuse (known)
// (unknown)|    /
//          |  /
//          |/
//      Theta(known)
// cos(Theta) = height/hypotenuse
// cos(Theta)*hypotenuse = height Isolate height
double getXYVal(double cVal, double BVal){
  return cVal*cos(BVal);
}

resetSide::resetSide(double newLeftFront, double newRightBack, double newDistanceApart, double newOffsetCenter){
  leftFront=newLeftFront;
  rightBack=newRightBack;
  distanceApart=newDistanceApart;
  offSetCenter=newOffsetCenter;
}

// --------------- RESET VALUES --------------- //
//  Create an array of the new position generated
//  Gather data from distance sensors
point2 findDistance(double distanceLeft, double distanceRight){

  // ----- CONVERT MM TO IN ----- //
  distanceLeft*=0.0393701;
  distanceRight*=0.0393701;

  // ----- GET VALUES ----- //
  resetSide sensorValueFront(distanceLeft,distanceRight,4.947,7.4); // Sensor Values based on
  // Apply offset from robot center
  sensorValueFront.leftFront+=sensorValueFront.offSetCenter;
  sensorValueFront.rightBack+=sensorValueFront.offSetCenter;

  // Check to ensure there is a value from each sensor
  point2 null(-10,-10,-10);
  if(sensorValueFront.leftFront==-1 || sensorValueFront.rightBack==-1)
    return null;


  // ---------- THETA VALUE ---------- //
  // ----- TRIANGLE FROM TRAPEZOID ----- //
  double baseF=sensorValueFront.distanceApart;

  double heightF = sensorValueFront.leftFront-sensorValueFront.rightBack; // Height of the triangle / Difference between larger and smaller distance values

  // ----- SOLVE TRIANGLE ANGLE / GET THETA ----- //
  double thetaVal = (getThetaVal(baseF,heightF));


  // ---------- X/Y VALUE ---------- //
  // ----- TRIANGE FOR X/Y VALUE ----- //
  double hypoF = (sensorValueFront.leftFront+sensorValueFront.rightBack)/2;

  double angF = fabs(thetaVal); // Angle of Triangle / 90 minus

  // ----- SOLVE BASE LENGTH / GET X/Y ----- //
  double newF=getXYVal(hypoF,angF);

  // ----- COMPRESS INFORMATION ----- //
  point2 result(newF,thetaVal*180/3.14159365,0);

  return result; // Send information off to program
}

point2 quickReset(){
  double d_result=0;
  double a_result=0;
  short sample_size=10;

  for(short i=0; i<sample_size; i++){
    point2 sample=findDistance(distWallLeft.get(),distWallRight.get());

    if(sample.x>50 || sample.x<6)	i--;
    else{
      d_result+=sample.x/sample_size;
      a_result+=sample.y/sample_size;
    }
    
    pros::delay(20);
  }

  point2 result(d_result,a_result);

  return result;
}