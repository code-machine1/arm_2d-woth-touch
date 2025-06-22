#include "at32m412_416.h"
#include "bsp_lcd_spi.h"
#include "at32m412_416_wk_config.h"
static dma_init_type dma_init_struct;


void spi_dma_write_bytes(uint8_t *pbuffer, uint32_t length)
{
    volatile uint16_t dummy_data;

    dma_reset(DMA1_CHANNEL2);
    dma_reset(DMA1_CHANNEL3);

    dma_default_para_init(&dma_init_struct);

    /* miso channel */
    dma_init_struct.buffer_size = length;
    dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_base_addr = (uint32_t)&dummy_data;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    dma_init_struct.memory_inc_enable = FALSE;
    dma_init_struct.peripheral_base_addr = (uint32_t)(&SPI2->dt);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
    dma_init_struct.loop_mode_enable = FALSE;
    dma_init(DMA1_CHANNEL2, &dma_init_struct);

    /* mosi channel */
    dma_init_struct.buffer_size = length;
    dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_base_addr = (uint32_t)pbuffer;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    dma_init_struct.memory_inc_enable = TRUE;
    dma_init_struct.peripheral_base_addr = (uint32_t)(&SPI2->dt);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_BYTE;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
    dma_init_struct.loop_mode_enable = FALSE;
    dma_init(DMA1_CHANNEL3, &dma_init_struct);

//  dma_flexible_config(DMA1, FLEX_CHANNEL4, DMA_FLEXIBLE_SPI1_RX);
//  dma_flexible_config(DMA1, FLEX_CHANNEL5, DMA_FLEXIBLE_SPI1_TX);

    spi_i2s_dma_transmitter_enable(SPI2, TRUE);
    spi_i2s_dma_receiver_enable(SPI2, TRUE);

    dma_channel_enable(DMA1_CHANNEL2, TRUE);
    dma_channel_enable(DMA1_CHANNEL3, TRUE);

    /* wait dma send and recv over */
    while(dma_flag_get(DMA1_FDT3_FLAG) == RESET);
    dma_flag_clear(DMA1_FDT3_FLAG);

    dma_channel_enable(DMA1_CHANNEL2, FALSE);
    dma_channel_enable(DMA1_CHANNEL3, FALSE);

    spi_i2s_dma_transmitter_enable(SPI2, FALSE);
    spi_i2s_dma_receiver_enable(SPI2, FALSE);
}

void spi_dma_write_halfwords(uint16_t *pbuffer, uint32_t length)
{
    volatile uint16_t dummy_data;
    dma_init_type dma_init_struct;

    spi_frame_bit_num_set(SPI2, SPI_FRAME_16BIT);

    dma_reset(DMA1_CHANNEL2);
    dma_reset(DMA1_CHANNEL3);

    dma_default_para_init(&dma_init_struct);

    /* miso channel */
    dma_init_struct.buffer_size = length;
    dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_base_addr = (uint32_t)&dummy_data;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_struct.memory_inc_enable = FALSE;
    dma_init_struct.peripheral_base_addr = (uint32_t)(&SPI2->dt);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
    dma_init_struct.loop_mode_enable = FALSE;
    dma_init(DMA1_CHANNEL2, &dma_init_struct);

    /* mosi channel */
    dma_init_struct.buffer_size = length;
    dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_base_addr = (uint32_t)pbuffer;
    dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    dma_init_struct.memory_inc_enable = FALSE;
    dma_init_struct.peripheral_base_addr = (uint32_t)(&SPI2->dt);
    dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    dma_init_struct.peripheral_inc_enable = FALSE;
    dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
    dma_init_struct.loop_mode_enable = FALSE;

    dma_init(DMA1_CHANNEL3, &dma_init_struct);

//  dma_flexible_config(DMA1, FLEX_CHANNEL4, DMA_FLEXIBLE_SPI1_RX);
//  dma_flexible_config(DMA1, FLEX_CHANNEL5, DMA_FLEXIBLE_SPI1_TX);

    spi_i2s_dma_transmitter_enable(SPI2, TRUE);
    spi_i2s_dma_receiver_enable(SPI2, TRUE);

    dma_channel_enable(DMA1_CHANNEL2, TRUE);
    dma_channel_enable(DMA1_CHANNEL3, TRUE);

    /* wait dma send and recv over */
    while(dma_flag_get(DMA1_FDT3_FLAG) == RESET);
    dma_flag_clear(DMA1_FDT3_FLAG);

    dma_channel_enable(DMA1_CHANNEL2, FALSE);
    dma_channel_enable(DMA1_CHANNEL3, FALSE);

    spi_i2s_dma_transmitter_enable(SPI2, FALSE);
    spi_i2s_dma_receiver_enable(SPI2, FALSE);

    spi_frame_bit_num_set(SPI2, SPI_FRAME_8BIT);
}

uint8_t spi_write_read_byte(uint8_t data)
{
    uint8_t retry = 0;

    spi_i2s_dma_transmitter_enable(SPI2, FALSE);
    spi_i2s_dma_receiver_enable(SPI2, FALSE);

    while(spi_i2s_flag_get(SPI2, SPI_I2S_TDBE_FLAG) == RESET )
    {
        if(retry++ > 200)
            return 0;
    }
    spi_i2s_data_transmit(SPI2, data);

    retry = 0;
    while(spi_i2s_flag_get(SPI2, SPI_I2S_RDBF_FLAG) == RESET)
    {
        if(retry++ > 200)
            return 0;
    }

    return spi_i2s_data_receive(SPI2);
}

