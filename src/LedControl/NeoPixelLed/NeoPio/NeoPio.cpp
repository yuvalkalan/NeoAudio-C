#include "NeoPio.h"

NeoPio::NeoPio() : pio(pio0), dma_config(dma_channel_get_default_config(LEDS_DMA_CHANNEL))
{
    // set pio
    uint offset = pio_add_program(pio, &neopio_program);
    neopio_program_init(pio, LEDS_SM, offset, LEDS_GPIO_PIN);
    // set dma
    channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_32);
    channel_config_set_read_increment(&dma_config, true);
    channel_config_set_write_increment(&dma_config, false);
    channel_config_set_dreq(&dma_config, DREQ_PIO0_TX0);
}
void NeoPio::write()
{
    dma_channel_configure(LEDS_DMA_CHANNEL, &dma_config, &pio->txf[LEDS_SM], buffer, NUM_PIXELS, true);
}
void NeoPio::wait_until_finish()
{
    dma_channel_wait_for_finish_blocking(LEDS_DMA_CHANNEL);
}