// #include <iostream>
// #include <string>
// #include "LedControl/LedControl.h"
// #include <memory>
// using namespace std;

// #define CLOCK_REFRESH_RATE 50.0
// #define ARRAY_SIZE 50

// void shiftArray(int* arr, int size) {
// 	for (int i = 0; i < size - 1; i++)
// 	{
// 		int offset = size - i - 2;
// 		*(long long*)((int*)arr + offset) <<= 32;
// 	}
// }

// int main() {
// 	int* arr = new int[ARRAY_SIZE];
// 	for (int i = 0; i < ARRAY_SIZE; i++)
// 	{
// 		arr[i] = i + 'A';
// 	}
// 	shiftArray(arr, ARRAY_SIZE);
// 	for (int i = 0; i < ARRAY_SIZE; i++)
// 	{
// 		cout << arr[i] << endl;
// 	}
// }

/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "NeoPio.pio.h"
#define IS_RGBW false
#define NUM_PIXELS 144

// default to pin 16 if the board doesn't have a default WS2812 pin defined
#define WS2812_PIN 0

static inline void init_pio(uint32_t led_count)
{
    pio_sm_put_blocking(pio0, 0, led_count - 1);
}

static inline void put_pixel(uint32_t pixel_grb)
{
    // puts("sending data...");
    // printf("%d", pixel_grb);
    pio_sm_put_blocking(pio0, 0, pixel_grb);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) |
           ((uint32_t)(g) << 16) |
           (uint32_t)(b);
}

void pattern_snakes(uint len, uint t)
{
    for (uint i = 0; i < len; ++i)
    {
        uint x = (i + (t >> 1)) % 64;
        if (x < 10)
            put_pixel(urgb_u32(0xff, 0, 0));
        else if (x >= 15 && x < 25)
            put_pixel(urgb_u32(0, 0xff, 0));
        else if (x >= 30 && x < 40)
            put_pixel(urgb_u32(0, 0, 0xff));
        else
            put_pixel(0);
    }
}

void pattern_random(uint len, uint t)
{
    // if (t % 8)
    //     return;
    for (int i = 0; i < len; ++i)
        put_pixel(rand());
}

void pattern_sparkle(uint len, uint t)
{
    // if (t % 8)
    //     return;
    for (int i = 0; i < len; ++i)
        put_pixel(rand() % 16 ? 0 : 0xffffffff);
}

void pattern_greys(uint len, uint t)
{
    int max = 100; // let's not draw too much current!
    t %= max;
    for (int i = 0; i < len; ++i)
    {
        put_pixel(t * 0x10101);
        if (++t >= max)
            t = 0;
    }
}

typedef void (*pattern)(uint len, uint t);
const struct
{
    pattern pat;
    const char *name;
} pattern_table[] = {
    {pattern_snakes, "Snakes!"},
    // {pattern_random, "Random data"},
    // {pattern_sparkle, "Sparkles"},
    // {pattern_greys, "Greys"},
};

int main()
{
    // set_sys_clock_48();
    stdio_init_all();
    sleep_ms(3000);
    printf("WS2812 Smoke Test, using pin %d", WS2812_PIN);

    // todo get free sm
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &neopio_program);

    neopio_program_init(pio, sm, offset, WS2812_PIN);
    // pio_sm_put_blocking(pio, sm, (NUM_PIXELS - 1));
    // init_pio(NUM_PIXELS);
    int t = 0;
    while (1)
    {
        // while (pio_interrupt_get(pio, 0));
        int pat = rand() % count_of(pattern_table);
        int dir = (rand() >> 30) & 1 ? 1 : -1;
        puts(pattern_table[pat].name);
        puts(dir == 1 ? "(forward)" : "(backward)");
        for (int i = 0; i < 1000; ++i)
        {
            pattern_table[pat].pat(NUM_PIXELS, t);
            sleep_ms(1);
            t += dir;
        }
        puts("testing");
        // sleep_ms(1000);
    }
}
