#include "accel.h"

#include <Wire.h>

const int ADXL345_I2CADDR = 0x53;   // 0x1D if SDO = HIGH


Accelerometer::Accelerometer()
    : _adxl(ADXL345_I2CADDR)    // USE FOR I2C COMMUNICATION
{

}

bool Accelerometer::begin()
{
    if (!Wire.begin())
    {
        return false;
    }

    if (!_adxl.init())
    {
        return false;
    }

    _startup_accel_data = read_data();

    return true;
}

Accelerometer::AccelData Accelerometer::read_data() const
{
    auto g = _adxl.getGValues();
    return {g.x, g.y, g.z};
}

void Accelerometer::enable_activity_interrupt(float wake_sensitivity_g)
{
    _adxl.setLowPower(true);

    _adxl.setDataRate(ADXL345_DATA_RATE_25);

    _adxl.setRange(ADXL345_RANGE_2G);

    _adxl.setAxisOffset(-2, 8, -55);    // TODO: Put in settings file
                                        // TODO: Add calibration mode to create settings file.

    _adxl.setActivityParameters(ADXL345_DC_MODE, ADXL345_XYZ, wake_sensitivity_g);
    _adxl.setInterrupt(ADXL345_ACTIVITY, INT_PIN_1);
}

const Accelerometer::AccelData& Accelerometer::get_startup_accel_data() const
{
    return _startup_accel_data;
}

