#include "EZ-Template/Odometry/tracking.hpp"
#include <vector>
/*-----------------------------------------------------------------------------
  _______             _
 |__   __|           | |
    | |_ __ __ _  ___| | _____ _ __
    | | '__/ _` |/ __| |/ / _ \ '__|
    | | | | (_| | (__|   <  __/ |
    |_|_|  \__,_|\___|_|\_\___|_|

Created on 2/20/2022
Last Updated on 2/26/2023

Updates the

-----------------------------------------------------------------------------*/

tracking::tracking(double new_fwd, double new_back, double new_imu){
  fwd=new_fwd;
  back=new_back;
  imu=new_imu;
}

tracking::tracking(){
  fwd=0;
  back=0;
  imu=0;
}

void tracking::setRotation(std::vector<double> rotation){
  if(rotation.size()>0)
    fwd=rotation.at(0);
  if(rotation.size()>1)
    back=rotation.at(1);
  if(rotation.size()>2)
    imu=rotation.at(2);
}

std::vector<double> tracking::getRotation(){
  std::vector<double> result;
  result.push_back(fwd);
  result.push_back(back);
  result.push_back(imu);
  return result;
}
