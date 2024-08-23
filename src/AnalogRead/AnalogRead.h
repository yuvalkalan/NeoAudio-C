#pragma once
#include "hardware/adc.h"
class AnalogRead
{
private:
    int m_adc_gpio;
    int m_sum = 0;
    int m_counter = 0;
    int m_max = 0;

private:
    void add(int value);

public:
    AnalogRead(int adc_gpio);
    void reset();
    int get_max();
    float get_avg();
    int read();
};
