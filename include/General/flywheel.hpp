#pragma once
#include "sylib/sylib.hpp"
#include "Flywheel-Control\FeedForward.hpp"

void setShooter(int power);
void setFlywheel(double rpm);
double getFlywheelTarget();

double getFlywheelVelocity();

extern sylib::Motor shooterMtr;
extern FeedForward flywheelInput;

enum flyMode{
  MAX=1, FAR=2, MID=3, CLOSE=4, BASE=5, OFF=6
};

extern flyMode deliveryMode;

void setShooterMotor();

void setShooterAsyncIterate();
