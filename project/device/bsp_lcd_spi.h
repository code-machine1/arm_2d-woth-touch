#ifndef __BSP_LCD_SPI__
#define __BSP_LCD_SPI__

#include "at32m412_416.h"

#define LCD_RES_Clr()   gpio_bits_reset(GPIOF, GPIO_PINS_9)//RES(RESET)
#define LCD_RES_Set()   gpio_bits_set(GPIOF, GPIO_PINS_9)

#define LCD_DC_Clr()    gpio_bits_reset(GPIOC, GPIO_PINS_13)//DC(DATA or CMD)
#define LCD_DC_Set()    gpio_bits_set(GPIOC, GPIO_PINS_13)

#define LCD_CS_Clr()    gpio_bits_reset(GPIOF, GPIO_PINS_8)//CS1(LCDÆ¬Ñ¡)
#define LCD_CS_Set()    gpio_bits_set(GPIOF, GPIO_PINS_8)

#define LCD_BLK_Clr()   gpio_bits_reset(GPIOF, GPIO_PINS_10)//BLK(±³¹â¿ØÖÆ)
#define LCD_BLK_Set()   gpio_bits_set(GPIOF, GPIO_PINS_10)


#define LCD_SCK_Clr()   gpio_bits_reset(GPIOB, GPIO_PINS_3)//SCK(RESET)
#define LCD_SCK_Set()   gpio_bits_set(GPIOB, GPIO_PINS_3)

#define LCD_SDA_Clr()   gpio_bits_reset(GPIOB, GPIO_PINS_5)//SDA(RESET)
#define LCD_SDA_Set()   gpio_bits_set(GPIOB, GPIO_PINS_5)


void lcd_spi_and_gpio_init(void);
void spi_dma_write_bytes(uint8_t *pbuffer, uint32_t length);
void spi_dma_write_halfwords(uint16_t *pbuffer, uint32_t length);
uint8_t spi_write_read_byte(uint8_t data);

#endif
