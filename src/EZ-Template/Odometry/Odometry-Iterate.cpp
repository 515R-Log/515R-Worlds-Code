#include "EZ-Template/Odometry/Odometry.hpp"
#include "EZ-Template/util.hpp"
#include "main.h"
/*-----------------------------------------------------------------------------

   ____      _                                _____ _                 _
  / __ \    | |                              |_   _| |               | |
 | |  | | __| | ___  _ __ ___       ______     | | | |_ ___ _ __ __ _| |_ ___
 | |  | |/ _` |/ _ \| '_ ` _ \     |______|    | | | __/ _ \ '__/ _` | __/ _ \
 | |__| | (_| | (_) | | | | | |_              _| |_| ||  __/ | | (_| | ||  __/
  \____/ \__,_|\___/|_| |_| |_(_)            |_____|\__\___|_|  \__,_|\__\___|

Created on 2/20/2022
Last Updated on 2/26/2023

Updates the

-----------------------------------------------------------------------------*/

// ---------------- ITERATE ODOMETRY ---------------- //
// This is the function to run in an outside while loop, to use odometry
void Odometry::iterate(){
  // Grab sensor values and convert to units inches and degrees
  tracking new_rotation(fwd.get()*3.14159265359*wheelDiam.fwd/360, back.get()*3.14159265359*wheelDiam.back/360, getTheta());
  update(new_rotation); // Update odometry with these values
}

// ----------- UPDATE ODOMETRY ----------- //
// Updates robot positional data on the field
void Odometry::update(tracking new_rotation){

  // ----- SETUP ----- //
  pos.theta=new_rotation.imu; // Update robot angle to the IMU Sensor
  
  new_rotation.imu=degToRad(new_rotation.imu); // Convert inertial sensor value 
  
  tracking rotation_delta(new_rotation.fwd-rotation.fwd,
                          new_rotation.back-rotation.back,
                          new_rotation.imu-rotation.imu);

  tracking rotation_old( rotation.fwd, rotation.back, rotation.imu);

  rotation.setRotation({new_rotation.fwd, new_rotation.back, new_rotation.imu});

  point2 pos_loc(0,0,0);
  if(rotation_delta.imu==0)
    pos_loc.setPosition({rotation_delta.back, rotation_delta.fwd});
  else{
    pos_loc.setPosition({ 2*std::sin(rotation_delta.imu/2) * ((rotation_delta.back/rotation_delta.imu) + offset.back),
                          2*std::sin(rotation_delta.imu/2) * ((rotation_delta.fwd/rotation_delta.imu) + offset.fwd)});
  }

  point2 polar_loc(0,0,0);
  if(pos_loc.x!=0 || pos_loc.y!=0)
    polar_loc.setPosition({ std::sqrt(std::pow(pos_loc.x, 2) + std::pow(pos_loc.y, 2)), 0 ,
                            std::atan2(pos_loc.y, pos_loc.x)});

  double polar_theta_glb = polar_loc.theta - rotation_old.imu - (rotation_delta.imu/2);

  point2 pos_delta( polar_loc.x * std::cos(polar_theta_glb),
                    polar_loc.x * std::sin(polar_theta_glb));
  
  pos.x+=pos_delta.x;
  pos.y+=pos_delta.y;
}

double Odometry::degToRad(double deg){
  return deg*3.14159265359/180;
}
