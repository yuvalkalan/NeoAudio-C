#include "AnalogRead.h"

AnalogRead::AnalogRead(int adc_gpio) : m_adc_gpio(adc_gpio), m_sum(0), m_counter(0), m_max(0)
{
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(adc_gpio);
}

void AnalogRead::reset()
{
    m_sum = 0;
    m_counter = 0;
    m_max = 0;
}
int AnalogRead::get_max()
{
    return (m_max << 4);
}
float AnalogRead::get_avg()
{
    return (m_sum << 4) / (m_counter + 0.001f);
}
int AnalogRead::read()
{
    // Select ADC input
    adc_select_input(m_adc_gpio - 26);
    uint16_t value = adc_read();
    add(value);
    return value;
}
void AnalogRead::add(int value)
{
    m_max = value > m_max ? value : m_max;
    m_counter += 1;
    m_sum += value;
}
