#pragma once
#include <chrono>

class Clock
{
private:
	double m_refresh_rate;
	double m_frequency;
	double m_delta;
	std::chrono::high_resolution_clock::time_point m_last_frame_time;

public:
	Clock(double refresh_rate);
	double tick();
	void set_refresh_rate(double refresh_rate);
};