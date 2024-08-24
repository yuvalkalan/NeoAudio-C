#pragma once

#include "Button/Button.h"

class Rotary
{
private:
    int m_clk;
    int m_dt;
    bool m_clk_last_value;
    int m_spin;

public:
    Button btn;

public:
    Rotary(int clk, int dt, int button);
    void update();
    int get_spin();
};
