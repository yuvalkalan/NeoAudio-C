#pragma once
#include "Button/Button.h"

class Rotary // this is singleton, use only once!
{
private:
    uint m_clk;
    uint m_dt;
    volatile bool m_last_clk;
    volatile int m_spin;
    static Rotary *instance;

public:
    Button btn;

private:
    static void encoder_callback(uint gpio, uint32_t events);
    static Rotary *getInstance();

public:
    Rotary(int clk, int dt, int button);
    int get_spin();
};
