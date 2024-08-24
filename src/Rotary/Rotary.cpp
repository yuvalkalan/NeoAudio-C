#include "Rotary.h"

Rotary::Rotary(int clk, int dt, int button) : m_clk(clk), m_dt(dt), m_clk_last_value(false), m_spin(0), btn(button)
{
}

void Rotary::update()
{
    bool clk_state = gpio_get(m_clk);
    bool dt_state = gpio_get(m_dt);
    if (clk_state != m_clk_last_value && m_clk_last_value)
    {
        if (dt_state != clk_state)
            m_spin += 1;
        else
            m_spin -= 1;
    }
    m_clk_last_value = clk_state;
}
int Rotary::get_spin()
{
    int v = m_spin;
    m_spin = 0;
    return v;
}