#ifndef HEATERLOGIC_H
#define HEATERLOGIC_H

#include <vector>
//#include <ArduinoSTL.h>
//#include <stdbool.h>


struct heater {
  bool htr1_flag = false;
  bool htr2_flag = false;
};

struct heater heaterCompare();

#endif