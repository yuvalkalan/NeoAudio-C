#include "Clock.h"

Clock::Clock(double refresh_rate) : m_refresh_rate(refresh_rate), m_frequency(1.0 / refresh_rate), m_delta(0)
{
	m_last_frame_time = std::chrono::high_resolution_clock::now();
}

double Clock::tick()
{
	auto current_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = current_time - m_last_frame_time;
	double elapsed_seconds = elapsed.count();
	while ((elapsed_seconds + m_delta) < m_frequency)
	{
		current_time = std::chrono::high_resolution_clock::now();
		elapsed = current_time - m_last_frame_time;
		elapsed_seconds = elapsed.count();
	}
	m_delta += elapsed_seconds - m_frequency;
	m_last_frame_time = current_time;

	return m_delta;
}

void Clock::set_refresh_rate(double refresh_rate)
{
	m_refresh_rate = refresh_rate;
	m_frequency = 1.0 / refresh_rate;
}