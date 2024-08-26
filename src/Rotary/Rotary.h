#pragma once
#include "Button/Button.h"
#include "chrono"
#define ROTARY_SLEEP (1 / 1000.0f) // 1ms
class Rotary
{
private:
    int m_clk;
    int m_dt;
    bool m_clk_last_value;
    int m_spin;
    std::chrono::high_resolution_clock::time_point m_last_run;

public:
    Button btn;

public:
    Rotary(int clk, int dt, int button);
    void update();
    int get_spin();
};
