#pragma once

#include "Button/Button.h"

class Rotary
{
private:
    int m_clk;
    int m_dt;
    int m_clk_last_value;
    int spin;
    Button y;

public:
    Rotary();
};

Rotary::Rotary()
{
}
