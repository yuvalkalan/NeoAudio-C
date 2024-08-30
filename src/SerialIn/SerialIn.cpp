#include "SerialIn.h"

SerialIn::SerialIn() : m_message(""), m_show_overloading(true)
{
}
void SerialIn::reset_bootsel()
{
    reset_usb_boot(0, 0);
}
void SerialIn::show_command(Settings &settings, const std::string &param)
{
    if (param == "?")
    {
        printf("%s\n", PARAM_CONFIG);
        printf("%s\n", PARAM_OVERLOADING);
    }
    else if (param == PARAM_OVERLOADING)
    {
        printf("%s=%d", PARAM_OVERLOADING, m_show_overloading);
    }
    else if (param == PARAM_CONFIG)
    {
        printf("settings: \n");
        printf("\tmax bright = %d\n", settings.get_max_bright());
        printf("\tsensitivity = %d\n", settings.get_sensitivity());
        printf("\tvolume threshold = %d\n", settings.get_volume_threshold());
    }
    else
    {
        printf("Invalid show command!\n");
    }
}
void SerialIn::set_command(Settings &settings, const std::string &param, int value)
{
    if (param == "?")
    {
        printf("%s (int)\n", PARAM_MAX_BRIGHT);
        printf("%s (int)\n", PARAM_SENSITIVITY);
        printf("%s (int)\n", PARAM_VOLUME_THRESHOLD);
        printf("%s (int)\n", PARAM_MACHINE_VOLUME);
        printf("%s (bool)\n", PARAM_OVERLOADING);
    }
    else if (param == PARAM_MAX_BRIGHT)
    {
        // TODO
    }
    else if (param == PARAM_SENSITIVITY)
    {
        // TODO
    }
    else if (param == PARAM_VOLUME_THRESHOLD)
    {
        // TODO
    }
    else if (param == PARAM_OVERLOADING)
    {
        m_show_overloading = value ? true : false;
        printf("set %s = %d", PARAM_OVERLOADING, m_show_overloading);
    }
    else if (param == PARAM_MACHINE_VOLUME)
    {
        settings.set_machine_volume(value);
    }
    else
    {
        printf("Invalid set command!\n");
    }
}
void SerialIn::delete_command(Settings &settings, const std::string &param)
{
}
void SerialIn::add_command(Settings &settings, const std::string &param, int value)
{
}
void SerialIn::process_command(Settings &settings)
{
    std::string command, param, value_str;
    int value;

    // Find the first space to extract the command
    size_t pos = m_message.find(' ');
    if (pos == std::string::npos)
    {
        printf("Invalid command format!\n");
        return;
    }
    command = m_message.substr(0, pos);
    std::string rest = m_message.substr(pos + 1);
    if (command == CMD_SHOW || command == CMD_DELETE)
    {
        // For "show <param>" and "delete <param>"
        param = rest;
        if (command == CMD_SHOW)
        {
            show_command(settings, param);
        }
        else if (command == CMD_DELETE)
        {
            delete_command(settings, param);
        }
    }
    else if (command == CMD_SET || command == CMD_ADD)
    {
        // For "set <param>=<value>" and "add <param>=<value>"
        size_t equal_pos = rest.find('=');
        if (equal_pos == std::string::npos)
        {
            printf("Invalid command format!\n");
            return;
        }
        param = rest.substr(0, equal_pos);
        value_str = rest.substr(equal_pos + 1);
        value = std::stoi(value_str); // Convert value_str to integer

        if (command == CMD_SET)
        {
            set_command(settings, param, value);
        }
        else if (command == CMD_ADD)
        {
            add_command(settings, param, value);
        }
    }
}
bool SerialIn::get_show_overloading() const
{
    return m_show_overloading;
}
void SerialIn::update(Settings &settings)
{
    // Check to see if anything is available in the serial receive buffer
    while (tud_cdc_available())
    {
        char chr = getchar();
        if (chr == CTRL_C)
            reset_bootsel();
        // printf("char is %c (value=%d)\n", chr, chr);
        m_message += chr;
    }
    if (m_message.length() != 0)
    {
        process_command(settings);
        m_message = "";
        // if (m_message == "show ?")
        //     printf("show:\n\tconfig\n");
        // else if (m_message == "show config")
        // {
        //     printf("settings: \n");
        //     printf("\tmax bright = %d\n", settings.get_max_bright());
        //     printf("\tsensitivity = %d\n", settings.get_sensitivity());
        //     printf("\tvolume threshold = %d\n", settings.get_volume_threshold());
        // }
        // else if (m_message == "overloading")
        //     m_show_overloading = true;
        // else if (m_message == "!overloading")
        //     m_show_overloading = false;
        // printf("message = %s\n", m_message.c_str());
        // m_message = "";
    }
}
