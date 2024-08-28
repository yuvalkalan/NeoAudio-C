#include "SerialIn.h"

SerialIn::SerialIn() : m_message(""), m_show_overloading(true)
{
}

void SerialIn::reset_bootsel()
{
    reset_usb_boot(0, 0);
}

bool SerialIn::get_show_overloading() const
{
    return m_show_overloading;
}

void SerialIn::update(const Settings &settings)
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
        if (m_message == "show ?")
            printf("show:\n\tconfig\n");
        else if (m_message == "show config")
        {
            printf("settings: \n");
            printf("\tmax bright = %d\n", settings.get_max_bright());
            printf("\tsensitivity = %d\n", settings.get_sensitivity());
            printf("\tvolume threshold = %d\n", settings.get_volume_threshold());
        }
        else if (m_message == "overloading")
            m_show_overloading = true;
        else if (m_message == "!overloading")
            m_show_overloading = false;
        printf("message = %s\n", m_message.c_str());
        m_message = "";
    }
}
