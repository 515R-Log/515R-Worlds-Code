#pragma once
#include "EZ-Template/api.hpp"
#include "EZ-Template/Odometry/point2.hpp"

extern Drive chassis;

void setDrive(double leftFront, double leftBack);

void setDrive(double speed);

void setDriveMotors();

void motorHold();

void motorCoast();

void driveDiscs();

void driveGoal();

point2 setDriveJoystics();
