#pragma once
#include "pros/misc.hpp"
#include "pros/distance.hpp"
#include "pros/optical.hpp"
#include "PotMgr/PotMgr.hpp"

extern const short MAX_MTR_POWER;

extern pros::Controller controller;

extern pros::Distance distIndex;
extern pros::Distance distStack;
extern pros::Distance distStackBack;

extern pros::Distance distIntake;

extern pros::Distance distWallLeft;
extern pros::Distance distWallRight;

extern pros::Optical opticalRoller;

extern PotMgr autoSwitch;

extern pros::Optical leftRollerColor;
extern pros::Optical rightRollerColor;
