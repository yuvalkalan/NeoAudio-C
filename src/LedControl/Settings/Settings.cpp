#include "Settings.h"
#if LIB_PICO_STDIO_USB
void enable_usb(bool enable)
{
    stdio_set_driver_enabled(&stdio_usb, enable);
}
#else
void enable_usb(bool enable) {}
#endif

Settings::Settings()
{
    if (exist())
    {
        printf("settings file found!\n");
        read();
    }
    else
        printf("settings file not found!\n");
}
void Settings::read()
{
    m_max_bright = settings_flash_buffer[SETTINGS_MAX_BRIGHT_OFFSET];
    m_sensitivity = settings_flash_buffer[SETTINGS_SENSITIVITY_OFFSET];
    m_volume_threshold = settings_flash_buffer[SETTINGS_VOLUMN_THRESHOLD_OFFSET];
}
void Settings::write()
{
    uint8_t data[FLASH_PAGE_SIZE];
    data[SETTINGS_EXIST_OFFSET] = 1;
    data[SETTINGS_MAX_BRIGHT_OFFSET] = m_max_bright;
    data[SETTINGS_SENSITIVITY_OFFSET] = m_sensitivity;
    data[SETTINGS_VOLUMN_THRESHOLD_OFFSET] = m_volume_threshold;
    enable_usb(false);
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(SETTINGS_WRITE_START, FLASH_SECTOR_SIZE);
    flash_range_program(SETTINGS_WRITE_START, data, FLASH_PAGE_SIZE);
    restore_interrupts(ints);
    enable_usb(true);
}
bool Settings::exist()
{
    return settings_flash_buffer[SETTINGS_EXIST_OFFSET] == 1;
}
Mode Settings::get_mode()
{
    return m_mode;
}
void Settings::update_mode()
{
    Mode m_mode = (Mode)(((int)m_mode + 1) % Mode::LENGTH);
    if (m_mode == Mode::CONFIG_SENSITIVITY)
        m_config_temp_value = m_sensitivity;
    else if (m_mode == Mode::CONFIG_BRIGHTNESS)
        m_config_temp_value = m_max_bright;
    else if (m_mode == Mode::CONFIG_VOLUME_THRESH)
        m_config_temp_value = m_volume_threshold;
    else
        m_config_temp_value = 0;
    printf("new mode is %d\n", m_mode);
}
void Settings::reset()
{
    printf("reset settings!");
    enable_usb(false);
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(SETTINGS_WRITE_START, FLASH_SECTOR_SIZE);
    restore_interrupts(ints);
    enable_usb(true);
    m_max_bright = DEF_MAX_BRIGHT;
    m_sensitivity = DEF_SENSITIVITY;
    m_volume_threshold = DEF_VOLUME_THRESHOLD;
}
uint8_t Settings::get_config_temp_value()
{
    return m_config_temp_value;
}
uint8_t Settings::get_volume_threshold()
{
    return m_volume_threshold;
}
uint8_t Settings::get_max_bright()
{
    return m_max_bright;
}
uint8_t Settings::get_sensitivity()
{
    return m_sensitivity;
}

void Settings::set_config_temp_value(uint8_t value)
{
    m_config_temp_value = fix_percent(value);
    write();
}
void Settings::set_volume_threshold(uint8_t value)
{
    m_volume_threshold = fix_percent(value);
    write();
}
void Settings::set_max_bright(uint8_t value)
{
    m_max_bright = fix_percent(value);
    write();
}
void Settings::set_sensitivity(uint8_t value)
{
    m_sensitivity = fix_percent(value);
    write();
}
