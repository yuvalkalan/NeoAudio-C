#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "pico/multicore.h"

#include "LedControl/LedControl.h"
#include "Clock/Clock.h"
#include "BlinkLed/BlinkLed.h"
#include "AnalogRead/AnalogRead.h"

#define CLOCK_REFRESH_RATE 120  // 200.0
#define READ_REFRESH_RATE 44100 // 44.1kHz
#define GPIO_ANALOG_RIGHT 27
#define GPIO_ANALOG_LEFT 28
#define GPIO_CLK_PIN 18    // yellow
#define GPIO_DT_PIN 17     // green
#define GPIO_BUTTON_PIN 16 // blue

const float brightness = 0.020f;
AnalogRead analog_right(GPIO_ANALOG_RIGHT);
AnalogRead analog_left(GPIO_ANALOG_LEFT);

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
    LedControl LedCtrl;
    while (1)
    {
        led.update();
        // TODO: add Lock here
        int right_avg = analog_right.get_avg();
        int right_max = analog_right.get_max();
        int left_avg = analog_left.get_avg();
        int left_max = analog_left.get_max();
        analog_right.reset();
        analog_left.reset();
        // TODO: release Lock here

        LedCtrl.update(right_avg, right_max, left_avg, left_max);
        LedCtrl.pio.write();

        clk.tick();
    }
}

void core1()
{
    Clock clk(READ_REFRESH_RATE);
    while (true)
    {
        // TODO: add Lock here
        analog_right.read();
        analog_left.read();
        // TODO: release Lock here
        clk.tick();
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

// int main()
// {
//     stdio_init_all();
//     adc_init();

//     int counter = 0;
//     AnalogRead analog_right(GPIO_ANALOG_RIGHT);
//     AnalogRead analog_left(GPIO_ANALOG_LEFT);
//     while (1)
//     {
//         analog_right.read();
//         analog_left.read();
//         counter += 1;
//         sleep_ms(1);
//         if (counter % 500 == 0)
//         {
//             counter = 0;
//             printf("right avg: %f, right max: %d left avg: %f, left max: %d\n", analog_right.get_avg(), analog_right.get_max(), analog_left.get_avg(), analog_left.get_max());
//             analog_right.reset();
//             analog_left.reset();
//         }
//     }
// }