#include "SerialIn.h"

SerialIn::SerialIn()
{
    m_message = "";
}

void SerialIn::reset_bootsel()
{
    reset_usb_boot(0, 0);
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
        if (m_message == "show config")
        {
            printf("settings: \n");
            printf("\tmax bright = %d\n", settings.get_max_bright());
            printf("\tsensitivity = %d\n", settings.get_sensitivity());
            printf("\tvolume threshold = %d\n", settings.get_volume_threshold());
        }
        printf("message = %s\n", m_message.c_str());
        m_message = "";
    }
}
