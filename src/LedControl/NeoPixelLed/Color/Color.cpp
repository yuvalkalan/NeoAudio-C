#include "Color.h"

enum BaseColor
{
    BLACK = 0x000000,
    WHITE = 0xffffff,
    RED = 0x00ff00,
    GREEN = 0xff0000,
    BLUE = 0x0000ff,
    YELLOW = 0xffff00,
    MAGENTA = 0x00ffff,
    AQUA = 0xff00ff,
    SOFT_PINK = 0xB6FFC1,
    WARM_WHITE = 0xF4FFE5,
    DEEP_RED = 0x008B00,
    LAVENDER = 0xE6E6FA,
    PEACH = 0xDAFFB9,
    NEON_RED = 0x31FF31,
    NEON_BLUE = 0x511FFF,
    NEON_GREEN = 0xFF3914
};

// uint32_t COLORS[] = {
//     RED,
//     GREEN,
//     BLUE,
//     //  YELLOW,
//     MAGENTA,
//     //  AQUA
// };
uint32_t COLORS[] = {
    RED,
    GREEN,
    BLUE,
    YELLOW,
    MAGENTA,
    AQUA,
    //   DEEP_RED, PEACH, NEON_RED, NEON_BLUE, NEON_GREEN
};

static uint32_t inline pick_random_color(uint32_t current_color, uint32_t next_color)
{
    // Size of the array
    int size = sizeof(COLORS) / sizeof(COLORS[0]);

    // Generate a random index
    uint32_t color = current_color;
    while (color == current_color || color == next_color)
    {
        int randomIndex = std::rand() % size;

        // Get the random element from the array
        color = COLORS[randomIndex];
    }
    return color;
}
static uint8_t inline byte_fade(uint8_t start, uint8_t end, double delta)
{
    return start + (end - start) * delta;
}
static uint32_t inline led_fade(int start, int end, double delta, double brightness)
{
    uint8_t r1 = (start >> 8) & 0xFF;
    uint8_t g1 = (start >> 16) & 0xFF;
    uint8_t b1 = start & 0xFF;

    uint8_t r2 = (end >> 8) & 0xFF;
    uint8_t g2 = (end >> 16) & 0xFF;
    uint8_t b2 = end & 0xFF;

    uint8_t r = byte_fade(r1, r2, delta) * brightness;
    uint8_t g = byte_fade(g1, g2, delta) * brightness;
    uint8_t b = byte_fade(b1, b2, delta) * brightness;
    return (r << 8) | (g << 16) | b;
}

Color::Color() : m_color(BLACK), m_next_color(pick_random_color(m_color, m_next_color)), m_start_time(std::chrono::high_resolution_clock::now())
{
}
void Color::gen()
{
    auto current = m_color;
    m_color = m_next_color;
    m_next_color = pick_random_color(current, m_color);
    m_start_time = std::chrono::high_resolution_clock::now();
}
uint32_t Color::get(double duration, int max_bright)
{
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> delta = current_time - m_start_time;
    double delta_seconds = delta.count();
    if (delta_seconds > duration)
    {
        gen();
        delta_seconds = 0;
    }
    double brightness = max_bright / 255.0f;
    double fade_progress = delta_seconds / duration;
    uint32_t new_color = led_fade(m_color, m_next_color, fade_progress, brightness);
    return new_color;
}
