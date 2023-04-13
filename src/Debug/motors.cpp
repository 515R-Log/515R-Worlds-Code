#include "General/drivetrain.hpp"
#include "General/intakeLiftRoller.hpp"
#include "General/flywheel.hpp"

bool motor_unplugged(){
	for(int i=0; i<3; i++){
		if(!chassis.left_motors.at(0).get_actual_velocity())
			return true;
	}
	for(int i=0; i<3; i++){
		if(!chassis.right_motors.at(0).get_temperature()<100)
			return true;
	}
	return !(shooterMtr.get_temperature()<100);
}

int start_time=0;

void detect_motor_disconnect(){
    return;
    if(motor_unplugged() && (pros::millis()>(start_time+3000))){
        start_time=pros::millis();
        controller.rumble(".");
    }
}