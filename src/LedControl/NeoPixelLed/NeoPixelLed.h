#pragma once
#include "NeoPio/NeoPio.h"
enum Color
{
	BLACK = 0x000000,
	RED = 0xff0000,
	GREEN = 0x00ff00,
	BLUE = 0x0000ff
};

class NeoPixelLed
{
private:
	Color m_color;
	bool m_rtl;
	int m_index;

private:
	bool alive() const;

public:
	NeoPixelLed(Color color, bool rtl);
	bool update();
	Color color() const;
	int index() const;
};
