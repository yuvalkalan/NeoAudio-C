#include "LedControl.h"

LedControl::LedControl() : pio()
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

void LedControl::start(bool direction)
{
    leds.push_back({Color::RED, direction});
}

void LedControl::clear()
{
    for (int i = 0; i < NUM_PIXELS; i++)
    {
        this->pio.buffer[i] = 0;
    }
}

void LedControl::update(int index)
{
    this->clear();
    if (index % 50 == 0)
        this->start((index / 50) % 2 == 0);

    this->set_buffer();
}