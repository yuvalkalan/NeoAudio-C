#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "LedControl/LedControl.h"
#include "Clock/Clock.h"
#include "BlinkLed/BlinkLed.h"

#define CLOCK_REFRESH_RATE 200.0

const float brightness = 0.020f;

// static void shiftArray(int32_t *arr, int32_t size)
// {
//     for (int i = 0; i < size - 1; i++)
//     {
//         int offset = size - i - 2;
//         *(long long *)((int *)arr + offset) <<= 32;
//     }
// }

int main()
{
    stdio_init_all();
    sleep_ms(1000);
    BlinkLed led;
    Clock clk(CLOCK_REFRESH_RATE);
    LedControl LedCtrl;
    int t = 0;
    while (1)
    {
        led.update();
        LedCtrl.update(t);
        LedCtrl.pio.write();
        LedCtrl.pio.wait_until_finish();
        t += 1;
        clk.tick();
    }
}

// #include <stdio.h>
// #include <stdlib.h>
// #include "pico/stdlib.h"
// #include "BlinkLed/BlinkLed.h"

// int main()
// {
//     stdio_init_all();
//     sleep_ms(2000);
//     BlinkLed led;
//     Settings settings;
//     printf("current settings: %d, %d, %d, %d\n", settings.get_mode(), settings.get_max_bright(), settings.get_sensitivity(), settings.get_volume_threshold());
//     settings.set_volume_threshold(50);
//     printf("current settings: %d, %d, %d, %d\n", settings.get_mode(), settings.get_max_bright(), settings.get_sensitivity(), settings.get_volume_threshold());
//     for (int i = 0; i < 10; i++)
//     {
//         settings.update_mode();
//         printf("current mode: %d\n", settings.get_mode());
//     }

//     settings.reset();
//     while (true)
//         led.update();
// }