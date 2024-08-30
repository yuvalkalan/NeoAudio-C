#include "LedControl.h"

LedControl::LedControl() : m_leds(), m_color(), m_counter(0), pio(), settings()
{
}
void LedControl::set_buffer()
{
    // set buffer
    for (int i = 0; i < m_leds.size(); i++)
    {
        pio.buffer[m_leds[i].index()] = m_leds[i].color();
    }
    // update and remove m_leds
    m_leds.erase(std::remove_if(m_leds.begin(), m_leds.end(),
                                [](NeoPixelLed &obj)
                                { return !obj.update(); }),
                 m_leds.end());
}
void LedControl::start(bool rtl)
{
    m_leds.push_back({m_color.get(COLOR_DURATION, settings.get_max_bright()), rtl});
}
void LedControl::clear()
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        pio.buffer[i] = 0;
    }
}
void LedControl::update(UPDATE_PARAMS)
{
    clear();
    Mode mode = settings.get_mode();
    // TODO: replace with map!
    if (mode == SOUND_BAR)
        update_sound_bar(UPDATE_VALUES);
    else if (mode == SOUND_ROUTE)
        update_sound_route(UPDATE_VALUES);
    else if (mode == RANDOM_COLOR)
        update_random_colors(UPDATE_VALUES);
    else if (mode == CONFIG_BRIGHTNESS)
        update_config_brightness(UPDATE_VALUES);
    else if (mode == CONFIG_SENSITIVITY)
        update_config_sensitivity(UPDATE_VALUES);
    else if (mode == CONFIG_VOLUME_THRESH)
        update_config_volume_thresh(UPDATE_VALUES);
    else if (mode == OFF)
        update_off(UPDATE_VALUES);
}
void LedControl::update_sound_bar(UPDATE_PARAMS)
{
    // right
    int right_value = (settings.get_sensitivity() * (NUM_PIXELS / 2) * right_avg / 65535);
    right_value = right_value > (NUM_PIXELS / 2) ? (NUM_PIXELS / 2) : right_value;
    for (int i = 0; i < right_value; i++)
    {
        int c = i * settings.get_max_bright() / (NUM_PIXELS / 2);
        pio.buffer[NUM_PIXELS / 2 - 1 - i] = (c << 8) | (settings.get_max_bright() - c);
    }
    // left
    int left_value = (settings.get_sensitivity() * (NUM_PIXELS / 2) * left_avg / 65535);
    // printf("left value is %d", left_value);
    left_value = left_value > (NUM_PIXELS / 2) ? (NUM_PIXELS / 2) : left_value;
    for (int i = 0; i < left_value; i++)
    {
        int c = i * settings.get_max_bright() / (NUM_PIXELS / 2);
        pio.buffer[NUM_PIXELS / 2 + i] = (c << 8) | (settings.get_max_bright() - c);
    }
}
void LedControl::update_sound_route(UPDATE_PARAMS)
{
    if (right_max > settings.get_volume_threshold())
        start(true);
    if (left_max > settings.get_volume_threshold())
        start(false);
    set_buffer();
}
void LedControl::update_random_colors(UPDATE_PARAMS)
{
    start(true);
    start(false);
    set_buffer();
}
void LedControl::update_config_brightness(UPDATE_PARAMS)
{
    printf("temp value is %d\n", settings.get_config_temp_value());
    uint8_t v = 255 * settings.get_config_temp_value() / 100;
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        pio.buffer[i] = (v << 16) | (v << 8) | v;
    }
}
void LedControl::update_config_sensitivity(UPDATE_PARAMS)
{
    // printf("temp value is %d\n", settings.get_config_temp_value());
    int v = NUM_PIXELS * settings.get_config_temp_value() / 100;
    for (int i = 0; i < v; i++)
    {
        int c = i * settings.get_max_bright() / NUM_PIXELS;
        pio.buffer[i] = (c << 8) | ((settings.get_max_bright() - c) << 16);
    }
}
void LedControl::update_config_volume_thresh(UPDATE_PARAMS)
{
    int freq = (NUM_PIXELS * settings.get_config_temp_value() / 100) | 1; // counter / pixel
    printf("temp value is %d, freq is %d\n", settings.get_config_temp_value(), freq);
    if (m_counter / freq != 0)
    {
        start(true);
        start(false);
        m_counter -= freq;
    }
    m_counter += 1;
    set_buffer();
}
void LedControl::update_off(UPDATE_PARAMS)
{
}