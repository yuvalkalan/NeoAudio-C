#pragma once
#define NUM_OF_PIXELS 144

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
	int m_color;
	bool m_rtl;
	int m_index;

private:
	bool alive() const;

public:
	NeoPixelLed();
	NeoPixelLed(int color, bool rtl);
	bool update();
	int color() const;
	int index() const;
};
