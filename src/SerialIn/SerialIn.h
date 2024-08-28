#pragma once

#include <tusb.h>
#include <string>
#include "pico/bootrom.h"
#include <stdio.h>
#include "../LedControl/Settings/Settings.h"

#define CTRL_C 3
#define CTRL_D 4
#define CTRL_X 24
#define CTRL_Z 26

class SerialIn
{
private:
    std::string m_message;
    bool m_show_overloading;
    void reset_bootsel();

public:
    void update(const Settings &settings);
    bool get_show_overloading() const;
    SerialIn();
};