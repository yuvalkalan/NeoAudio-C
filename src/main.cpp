#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"

#include "LedControl/LedControl.h"
#include "Clock/Clock.h"
#include "BlinkLed/BlinkLed.h"
#include "AnalogRead/AnalogRead.h"
#include "Rotary/Rotary.h"
#include "SerialIn/SerialIn.h"

#define CLOCK_REFRESH_RATE 200  // 200.0
#define READ_REFRESH_RATE 10000 // 10kHz
#define GPIO_ANALOG_RIGHT 28    // aux right channel pin
#define GPIO_ANALOG_LEFT 27     // aux left channel pin
#define GPIO_CLK_PIN 17         // rotary clk pin
#define GPIO_DT_PIN 18          // rotary dt pin
#define GPIO_BUTTON_PIN 16      // rotary button pin

AnalogRead analog_right(GPIO_ANALOG_RIGHT);
AnalogRead analog_left(GPIO_ANALOG_LEFT);
Rotary rotary(GPIO_CLK_PIN, GPIO_DT_PIN, GPIO_BUTTON_PIN);
LedControl led_ctrl;
void core0()
{
    BlinkLed led;
    Clock clk(CLOCK_REFRESH_RATE);
    multicore_lockout_victim_init();
    led_ctrl.settings.update_mode();
    auto overloading_timer = std::chrono::high_resolution_clock::now();
    while (true)
    {
        led.update();
        multicore_lockout_start_blocking(); // get lock
        int spin = rotary.get_spin();
        if (spin)
            led_ctrl.settings.set_config_temp_value(led_ctrl.settings.get_config_temp_value() + spin);
        int right_avg = analog_right.get_avg();
        int right_max = analog_right.get_max();
        int left_avg = analog_left.get_avg();
        int left_max = analog_left.get_max();
        analog_right.reset();
        analog_left.reset();
        rotary.btn.update();
        if (rotary.btn.clicked())
        {
            led_ctrl.settings.update_mode();
            printf("clicked!\n");
        }
        if (rotary.btn.double_clicked())
        {
            int mode = led_ctrl.settings.get_mode();
            if (mode == CONFIG_BRIGHTNESS)
                led_ctrl.settings.set_max_bright(led_ctrl.settings.get_config_temp_value());
            else if (mode == CONFIG_SENSITIVITY)
                led_ctrl.settings.set_sensitivity(led_ctrl.settings.get_config_temp_value());

            else if (mode == CONFIG_VOLUME_THRESH)
                led_ctrl.settings.set_volume_threshold(led_ctrl.settings.get_config_temp_value());
            led_ctrl.settings.update_mode();
            printf("double clicked!\n");
        }
        if (rotary.btn.hold_down())
        {
            led_ctrl.settings.reset();
            printf("hold down!\n");
        }
        multicore_lockout_end_blocking(); // release lock

        led_ctrl.update(right_avg, right_max, left_avg, left_max);
        led_ctrl.pio.write();
        led_ctrl.pio.wait_until_finish();
        if (clk.tick() > 0.1)
        {
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = current_time - overloading_timer;
            double elapsed_seconds = elapsed.count();
            if (elapsed_seconds > 1)
            {
                printf("overloading core 0!\n");
                overloading_timer = current_time;
            }
        }
    }
}

void core1()
{
    multicore_lockout_victim_init();
    SerialIn srl_in;
    Clock clk(READ_REFRESH_RATE);
    auto overloading_timer = std::chrono::high_resolution_clock::now();
    while (true)
    {
        multicore_lockout_start_blocking(); // get lock
        analog_right.read();
        analog_left.read();
        rotary.update();
        srl_in.update(led_ctrl.settings);
        multicore_lockout_end_blocking(); // release lock
        if (clk.tick() > 0.1)
        {
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = current_time - overloading_timer;
            double elapsed_seconds = elapsed.count();
            if (elapsed_seconds > 1)
            {
                printf("overloading core 1!\n");
                overloading_timer = current_time;
            }
        }
    }
}

int main()
{
    stdio_init_all();
    adc_init();
    sleep_ms(1000);
    multicore_launch_core1(core1);
    core0();
}
