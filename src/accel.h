#pragma once


#include <ADXL345_WE.h>


class Accelerometer
{
public:
    Accelerometer();

    bool begin();

    struct AccelData
    {
        float x;
        float y;
        float z;
    };
    AccelData read_data() const;

    void enable_activity_interrupt(float wake_sensitivity_g);

    const AccelData& get_startup_accel_data() const;

private:
    mutable ADXL345_WE _adxl;
    AccelData _startup_accel_data;
};
