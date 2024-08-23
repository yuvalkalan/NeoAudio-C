#pragma once
#include <chrono>
#include "pico/stdlib.h"

#define LED_PIN 25
#define BLINK_REFRESH_RATE 2
#define GPIO_ON 1
#define GPIO_OFF 0

class BlinkLed
{
private:
    std::chrono::high_resolution_clock::time_point m_last_time;
    bool m_is_on = false;

public:
    BlinkLed();
    void update();
};
