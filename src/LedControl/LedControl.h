#pragma once
#include "NeoPixelLed/NeoPixelLed.h"
#include "Settings/Settings.h"
#include <vector>
#include <algorithm>
#define COLOR_DURATION 0.2
#define UPDATE_PARAMS int right_avg, int right_max, int left_avg, int left_max
#define UPDATE_VALUES right_avg, right_max, left_avg, left_max
class LedControl
{
private:
	std::vector<NeoPixelLed> m_leds;
	Color m_color;
	int m_counter;

public:
	NeoPio pio;
	Settings settings;

private:
	void start(bool direction);
	void clear();
	void set_buffer();
	void update_sound_bar(UPDATE_PARAMS);
	void update_sound_route(UPDATE_PARAMS);
	void update_random_colors(UPDATE_PARAMS);
	void update_config_brightness(UPDATE_PARAMS);
	void update_config_sensitivity(UPDATE_PARAMS);
	void update_config_volume_thresh(UPDATE_PARAMS);
	void update_off(UPDATE_PARAMS);

public:
	LedControl();
	void update(UPDATE_PARAMS);
};
