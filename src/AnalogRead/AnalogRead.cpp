#include "AnalogRead.h"

AnalogRead::AnalogRead()
{
}

void AnalogRead::reset()
{
    m_sum = 0;
    m_counter = 0;
    m_max = 0;
}
int AnalogRead::get_max()
{
    return m_max;
}
float AnalogRead::get_avg()
{
    return (m_sum) / (m_counter + 0.001f);
}
void AnalogRead::add(int value)
{
    m_max = value > m_max ? value : m_max;
    m_counter += 1;
    m_sum += value;
}
