#include "General/general.hpp"

extern pros::Motor rollerMtr;

void setIntake(double power);

void setLift(double power);

void setRollerMotor();

void antiJamAsyncIterate();
void antiFourAsyncIterate();

void setRoller(double power);

void enableAutoIntake();
void disableAutoIntake();

void liftAsyncIterate();

enum iMode {I_NEUTRAL=0, I_THREE=1, I_REMOVE_2LEFT=2, I_REMOVE_3LEFT=3, I_DISABLE=4};
extern iMode intakeMode; // Set the default mode of anti-four stack to disabled