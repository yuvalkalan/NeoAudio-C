#pragma once
#include "NeoPio/NeoPio.h"
#include "Color/Color.h"

class NeoPixelLed
{
private:
	uint32_t m_color;
	bool m_rtl;
	int m_index;

private:
	bool alive() const;

public:
	NeoPixelLed(uint32_t color, bool rtl);
	bool update();
	uint32_t color() const;
	int index() const;
};
