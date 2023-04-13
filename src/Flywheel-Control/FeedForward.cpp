#include "Flywheel-Control/FeedForward.hpp"
/*-----------------------------------------------------------------------------
  ______            _     ______                               _
 |  ____|          | |   |  ____|                             | |
 | |__ ___  ___  __| |   | |__ ___  _ ____      ____ _ _ __ __| |
 |  __/ _ \/ _ \/ _` |   |  __/ _ \| '__\ \ /\ / / _` | '__/ _` |
 | | |  __/  __/ (_| |   | | | (_) | |   \ V  V / (_| | | | (_| |
 |_|  \___|\___|\__,_|   |_|  \___/|_|    \_/\_/ \__,_|_|  \__,_|

Created on 2/20/2022
Last Updated on 2/25/2023

Source File for advanced flywheel control for close and far range scoring
Utilizes the following methods to maximize efficiency:
  - PID / Quick adjustments to small changes in target
  - Kick-off Voltage / Sets flywheel to max power when the current speed is far from its target
  - Weighted Average / Stabilizes voltage outup to prevent harsh changes

-----------------------------------------------------------------------------*/

// ---------------- FEED FORWARD ---------------- //
// Base initialization for the class
// Inputs the provided information into the PID and Weighted Average classes
FeedForward::FeedForward(const double p, const double i, const double d, const double v, const int sample_size) : stabilize(sample_size){
  setConstants(p,i,d,v);
}

// ---------------- SET CONSTANTS ---------------- //
// Sets the PID Gains and Opening for kick-off voltage
void FeedForward::setConstants(const double p, const double i, const double d, const double v){
  kP=p, kI=i, kD=d, kV=v;
}

// ---------------- SET SAMPLE SIZE ---------------- //
// Sets the Weighted average sample consideration
// The higher the number, the smoother things change
void FeedForward::setSampleSize(const int sample_size){
  stabilize.setSize(sample_size);
}

// ---------------- SET VELOCITY ---------------- //
// Sets the target RPM for the flywheel to spin to
void FeedForward::set_velocity(const double new_target){
  target = fabs(new_target);
}

// --------------- PID CONTROL --------------- //
// Determine the PID output of the flywheel control
double FeedForward::calcPID(double currRPM,const short delta_time){

  // If the Current RPM is below 0, make it zero.
  // This removes any consideration that the flywheel spinning negative
  if(currRPM<0)
    currRPM=0;

  // Find the difference between target and currentRPM and mark it as error
  int error = target-currRPM;

  // Calculate Proportional by multiplying the error by a gain factor
  double proportional = kP*error;

  // Calculate the integral term by taking the composite error values
  sumError+=error*kI*delta_time;
  if(sumError>50)
    sumError=50;

  // Store the sum as the integral term
  double integral = sumError;

  // Calculate Derivative by taking the difference of current and last error
  double derivative = kD*(error-lastError)/delta_time;

  lastError=error;

  // Find the new velocity via PID. Make sure to add on the currentRPM as well
  double result = (proportional + integral + derivative)+currRPM+100;

  // Prevent an integer overflow when converting to volts
  if(result>1000)
    result=1000;

  // Prevent motor from putting in too much juice backwards and breaking itself
  if(result<20)
    result=20;

  stabilize.addValue(result);

  return stabilize.getWeightedAverage();
}

// ---------------- ITERATE ---------------- //
// This is the function that runs each iteration of a task while loop
// Organizes all the functions to run in Feed Forward
// Returns the desired Voltage power to use
double FeedForward::iterate(double value, const int delta_time){

  // Gather basic PID output of the system
  double result=calcPID(value, delta_time);

  double error = target-value;

  // Cap the desired new rpm to 700
  if(error>kV)
    result=1000;
   
  return result; // Send out the calculated weighted average
}
