#include "Rotary.h"
#include <stdio.h>
Rotary::Rotary(int clk, int dt, int button) : m_clk(clk), m_dt(dt), m_clk_last_value(false), m_spin(0), m_last_run(std::chrono::high_resolution_clock::now()), btn(button)
{
}

void Rotary::update()
{
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = current_time - m_last_run;
    double elapsed_seconds = elapsed.count();
    if (elapsed_seconds > ROTARY_SLEEP)
    {
        bool clk_state = gpio_get(m_clk);
        bool dt_state = gpio_get(m_dt);
        if ((!clk_state) && m_clk_last_value)
        {
            if (dt_state)
                m_spin -= 1;
            else
                m_spin += 1;
        }
        m_clk_last_value = clk_state;
        m_last_run = current_time;
    }
}
int Rotary::get_spin()
{
    int v = m_spin;
    m_spin = 0;
    return v;
}