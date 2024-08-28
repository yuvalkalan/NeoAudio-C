#include "SerialIn.h"

SerialIn::SerialIn()
{
    m_message = "";
}

void SerialIn::reset_bootsel()
{
    reset_usb_boot(0, 0);
}

void SerialIn::update()
{
    // Check to see if anything is available in the serial receive buffer
    while (tud_cdc_available())
    {
        char chr = getchar();
        if (chr == CTRL_C)
            reset_bootsel();
        printf("char is %c (value=%d)\n", chr, chr);
        m_message += chr;
    }
    if (m_message.length() != 0)
    {
        printf("message = %s", m_message.c_str());
        m_message = "";
    }
}
