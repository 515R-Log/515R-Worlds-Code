#include "General/flywheel.hpp"
#include "General/general.hpp"
#include "General/intakeLiftRoller.hpp"
#include "General/piston.hpp"
#include "pros/misc.hpp"

/*-----------------------------------------------------------------------------
  _____         _  _
 |  __ \       | |(_)
 | |  | |  ___ | | _ __   __ ___  _ __  _   _
 | |  | | / _ \| || |\ \ / // _ \| '__|| | | |
 | |__| ||  __/| || | \ V /|  __/| |   | |_| |
 |_____/  \___||_||_|  \_/  \___||_|    \__, |
                                         __/ |
Created on 5/20/2022                    |___/
Last Updated on 3/10/2022

Contains commands for the Delivery system in shooting Disks
Uses PID / FEED FORWARD to quickly adapt flywheel control to a target RPM
Sets up Flywheel Velocity between and during shots.
Utilizes driver and autonomous input

Applications of the Delivery are as follows
• Switching between a goal "quick fire" and "fill up" speed
• Locking to Max Speed
• Spinning at a slow base speed, to prevent jamming
• Different velocity targets when deflector is raised/lowered
• Turning off Flywheel entirely

-----------------------------------------------------------------------------*/

FeedForward flywheelInput(6.0475,0.00358,0.000221,34.4,10);
// FeedForward flywheelInput(4.6995,0.0008315,0.0005144,800,20);

sylib::Motor shooterMtr (11, 600,true);
// sylib::Motor shooterMtr (7, 600,true);

// Delcare the type of distance to set the flywheel for Driver Control
flyMode deliveryMode=CLOSE;

// --------------- SET DELIVERY --------------- //
// Set the Delivery motor on a given speed
void setShooter(int power){
  if(power==127)
    shooterMtr.set_voltage(12000);
  else
    shooterMtr.set_voltage(power*94.488);
}

// --------------- SET DELIVERY --------------- //
// Set the Delivery motor on a given speed
void setFlywheel (double rpm){
  flywheelInput.set_velocity(rpm);
}

double getFlywheelTarget(){
  return flywheelInput.target;
}

// --------------- SET DELIVERY MODE --------------- //
// Allows driver to switch between modes
// Modes: Max / Far / Close / Off / Low
void setShooterMode(){
  // ---------- MAX -   Used as a catch-all velocity for niche situations
  if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_B))
    deliveryMode=MAX;

  // ---------- FAR  -   Useful for shooting from the Autonomous Line
  if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1) && controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
    deliveryMode=FAR;
  }

  // ---------- MID  -   Useful when opposing robots play defense beneath low goal
  if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
    deliveryMode=MID;
  }

  // ---------- CLOSE  - Used to fill goals at later stages of a match
  if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_L2) && controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
    deliveryMode=CLOSE;
  }

  // ---------- BASE  -  Used to slow down the flywheel without turning it off completely
  if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)&& !controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
    deliveryMode=BASE;
  }

  // ---------- OFF   -  Used during practice and debugging purposes
  if(controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)&& controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
    deliveryMode=OFF;
  }
}

void setFlySpeed(const short vel1, short vel2=-1){
    if(vel2==-1)
        vel2=vel1;
    
    if(deflectorUp)
      flywheelInput.set_velocity(vel1);
    else
      setFlywheel(vel2);
}

// --------------- SET DELIVERY VELOCITY --------------- //
// Sets the flywheel target velocity based on its driver-control mode
void setShooterTarget(){

    // Go Through each value of Delivery Mode
    switch (deliveryMode){

        // ---------- MAX POWER ---------- //
        case MAX:
            setFlywheel(575); // 800 RPM Target
            break;

        // ---------- FAR-RANGE ---------- //
        case FAR:
            setFlywheel(550); // 550 RPM Target
            break;

        // ---------- MID-RANGE ---------- //
        case MID:
            setFlywheel(480); // 480 RPM Target
            break;

        // ---------- CLOSE-RANGE ---------- //
        case CLOSE:
            setFlySpeed(470,480); // 480 RPM Target w/ Deflector Up | 420 RPM w/ deflector Down
            break;

        // ---------- BASE POWER ---------- //
        case BASE:
            setFlywheel(40); // 40 RPM Target
            break;

        // ---------- TURN OFF FLYWEEL ---------- //
        case OFF:
            setFlywheel(0); // Set Target RPM to 0
            break;

    };

    if(distIndex.get()<90)
      setFlywheel(10000);

}

// --------------- SET DELIVERY MOTOR --------------- //
// Run the Delivery Motor based on its distance from the Goal
void setShooterMotor(){

  // Update the Shooter Mode
  setShooterMode();

  // Change Shooter Speed based on Mode
  setShooterTarget();
}

// Declare the first pass-through of the flywheel
bool firstPass=true;

// --------------- FLYWHEEL TASK --------------- //
// Run the Flywheel in the background of everything
// Uses Feed Forward to adjusts voltage given to motor
void setShooterAsyncIterate(){

  // If the controller disconnects momentarily
  if(!firstPass && shooterMtr.get_amps()==0 && getFlywheelTarget()!=0){
    // Reset the Flywheel
    shooterMtr.set_voltage(0);
    pros::delay(1);
  }

  // Grab the output voltage by Feed Forward
  double result=flywheelInput.iterate(shooterMtr.get_velocity(),20);

  result=result*2.94*6+1400;

  if(getFlywheelTarget()==0) // If the target is zero
    shooterMtr.set_voltage(0); // Turn off the flywheel
  
  else // If the target is not 0
    shooterMtr.set_voltage(-result); // Apply speed to flywheel

  //If this is the first go-around
  if(firstPass)
    firstPass=false; // Declare that the first run-through of flywheel is now done
}
