#pragma once
#include "NeoPixelLed/NeoPixelLed.h"
#include "Settings/Settings.h"
#include <vector>
#define COLOR_DURATION 0.2

class LedControl
{
private:
	std::vector<NeoPixelLed> m_leds;
	Color m_color;

public:
	NeoPio pio;
	Settings settings;

private:
	void start(bool direction);
	void clear();
	void set_buffer();
	void update_sound_bar(int right_avg, int right_max, int left_avg, int left_max);
	void update_sound_route(int right_avg, int right_max, int left_avg, int left_max);
	void update_random_colors(int right_avg, int right_max, int left_avg, int left_max);
	void update_config_brightness(int right_avg, int right_max, int left_avg, int left_max);
	void update_config_sensitivity(int right_avg, int right_max, int left_avg, int left_max);
	void update_config_volume_thresh(int right_avg, int right_max, int left_avg, int left_max);
	void update_off(int right_avg, int right_max, int left_avg, int left_max);

public:
	LedControl();
	void update(int right_avg, int right_max, int left_avg, int left_max);
};
