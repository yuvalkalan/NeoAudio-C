#pragma once

#include <stdio.h>
#include <pico/stdlib.h>
#include "hardware/flash.h"
#include "hardware/sync.h"
// settings flash buffer location
#define SETTINGS_WRITE_START (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE)
#define SETTINGS_READ_START (SETTINGS_WRITE_START + XIP_BASE)
// ---------------------------------------------------------------------------
// max settings configuration
#define MAX_SENSITIVITY 100
#define MAX_BRIGHTNESS 255
#define MAX_VOLUME_THRESHOLD 65535
// ---------------------------------------------------------------------------
// default settings configuration (percents from max)
#define DEF_MAX_BRIGHT 20
#define DEF_SENSITIVITY 30
#define DEF_VOLUME_THRESHOLD 12
// ---------------------------------------------------------------------------
// settings file contant offsets
#define SETTINGS_EXIST_OFFSET 0
#define SETTINGS_MAX_BRIGHT_OFFSET 1
#define SETTINGS_SENSITIVITY_OFFSET 2
#define SETTINGS_VOLUMN_THRESHOLD_OFFSET 3
// ---------------------------------------------------------------------------
static const uint8_t *settings_flash_buffer = (const uint8_t *)SETTINGS_READ_START;
void enable_usb(bool enable);
enum Mode
{
    SOUND_BAR,
    SOUND_ROUTE,
    RANDOM_COLOR,
    CONFIG_BRIGHTNESS,
    CONFIG_SENSITIVITY,
    CONFIG_VOLUME_THRESH,
    OFF,

    LENGTH // always set this item to be last!
};

uint8_t inline fix_percent(int value)
{
    if (value < 0)
        return 0;
    else if (value > 100)
        return 100;
    return value;
}

class Settings
{
private:
    Mode m_mode;
    uint8_t m_max_bright;
    uint8_t m_sensitivity;
    uint8_t m_volume_threshold;
    uint8_t m_config_temp_value;

private:
    // file operation
    void read();
    void write();
    bool exist() const;

public:
    Settings();
    // getters
    Mode get_mode() const;
    uint8_t get_config_temp_value() const;
    int get_volume_threshold() const;
    int get_max_bright() const;
    int get_sensitivity() const;
    // setters
    void reset();
    void update_mode();
    void set_config_temp_value(int value);
    void set_volume_threshold(int value);
    void set_max_bright(int value);
    void set_sensitivity(int value);
};
