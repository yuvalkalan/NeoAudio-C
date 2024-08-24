#pragma once
#include "pico/stdlib.h"
#include <chrono>
#define MAX_DOUBLE_CLICK_DELTA 0.5
#define HOLD_DOWN_THRESHOLD 0.3
class Button
{
private:
    int m_pin;                                                        // gpio
    bool m_value;                                                     // raw value
    std::chrono::high_resolution_clock::time_point m_last_click_time; // last click timer
    std::chrono::high_resolution_clock::time_point m_press_time;      // hold down timer
    bool m_have_click_time = false;                                   // have click time flag
    bool m_have_press_time = false;                                   // have press time flag
    bool m_has_changed = false;                                       // value change flag
    bool m_has_click = false;                                         // click flag
    bool m_has_double_click = false;                                  // double click flag
    bool m_has_hold_down = false;                                     // hold flag
    bool m_got_hold_down = false;                                     // hold lock
private:
    void reset();

public:
    Button(int pin);
    void update();
    bool is_down();
    bool is_up();
    bool clicked();
    bool double_clicked();
    bool hold_down();
};
