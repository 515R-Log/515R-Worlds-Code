#pragma once
#include "api.h"

class PotMgr {
private:
  int raw_value;
  int raw_max = 4095;

  double scale_factor=1;
  bool reversed;

  double zero_range_max=0;
  double zero_range_min=0;

  pros::ADIAnalogIn potentiometer;

  void update_scale_factor();

  double degrees_to_ticks(double degrees);

  double ticks_to_degrees(double ticks);

  double max_value = 4095;
  double min_value = 0;
 
public: 
 
  PotMgr(char portChar, bool direction=false, int max=100, int min=0, int zeroRangeMax=0, int zeroRangeMin=0);

  int get_raw_value();
  double get_scale_factor();
  double get_max();
  double get_min();
  bool get_direction();

  double get_zero_range_max();
  double get_zero_range_min();

  void set_direction(bool dir);
  void set_max(double max);
  void set_min(double min);
  void set_zero_range_min(double degrees);
  void set_zero_range_max(double degrees);

  double get_value();
};

extern PotMgr autoSwitch;
