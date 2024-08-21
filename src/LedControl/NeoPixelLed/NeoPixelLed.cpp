#include "NeoPixelLed.h"


NeoPixelLed::NeoPixelLed()
	: m_color(Color::BLACK), m_rtl(true), m_index(0)
{
}

NeoPixelLed::NeoPixelLed(int color, bool rtl)
	: m_color(color), m_rtl(rtl), m_index(0)
{
}

bool NeoPixelLed::update()
{
	this->m_index += 1;
	return this->alive();
}

int NeoPixelLed::color() const
{
	return this->m_color;
}

int NeoPixelLed::index() const
{
	return this->m_rtl ? this->m_index : NUM_OF_PIXELS - m_index - 1;
}

bool NeoPixelLed::alive() const
{
	return this->m_index < NUM_OF_PIXELS / 2;
}
