#pragma once

class AnalogRead
{
private:
    int m_sum = 0;
    int m_counter = 0;
    int m_max = 0;

public:
    AnalogRead();
    void reset();
    int get_max();
    float get_avg();
    void add(int value);
};
