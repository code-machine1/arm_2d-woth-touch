#ifndef TOUCH_H
#define TOUCH_H
#include "at32m412_416.h"
#include "perf_counter.h"

// PA4  T_IRQ
// PA5  T_DO
// PA6  T_DIN
// PA7  T_CS
// PB0  T_CLK

//���������Ŷ���

#define XPT2046_PEN          gpio_input_data_bit_read(GPIOA,GPIO_PINS_4)   //T_IRQ

#define XPT2046_MOSI_Set     gpio_bits_set(GPIOA,GPIO_PINS_6)              //T_DIN
#define XPT2046_MOSI_Clr     gpio_bits_reset(GPIOA,GPIO_PINS_6)  

#define XPT2046_MISO         gpio_input_data_bit_read(GPIOA,GPIO_PINS_5)   //T_DO

#define XPT2046_SCK_Set      gpio_bits_set(GPIOB,GPIO_PINS_0)              //T_CLK
#define XPT2046_SCK_Clr      gpio_bits_reset(GPIOB,GPIO_PINS_0)  

#define XPT2046_CS_Set      gpio_bits_set(GPIOA,GPIO_PINS_7)               //T_CS
#define XPT2046_CS_Clr      gpio_bits_reset(GPIOA,GPIO_PINS_7)  


//��������
void XPT2046_TouchInit(void);
void XPT2046_SPI_WriteOneByte(u8 cmd);
u8 XPT2046_ReadXY(void);
 
//��Ŵ�������Ϣ�Ľṹ��
volatile typedef struct
{
    u16 x0; //��������x
    u16 y0; //��������y
    u16 x;  //��������x
    u16 y;  //��������y
}XPT2046_TOUCH;
extern XPT2046_TOUCH xpt2046_touch;
#endif










