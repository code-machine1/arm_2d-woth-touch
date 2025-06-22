#include "wk_system.h"
#include "bsp_lcd_spi.h"
#include "lcd_init.h"


/*
    touch：
	T_CLK :PA1
	T_CS  :PA2
	T_DIN :PA4
	T_DO  :PA5
	T_IRQ :PA6
*/

_lcd_dev lcddev;
/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat)
{
    LCD_CS_Clr();
    spi_write_read_byte(dat);
    LCD_CS_Set();
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
    LCD_Writ_Bus(dat);
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
    LCD_DC_Set();//写数据
    LCD_CS_Clr();
    LCD_Writ_Bus(dat >> 8);
    LCD_Writ_Bus(dat);
    LCD_CS_Set();
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
    LCD_DC_Clr();//写命令
    LCD_CS_Clr();
    LCD_Writ_Bus(dat);
    LCD_CS_Set();
    LCD_DC_Set();//写数据
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    if (USE_HORIZONTAL == 0)
    {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);//储存器写
    }
    else if (USE_HORIZONTAL == 1)
    {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA(y1 + 80);
        LCD_WR_DATA(y2 + 80);
        LCD_WR_REG(0x2c);//储存器写
    }
    else if (USE_HORIZONTAL == 2)
    {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA(x1);
        LCD_WR_DATA(x2);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);//储存器写
    }
    else
    {
        LCD_WR_REG(0x2a);//列地址设置
        LCD_WR_DATA(x1 + 80);
        LCD_WR_DATA(x2 + 80);
        LCD_WR_REG(0x2b);//行地址设置
        LCD_WR_DATA(y1);
        LCD_WR_DATA(y2);
        LCD_WR_REG(0x2c);//储存器写
    }
}

void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}

void LCD_direction(u8 direction)
{
    lcddev.setxcmd = 0x2A;
    lcddev.setycmd = 0x2B;
    lcddev.wramcmd = 0x2C;

    switch (direction)
    {
    case 0:
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;
        LCD_WriteReg(0x36, (1 << 3) | (0 << 6) | (0 << 7)); //BGR==1,MY==0,MX==0,MV==0
        break;

    case 1:
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;
        LCD_WriteReg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); //BGR==1,MY==1,MX==0,MV==1
        break;

    case 2:
        lcddev.width = LCD_W;
        lcddev.height = LCD_H;
        LCD_WriteReg(0x36, (1 << 3) | (1 << 6) | (1 << 7)); //BGR==1,MY==0,MX==0,MV==0
        break;

    case 3:
        lcddev.width = LCD_H;
        lcddev.height = LCD_W;
        LCD_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 5)); //BGR==1,MY==1,MX==0,MV==1
        break;

    default:
        break;
    }
}


void LCD_Init(void)
{
#if 0
    //lcd_spi_and_gpio_init();//初始化GPIO和SPI
    LCD_RES_Clr();  //复位
    wk_delay_ms(200);
    LCD_RES_Set();
    wk_delay_ms(120);
    LCD_BLK_Set();//打开背光
    wk_delay_ms(100);
    LCD_WR_REG(0x3A); //65k mode RGB565
    LCD_WR_DATA8(0x05);
    LCD_WR_REG(0xC5);
    LCD_WR_DATA8(0x1A);
    LCD_WR_REG(0x36);

    if (0 == USE_HORIZONTAL)
    {
        LCD_WR_DATA8(0x00);
    }
    else if (1 == USE_HORIZONTAL)
    {
        LCD_WR_DATA8(0x60);
    }
    else if (2 == USE_HORIZONTAL)
    {
        L
    }
    else if (3 == USE_HORIZONTAL)
        ;

    //    if(USE_HORIZONTAL==0)LCD_WR_DATA8(0x00);
    //    else if(USE_HORIZONTAL==1)LCD_WR_DATA8(0xC0);
    //    else if(USE_HORIZONTAL==2)LCD_WR_DATA8(0x70);
    //    else LCD_WR_DATA8(0xA0);
    LCD_WR_REG(0xb2);       //Porch Setting
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x33);
    LCD_WR_REG(0xb7);           //Gate Control
    LCD_WR_DATA8(0x05);         //12.2v   -10.43v
    LCD_WR_REG(0xBB);//VCOM
    LCD_WR_DATA8(0x3F);
    LCD_WR_REG(0xC0); //Power control
    LCD_WR_DATA8(0x2c);
    LCD_WR_REG(0xC2);       //VDV and VRH Command Enable
    LCD_WR_DATA8(0x01);
    LCD_WR_REG(0xC3);           //VRH Set
    LCD_WR_DATA8(0x0F);     //4.3+( vcom+vcom offset+vdv)
    LCD_WR_REG(0xC4);           //VDV Set
    LCD_WR_DATA8(0x20);             //0v
    LCD_WR_REG(0xC6);               //Frame Rate Control in Normal Mode
    LCD_WR_DATA8(0X01);         //111Hz
    LCD_WR_REG(0xd0);               //Power Control 1
    LCD_WR_DATA8(0xa4);
    LCD_WR_DATA8(0xa1);
    LCD_WR_REG(0xE8);               //Power Control 1
    LCD_WR_DATA8(0x03);
    LCD_WR_REG(0xE9);               //Equalize time control
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x08);
    //---------------ST7789V gamma setting-------------//
    LCD_WR_REG(0xE0); //Set Gamma
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x14);
    LCD_WR_DATA8(0x28);
    LCD_WR_DATA8(0x33);
    LCD_WR_DATA8(0x3F);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x14);
    LCD_WR_DATA8(0x28);
    LCD_WR_DATA8(0x30);
    LCD_WR_REG(0XE1); //Set Gamma
    LCD_WR_DATA8(0xD0);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x24);
    LCD_WR_DATA8(0x32);
    LCD_WR_DATA8(0x32);
    LCD_WR_DATA8(0x3B);
    LCD_WR_DATA8(0x14);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x28);
    LCD_WR_DATA8(0x2F);
    LCD_WR_REG(0x21);
    LCD_WR_REG(0x11); //Exit Sleep // 退出睡眠模式
    wk_delay_ms(120);
    LCD_WR_REG(0x29); //Display on // 开显示
#endif
#if 1
    //LCD_GPIOInit();//LCD GPIO初始化
    LCD_RES_Clr();  //复位
    wk_delay_ms(200);
    LCD_RES_Set();
    wk_delay_ms(120);
    LCD_BLK_Set();//打开背光
    wk_delay_ms(100);
    //*************2.4inch ILI9341初始化**********//
    LCD_direction(1);
    LCD_WR_REG(0xCF);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0xD9); //0xC1
    LCD_WR_DATA8(0X30);
    LCD_WR_REG(0xED);
    LCD_WR_DATA8(0x64);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0X12);
    LCD_WR_DATA8(0X81);
    LCD_WR_REG(0xE8);
    LCD_WR_DATA8(0x85);
    LCD_WR_DATA8(0x10);
    LCD_WR_DATA8(0x7A);
    LCD_WR_REG(0xCB);
    LCD_WR_DATA8(0x39);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x34);
    LCD_WR_DATA8(0x02);
    LCD_WR_REG(0xF7);
    LCD_WR_DATA8(0x20);
    LCD_WR_REG(0xEA);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_REG(0xC0);    //Power control
    LCD_WR_DATA8(0x1B);   //VRH[5:0]
    LCD_WR_REG(0xC1);    //Power control
    LCD_WR_DATA8(0x12);   //SAP[2:0];BT[3:0] 0x01
    LCD_WR_REG(0xC5);    //VCM control
    LCD_WR_DATA8(0x08); 	 //30
    LCD_WR_DATA8(0x26); 	 //30
    LCD_WR_REG(0xC7);    //VCM control2
    LCD_WR_DATA8(0XB7);
    LCD_WR_REG(0x36);    // Memory Access Control
    LCD_WR_DATA8(0x08);
    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x05);
    LCD_WR_REG(0xB1);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x1A);
    LCD_WR_REG(0xB6);    // Display Function Control
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0xA2);
    LCD_WR_REG(0xF2);    // 3Gamma Function Disable
    LCD_WR_DATA8(0x00);
    LCD_WR_REG(0x26);    //Gamma curve selected
    LCD_WR_DATA8(0x01);
    LCD_WR_REG(0xE0);    //Set Gamma
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x1D);
    LCD_WR_DATA8(0x1A);
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x49);
    LCD_WR_DATA8(0X66);
    LCD_WR_DATA8(0x3B);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x01);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x04);
    LCD_WR_REG(0XE1);    //Set Gamma
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x18);
    LCD_WR_DATA8(0x1D);
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x36);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x4C);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x2E);
    LCD_WR_DATA8(0x2F);
    LCD_WR_DATA8(0x05);
    LCD_WR_REG(0x2B);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x01);
    LCD_WR_DATA8(0x3f);
    LCD_WR_REG(0x2A);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0xef);
    LCD_WR_REG(0x21);
    LCD_WR_REG(0x11); //Exit Sleep
    wk_delay_ms(120);
    LCD_WR_REG(0x29); //display on
#endif
}
