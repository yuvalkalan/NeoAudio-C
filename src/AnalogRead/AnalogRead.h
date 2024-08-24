#pragma once
#include "hardware/adc.h"
class AnalogRead
{
private:
    int m_adc_gpio;
    int m_sum;
    int m_counter;
    int m_max;

private:
    void add(int value);

public:
    AnalogRead(int adc_gpio);
    void reset();
    int get_max();
    float get_avg();
    int read();
};
