#pragma once
#include "NeoPixelLed/NeoPixelLed.h"
#include "NeoPio/NeoPio.h"

class LedControl
{
private:
	NeoPio pio;
	int* m_buffer;
};

