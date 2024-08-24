#include "LedControl.h"

LedControl::LedControl() : settings(), pio()
{
}

void LedControl::set_buffer()
{
    // set buffer
    for (int i = 0; i < leds.size(); i++)
    {
        this->pio.buffer[leds[i].index()] = leds[i].color();
    }
    // update and remove leds
    std::vector<int> to_remove;
    for (int i = 0; i < leds.size(); i++)
    {
        if (!leds[i].update())
            to_remove.push_back(i);
    }
    for (int i = 0; i < to_remove.size(); i++)
    {
        leds.erase(leds.begin() + to_remove[i]);
    }
}

void LedControl::start(bool rtl)
{
    leds.push_back({Color::RED, rtl});
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
    this->pio.wait_until_finish();
    this->clear();
    float real_volume_threshold = settings.get_volume_threshold() / 100.0f * MAX_VOLUME_THRESHOLD;
    if (right_max > real_volume_threshold)
        this->start(true);
    if (left_max > real_volume_threshold)
        this->start(false);
    this->set_buffer();
}