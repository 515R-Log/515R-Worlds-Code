#include "EZ-Template/Odometry/Odometry.hpp"
#include "EZ-Template/Odometry/tracking.hpp"
/*-----------------------------------------------------------------------------
   ____      _                                __  __       _
  / __ \    | |                              |  \/  |     (_)
 | |  | | __| | ___  _ __ ___       ______   | \  / | __ _ _ _ __
 | |  | |/ _` |/ _ \| '_ ` _ \     |______|  | |\/| |/ _` | | '_ \
 | |__| | (_| | (_) | | | | | |_             | |  | | (_| | | | | |
  \____/ \__,_|\___/|_| |_| |_(_)            |_|  |_|\__,_|_|_| |_|

Created on 2/20/2022
Last Updated on 2/26/2023

Updates the

-----------------------------------------------------------------------------*/

Odometry::Odometry( const char fwd_top_port, const char back_top_port, const int extender_port, const short imu_port,
                    const double fwd_wheel_diameter, const double back_wheel_diameter, const double fwd_wheel_offset, const double back_wheel_offset):
                    fwd({extender_port, fwd_top_port, fwd_top_port+1},false), back({extender_port, back_top_port, back_top_port+1},true), imu(imu_port),
                    pos(), rotation(0,0), wheelDiam(fwd_wheel_diameter, back_wheel_diameter), offset(fwd_wheel_offset,back_wheel_offset){
}

void Odometry::setDimensions(const double fwd_wheel_offset, const double back_wheel_offset){
  offset.setRotation({fwd_wheel_offset,back_wheel_offset});
}

void Odometry::setPosition(const double x, const double y){
  pos.x=x;
  pos.y=y;
}

void Odometry::setPosition(const double x, const double y, const double theta){
  point2 position({x,y,theta});
  pos=position;
  setTheta(theta);
}

point2 Odometry::getPosition(){
  return pos;
}

double Odometry::getX(){
  return pos.x;
}
void Odometry::setX(double x){
  pos.x = x;
}

double Odometry::getY(){
  return pos.y;
}
void Odometry::setY(double y){
  pos.y = y;
}

double Odometry::getTheta(){
  return imu.get_rotation();
}
void Odometry::setTheta(double theta){
  imu.set_rotation(theta);
}
