#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "hardware/dma.h"
#include "NeoPio.pio.h"
#include "LedControl/LedControl.h"
#include "Clock/Clock.h"

#define NUM_PIXELS 144
#define WS2812_PIN 0 // default to pin 0
#define CLOCK_REFRESH_RATE 200.0

const float brightness = 0.020f;

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

void pattern_snakes(uint len, uint t)
{
    for (uint i = 0; i < len; ++i)
    {
        uint x = (i + (t >> 1)) % 64;
        if (x < 10)
            led_buffer[i] = urgb_u32(0xff, 0, 0);
        else if (x >= 15 && x < 25)
            led_buffer[i] = urgb_u32(0, 0xff, 0);
        else if (x >= 30 && x < 40)
            led_buffer[i] = urgb_u32(0, 0, 0xff);
        else
            led_buffer[i] = 0;
    }
}

typedef void (*pattern)(uint len, uint t);
int main()
{
    // set_sys_clock_48();
    stdio_init_all();
    sleep_ms(3000);
    Clock clk(CLOCK_REFRESH_RATE);
    const int led_dma_chan = 0;
    dma_channel_config dma_ch0 = dma_channel_get_default_config(led_dma_chan);
    channel_config_set_transfer_data_size(&dma_ch0, DMA_SIZE_32);
    channel_config_set_read_increment(&dma_ch0, true);
    channel_config_set_write_increment(&dma_ch0, false);
    channel_config_set_dreq(&dma_ch0, DREQ_PIO0_TX0);
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
        int dir = (rand() >> 30) & 1 ? 1 : -1;
        puts(dir == 1 ? "(forward)" : "(backward)");
        uint64_t a = time_us_64();
        // pattern_table[pat].pat(NUM_PIXELS, t);
        for (int i = 0; i < 144; ++i)
        {
            pattern_snakes(NUM_PIXELS, t);
            // initiate DMA transfer
            // shiftArray(led_buffer, NUM_PIXELS);
            dma_channel_configure(led_dma_chan, &dma_ch0, &pio->txf[sm], led_buffer, NUM_PIXELS, true);
            dma_channel_wait_for_finish_blocking(led_dma_chan);
            sleep_us(300);
            t += dir;
            clk.tick();
        }
        uint64_t b = time_us_64();
        printf("Time difference: %f seconds\n", (b - a) / 1000000.0);
        // sleep_ms(1000);
    }
}
