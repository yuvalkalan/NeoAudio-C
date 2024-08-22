#pragma once
#include "NeoPixelLed/NeoPixelLed.h"
#include "NeoPio/NeoPio.h"
#define NUM_PIXELS 144

int32_t led_buffer[NUM_PIXELS];
class LedControl
{
private:
	NeoPio pio;
	int *m_buffer;
};
