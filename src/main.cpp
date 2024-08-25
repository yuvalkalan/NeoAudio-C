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

#define CLOCK_REFRESH_RATE 200  // 200.0
#define READ_REFRESH_RATE 10000 // 10kHz
#define GPIO_ANALOG_RIGHT 27    // aux right channel pin
#define GPIO_ANALOG_LEFT 28     // aux left channel pin
#define GPIO_CLK_PIN 18         // rotary clk pin
#define GPIO_DT_PIN 17          // rotary dt pin
#define GPIO_BUTTON_PIN 16      // rotary button pin

const float brightness = 0.020f;
AnalogRead analog_right(GPIO_ANALOG_RIGHT);
AnalogRead analog_left(GPIO_ANALOG_LEFT);
Rotary rotary(GPIO_CLK_PIN, GPIO_DT_PIN, GPIO_BUTTON_PIN);

// static void shiftArray(int32_t *arr, int32_t size)
// {
//     for (int i = 0; i < size - 1; i++)
//     {
//         int offset = size - i - 2;
//         *(long long *)((int *)arr + offset) <<= 32;
//     }
// }

void core0()
{
    BlinkLed led;
    Clock clk(CLOCK_REFRESH_RATE);
    LedControl led_ctrl;
    led_ctrl.settings.update_mode();
    multicore_lockout_victim_init();
    while (true)
    {
        led.update();
        multicore_lockout_start_blocking(); // get lock
        int right_avg = analog_right.get_avg();
        int right_max = analog_right.get_max();
        int left_avg = analog_left.get_avg();
        int left_max = analog_left.get_max();
        analog_right.reset();
        analog_left.reset();
        rotary.btn.update();
        if (rotary.btn.clicked())
        {
            // printf("clicked!\n");
        }
        if (rotary.btn.double_clicked())
        {
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
            printf("overloading core 0!\n");
    }
}

void core1()
{
    multicore_lockout_victim_init();
    Clock clk(READ_REFRESH_RATE);
    while (true)
    {
        multicore_lockout_start_blocking(); // get lock
        analog_right.read();
        analog_left.read();
        rotary.update();
        multicore_lockout_end_blocking(); // release lock
        if (clk.tick() > 0.1)
            printf("overloading core 1!\n");
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

// #include "Rotary/Button/Button.h"

// int main()
// {
//     stdio_init_all();
//     sleep_ms(1000);
//     Button b(16);
//     while (1)
//     {
//         b.update();
//         if (b.clicked())
//         {
//             printf("clicked!\n");
//         }
//         if (b.double_clicked())
//         {
//             printf("double clicked!\n");
//         }
//         if (b.hold_down())
//         {
//             printf("hold!\n");
//         }
//     }
// }