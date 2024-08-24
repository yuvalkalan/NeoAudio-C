#pragma once

#include "hardware/pio.h"
#include "hardware/dma.h"
#include "NeoPio.pio.h"

#define NUM_PIXELS 144
#define LEDS_DMA_CHANNEL 0
#define LEDS_SM 0
#define LEDS_GPIO_PIN 0

class NeoPio
{
private:
    PIO pio;
    dma_channel_config dma_config;

public:
    uint32_t buffer[NUM_PIXELS];
    NeoPio();
    void write();
    void wait_until_finish();
};
