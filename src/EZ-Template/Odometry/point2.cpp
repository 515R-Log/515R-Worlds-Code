#include "EZ-Template/Odometry/point2.hpp"
#include <vector>
/*-----------------------------------------------------------------------------
  _____      _       _
 |  __ \    (_)     | |
 | |__) |__  _ _ __ | |_
 |  ___/ _ \| | '_ \| __|
 | |  | (_) | | | | | |_
 |_|   \___/|_|_| |_|\__|

Created on 2/20/2022
Last Updated on 2/26/2023

Updates the

-----------------------------------------------------------------------------*/

// ---------------- POINT 2 ---------------- //
// Sets up the PID system class.
point2::point2(double new_x, double new_y, double new_theta){
  x=new_x;
  y=new_y;
  theta=new_theta;
}

// ---------------- POINT 2 ---------------- //
// Sets up the PID system class.
point2::point2(){
  x=0;
  y=0;
  theta=0;
}

// ---------------- SET POSITION ---------------- //
// Sets up the PID system class.
void point2::setPosition(std::vector<double> position){
  if(position.size()>0)
    x=position.at(0);
  if(position.size()>1)
    y=position.at(1);
  if(position.size()>2)
    theta=position.at(2);
}

// ---------------- GET POSITION ---------------- //
// Sets up the PID system class.
std::vector<double> point2::getPosition(){
  std::vector<double> result;
  result.push_back(x);
  result.push_back(y);
  result.push_back(theta);
  return result;
}
