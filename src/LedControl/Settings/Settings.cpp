#include "Settings.h"
#if LIB_PICO_STDIO_USB
void enable_usb(bool enable)
{
    stdio_set_driver_enabled(&stdio_usb, enable);
}
#else
void enable_usb(bool enable) {}
#endif

Settings::Settings() : m_mode(Mode::SOUND_BAR), m_max_bright(DEF_MAX_BRIGHT), m_sensitivity(DEF_SENSITIVITY), m_volume_threshold(DEF_VOLUME_THRESHOLD), m_config_temp_value(0), m_machine_volume(100)
{
    if (exist())
    {
        printf("settings file found!\n");
        read();
    }
    else
    {
        printf("settings file not found!\n");
    }
}
void Settings::read()
{
    m_max_bright = settings_flash_buffer[SETTINGS_MAX_BRIGHT_OFFSET];
    m_sensitivity = settings_flash_buffer[SETTINGS_SENSITIVITY_OFFSET];
    m_volume_threshold = settings_flash_buffer[SETTINGS_VOLUME_THRESHOLD_OFFSET];
}
void Settings::write()
{
    uint8_t data[FLASH_PAGE_SIZE];
    data[SETTINGS_EXIST_OFFSET] = 1;
    data[SETTINGS_MAX_BRIGHT_OFFSET] = m_max_bright;
    data[SETTINGS_SENSITIVITY_OFFSET] = m_sensitivity;
    data[SETTINGS_VOLUME_THRESHOLD_OFFSET] = m_volume_threshold;
    enable_usb(false);
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(SETTINGS_WRITE_START, FLASH_SECTOR_SIZE);
    flash_range_program(SETTINGS_WRITE_START, data, FLASH_PAGE_SIZE);
    restore_interrupts(ints);
    enable_usb(true);
}
bool Settings::exist() const
{
    return settings_flash_buffer[SETTINGS_EXIST_OFFSET] == 1;
}
Mode Settings::get_mode() const
{
    return m_mode;
}
void Settings::update_mode()
{
    m_mode = (Mode)(((int)m_mode + 1) % Mode::LENGTH);
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
    printf("reset settings!\n");
    enable_usb(false);
    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(SETTINGS_WRITE_START, FLASH_SECTOR_SIZE);
    restore_interrupts(ints);
    enable_usb(true);
    m_max_bright = DEF_MAX_BRIGHT;
    m_sensitivity = DEF_SENSITIVITY;
    m_volume_threshold = DEF_VOLUME_THRESHOLD;
}
uint8_t Settings::get_config_temp_value() const
{
    return m_config_temp_value;
}
int Settings::get_volume_threshold() const
{
    // return volume threshold between 0 and MAX_VOLUME_THRESHOLD
    // return m_volume_threshold / 100.0f * MAX_VOLUME_THRESHOLD;

    // TODO: fix to machine volume
    return (m_volume_threshold / 100.0f) * MAX_VOLUME_THRESHOLD * std::pow((m_machine_volume / 100.0f), 2); // use the current machine volume. don't know why power 2 but it works (tested)
}
int Settings::get_max_bright() const
{
    // return brightness between 0 and MAX_BRIGHTNESS
    return (m_max_bright / 100.0f) * MAX_BRIGHTNESS;
}
int Settings::get_sensitivity() const
{
    // return sensitivity between 0 and MAX_SENSITIVITY
    // return m_sensitivity / 100.0f * MAX_SENSITIVITY;
    return (m_sensitivity / 100.0f) * MAX_SENSITIVITY * 100.0f / (m_machine_volume + 0.01); // use the current machine volume
}
void Settings::set_config_temp_value(int value)
{
    m_config_temp_value = fix_percent(value);
}
void Settings::set_volume_threshold(int value)
{
    m_volume_threshold = fix_percent(value);
    write();
}
void Settings::set_max_bright(int value)
{
    m_max_bright = fix_percent(value);
    write();
}
void Settings::set_sensitivity(int value)
{
    m_sensitivity = fix_percent(value);
    write();
}
void Settings::set_machine_volume(int value)
{
    m_machine_volume = value;
}
