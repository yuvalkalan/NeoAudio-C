#pragma once
#include <chrono>
#include <stdlib.h>

class Color
{
private:
    uint32_t m_color;
    uint32_t m_next_color;
    std::chrono::high_resolution_clock::time_point m_start_time;

private:
    void gen();

public:
    Color();
    uint32_t get(double duration, int max_bright);
};
