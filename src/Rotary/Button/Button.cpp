#include "Button.h"

Button::Button(int pin) : m_pin(pin)
{
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
    m_value = gpio_get(pin);
}

void Button::reset()
{
    m_has_changed = false;
    m_has_click = false;
    m_has_double_click = false;
    m_has_hold_down = false;
}
void Button::update()
{
    reset();
    bool value = gpio_get(m_pin);
    m_has_changed = value != m_value;
    auto current_time = std::chrono::high_resolution_clock::now();
    if (m_have_click_time)
    {

        std::chrono::duration<double> elapsed = current_time - m_last_click_time;
        double elapsed_seconds = elapsed.count();
        if (elapsed_seconds > MAX_DOUBLE_CLICK_DELTA)
        {
            m_have_click_time = false;
            m_has_click = true;
        }
    }
    if (!value)
    {
        if (m_has_changed)
        {
            if (m_have_click_time)
            {
                m_has_double_click = true;
                m_have_click_time = false;
            }
            else
            {
                m_last_click_time = current_time;
                m_have_click_time = true;
            }
            m_press_time = current_time;
            m_have_press_time = true;
        }
    }
    else
    {
        m_have_press_time = false;
    }
    if (m_have_press_time)
    {
        std::chrono::duration<double> elapsed = current_time - m_press_time;
        double elapsed_seconds = elapsed.count();
        if (elapsed_seconds > HOLD_DOWN_THRESHOLD)
        {
            m_has_hold_down = true && !m_got_hold_down;
            m_got_hold_down = true;
            m_have_click_time = false;
        }
        else
        {
            m_got_hold_down = false;
        }
    }
    else
    {
        m_got_hold_down = false;
    }
    m_value = value;
}
bool Button::is_down()
{
    return m_value == 0;
}
bool Button::is_up()
{
    return !is_down();
}
bool Button::clicked()
{
    return m_has_click;
}
bool Button::double_clicked()
{
    return m_has_double_click;
}
bool Button::hold_down()
{
    return m_has_hold_down;
}