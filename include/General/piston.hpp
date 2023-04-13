#pragma once
#include <string>
#include "pros/adi.hpp"

extern bool deflectorUp;

enum piston_state {
    UP=1,
    OUT=1,
    DOWN=0,
    IN=0
};

extern pros::ADIDigitalOut deflectorPiston;
extern pros::ADIDigitalOut endGameShootPiston;

//HELPER FUNCTIONS
void setPiston(std::string pistonType, piston_state dir);
void setPiston();
