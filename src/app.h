#pragma once

#include "accel.h"
#include <ESPDeepSleep.h>

class App
{
public:
    void begin();
    void loop();
private:
    void print_debug_data();
    ESPDeepSleep deepSleep;
    Accelerometer accel;
};
