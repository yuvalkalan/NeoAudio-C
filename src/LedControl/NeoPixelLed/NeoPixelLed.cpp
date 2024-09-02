#include "NeoPixelLed.h"

NeoPixelLed::NeoPixelLed(uint32_t color, bool rtl)
	: m_color(color), m_rtl(rtl), m_index(0)
{
}

bool NeoPixelLed::update()
{
	m_index += 1;
	return alive();
}

uint32_t NeoPixelLed::color() const
{
	return m_color;
}

int NeoPixelLed::index() const
{
	// center -> edge
	return NUM_PIXELS / 2 + (m_rtl ? (-m_index - 1) : m_index);
	// edge -> center
	// return m_rtl ? m_index : NUM_PIXELS - m_index - 1;
}

bool NeoPixelLed::alive() const
{
	return m_index < NUM_PIXELS / 2;
}
