#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "LedControl/LedControl.h"
#include "Clock/Clock.h"

#define CLOCK_REFRESH_RATE 200.0

const float brightness = 0.020f;
LedControl LedCtrl;

static void shiftArray(int32_t *arr, int32_t size)
{
    for (int i = 0; i < size - 1; i++)
    {
        int offset = size - i - 2;
        *(long long *)((int *)arr + offset) <<= 32;
    }
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r * brightness) << 8) |
           ((uint32_t)(g * brightness) << 16) |
           (uint32_t)(b * brightness);
}

typedef void (*pattern)(uint len, uint t);
int main()
{
    stdio_init_all();
    sleep_ms(3000);
    Clock clk(CLOCK_REFRESH_RATE);
    int t = 0;
    while (1)
    {
        LedCtrl.update(t);
        LedCtrl.pio.write();
        LedCtrl.pio.wait_until_finish();
        t += 1;
        sleep_us(500);
        printf("delta time %f\n", clk.tick());
    }
}
