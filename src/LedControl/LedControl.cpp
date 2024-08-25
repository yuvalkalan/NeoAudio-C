#include "LedControl.h"

LedControl::LedControl() : m_leds(), settings(), pio(), m_color()
{
}

void LedControl::set_buffer()
{
    // set buffer
    for (int i = 0; i < m_leds.size(); i++)
    {
        this->pio.buffer[m_leds[i].index()] = m_leds[i].color();
    }
    // update and remove m_leds
    std::vector<int> to_remove;
    for (int i = 0; i < m_leds.size(); i++)
    {
        if (!m_leds[i].update())
            to_remove.push_back(i);
    }
    for (int i = 0; i < to_remove.size(); i++)
    {
        m_leds.erase(m_leds.begin() + to_remove[i]);
    }
}

void LedControl::start(bool rtl)
{
    m_leds.push_back({m_color.get(COLOR_DURATION, settings.get_max_bright()), rtl});
}

void LedControl::clear()
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        this->pio.buffer[i] = 0;
    }
}

void LedControl::update(int right_avg, int right_max, int left_avg, int left_max)
{
    this->clear();
    Mode mode = settings.get_mode();
    // TODO: replace with map!
    if (mode == SOUND_BAR)
        update_sound_bar(right_avg, right_max, left_avg, left_max);
    else if (mode == SOUND_ROUTE)
        update_sound_route(right_avg, right_max, left_avg, left_max);
    else if (mode == RANDOM_COLOR)
        update_random_colors(right_avg, right_max, left_avg, left_max);
    else if (mode == CONFIG_BRIGHTNESS)
        update_config_brightness(right_avg, right_max, left_avg, left_max);
    else if (mode == CONFIG_SENSITIVITY)
        update_config_sensitivity(right_avg, right_max, left_avg, left_max);
    else if (mode == CONFIG_VOLUME_THRESH)
        update_config_volume_thresh(right_avg, right_max, left_avg, left_max);
    else if (mode == OFF)
        update_off(right_avg, right_max, left_avg, left_max);
}
void LedControl::update_sound_bar(int right_avg, int right_max, int left_avg, int left_max)
{
    // right
    int right_value = (settings.get_sensitivity() * (NUM_PIXELS / 2) * right_avg / 65535);
    right_value = right_value > (NUM_PIXELS / 2) ? (NUM_PIXELS / 2) : right_value;
    for (int i = 0; i < right_value; i++)
    {
        int c = i * settings.get_max_bright() / (NUM_PIXELS / 2);
        pio.buffer[i] = (c << 8) | ((settings.get_max_bright() - c) << 16);
    }
    // left
    int left_value = (settings.get_sensitivity() * (NUM_PIXELS / 2) * left_avg / 65535);
    // printf("left value is %d", left_value);
    left_value = left_value > (NUM_PIXELS / 2) ? (NUM_PIXELS / 2) : left_value;
    for (int i = 0; i < left_value; i++)
    {
        int c = i * settings.get_max_bright() / (NUM_PIXELS / 2);
        pio.buffer[NUM_PIXELS - 1 - i] = (c << 8) | ((settings.get_max_bright() - c) << 16);
    }
}
void LedControl::update_sound_route(int right_avg, int right_max, int left_avg, int left_max)
{
    if (right_max > settings.get_volume_threshold())
        this->start(true);
    if (left_max > settings.get_volume_threshold())
        this->start(false);
    this->set_buffer();
}

void LedControl::update_random_colors(int right_avg, int right_max, int left_avg, int left_max)
{
    this->start(true);
    this->start(false);
    this->set_buffer();
}
void LedControl::update_config_brightness(int right_avg, int right_max, int left_avg, int left_max)
{
}
void LedControl::update_config_sensitivity(int right_avg, int right_max, int left_avg, int left_max)
{
}
void LedControl::update_config_volume_thresh(int right_avg, int right_max, int left_avg, int left_max)
{
}
void LedControl::update_off(int right_avg, int right_max, int left_avg, int left_max)
{
}