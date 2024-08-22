#pragma once
#include "NeoPixelLed/NeoPixelLed.h"

#include <vector>

class LedControl
{
private:
	std::vector<NeoPixelLed> leds;

public:
	NeoPio pio;

private:
	void start(bool direction);
	void clear();
	void set_buffer();

public:
	LedControl();
	void update(int index);
};
