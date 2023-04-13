#include "PotMgr/PotMgr.hpp"
/*-----------------------------------------------------------------------------
  _____      _             _   _                      _
 |  __ \    | |           | | (_)                    | |
 | |__) |__ | |_ ___ _ __ | |_ _  ___  _ __ ___   ___| |_ ___ _ __
 |  ___/ _ \| __/ _ \ '_ \| __| |/ _ \| '_ ` _ \ / _ \ __/ _ \ '__|
 | |  | (_) | ||  __/ | | | |_| | (_) | | | | | |  __/ ||  __/ |
 |_|   \___/ \__\___|_| |_|\__|_|\___/|_| |_| |_|\___|\__\___|_|

Created on 1/31/2022
Last updated on 2/16/2022

Organizes sensor data by the poteniometer to be used for a wide variety of applications
This custom code allows for purposes including the following:
  - Setting max/min value of sensor
  - Crimping values to a specified degrees of rotation
 -----------------------------------------------------------------------------*/

// --------------- POTENTIOMETER MANAGER --------------- //
// Define the vaariables that will be used in the class, as well as initializing the V5 sensor through the specified port
PotMgr::PotMgr(char portChar, bool direction, int max, int min, int zeroRangeMax, int zeroRangeMin) : potentiometer(portChar){
  max_value=max;
  min_value=min;
  reversed=direction;
  zero_range_max=degrees_to_ticks(zeroRangeMax);
  zero_range_min=degrees_to_ticks(zeroRangeMin);
}


// ---------- GET RAW VALUE ---------- //
// Returns the base value from poteniomter sensor (ranges from 0 to 4085)
int PotMgr::get_raw_value(){
  return potentiometer.get_value();
}

// ---------- GET SCALE FACTOR ---------- //
// Calculates change in value per tick of degree
// Scale is found using the slope formula
double PotMgr::get_scale_factor(){
  update_scale_factor();
  return scale_factor;
}

// ---------- GET MAX DEGREES RANGE ---------- //
// Marks a specified amount of degrees as "max"
// This prevents a sudden jump from the max to min
double PotMgr::get_zero_range_max(){
  return ticks_to_degrees(zero_range_max);
}

// ---------- GET MIN DEGREES RANGE ---------- //
// Marks a specified amount of degrees as "min"
// Similar to Max Degrees, this prevents a sudden jump from the min to max
double PotMgr::get_zero_range_min(){
  return ticks_to_degrees(zero_range_min);
}

// ---------- GET MAX ---------- //
// Gets the maximum value set by the user
double PotMgr::get_max(){
  return max_value;
}

// ---------- GET MIN ---------- //
// Gets the minimum value set by the user
double PotMgr::get_min(){
  return min_value;
}

// ---------- GET DIRECTION ---------- //
// Gets the direction of rotation set by the user
// When facing the top of the sensor, True = Clockwise / False = Counter-clockwise
bool PotMgr::get_direction(){
  return reversed;
}


// ---------- SET MAX DEGREES RANGE ---------- //
// Sets the minimum value by a given value
void PotMgr::set_zero_range_max(double degrees){
  zero_range_max=degrees_to_ticks(degrees);
}

// ---------- SET MAX DEGREES RANGE ---------- //
// Sets the minimum value by a given value
void PotMgr::set_zero_range_min(double degrees){
  zero_range_min=degrees_to_ticks(degrees);
}

// ---------- SET MAX ---------- //
// Sets the maximum value by a given value
void PotMgr::set_max(double max){
  max_value=max;
}

// ---------- SET MIN ---------- //
// Sets the minimum value by a given value
void PotMgr::set_min(double min){
  min_value=min;
}

// ---------- SET DIRECTION ---------- //
// Sets the minimum value by a given value
void PotMgr::set_direction(bool dir){
  reversed=dir;
}


// ---------- DEGREES TO TICKS ---------- //
// Converts given degrees to sensor ticks
double PotMgr::degrees_to_ticks(double degrees){
  double result = degrees/360*raw_max;
  return result;
}

// ---------- TICKS TO DEGREES ---------- //
// Converts given sensor ticks to degrees
double PotMgr::ticks_to_degrees(double ticks){
  double result = ticks*360/raw_max;
  return result;
}

// ---------- UPDATE SCALE ---------- //
// Uses slope-formula to find the change in value over change in tick count
void PotMgr::update_scale_factor(){
  // Using the format of finding the slope of a line, we will find the change in Y over X
  double changeInVal = max_value-min_value; // Change in Y (change from min to max)
  double sampleCount = raw_max-(zero_range_max+zero_range_min); // Change in X (sample size)

  double result = changeInVal/sampleCount; // Calculate slope
  scale_factor=result; // Package and store scale
}
 
// ---------- GET VALUE ---------- //
// Applies scale to ticks and crimps rotation to allowable degrees
double PotMgr::get_value(){
  update_scale_factor(); // Update the scale from min to max value

  double result = get_raw_value(); // get raw data from sensor

  // If the user says the system is reversed, reverse the system
  if(reversed)
    result=raw_max-result;

  // If the value is above max, make it max
  if(result>raw_max-zero_range_max)
    result=raw_max-zero_range_max;

  // If the value is below min, make it min
  else if( result<zero_range_min)
    result=zero_range_min;

  result-=zero_range_min; // Remove the zero range offset from min

  result*=scale_factor; // Multiply raw data by scale factor

  result+=min_value;  // Add min value to system

  // Package and send out value
  return result;
}
