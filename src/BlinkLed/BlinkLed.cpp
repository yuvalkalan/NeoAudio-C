#include "BlinkLed.h"

BlinkLed::BlinkLed()
{
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
}

void BlinkLed::update()
{
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = current_time - m_last_time;
    double elapsed_seconds = elapsed.count();
    if (elapsed_seconds > 1.0 / BLINK_REFRESH_RATE)
    {
        gpio_put(LED_PIN, m_is_on ? GPIO_OFF : GPIO_ON);
        m_is_on = !m_is_on;
        m_last_time = current_time;
    }
}
