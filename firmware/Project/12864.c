/******************************************************************************************
* 文件名称 ：12864.c
* 版权     ：
* 模块名称 ：st7920驱动的12864液晶的串行工作方式驱动程序
* cpu      :  stm32f103rct6                       主频：72M
* 作者     ：
* 创建日期 ：2009-10-15
* 功能概要 ：
*----------------------------------------修改历史------------------------------------------
* 当前版本 ：       修改人：                  修改日期：
* 修改说明 ：
******************************************************************************************/

#include "12864.h"
#include "common.h"
#include "systick.h"
#include "hardware_conf.h"

#define SID_H   GPIO_SetBits(LCD12864_PORT,LCD12864_SID)
#define SID_L   GPIO_ResetBits(LCD12864_PORT,LCD12864_SID)

#define CS_H    GPIO_SetBits(LCD12864_PORT,LCD12864_CS)
#define CS_L    GPIO_ResetBits(LCD12864_PORT,LCD12864_CS)

#define SCLK_H  GPIO_SetBits(LCD12864_PORT,LCD12864_SCK)
#define SCLK_L  GPIO_ResetBits(LCD12864_PORT,LCD12864_SCK)

#define x1    0x80
#define x2    0x88
#define y     0x80
#define comm  0
#define dat   1

static void Send_Byte(u8 bbyte);
static void Write_Char(u8 start, u8 ddata);

static uint8_t const  num_lcd[] = {"0123456789 :.-"};

/******************************************************************************************
* 函数名称    ：Lcd_init
* 功能描述    ：初始化LCD
* 参数        ：  参数名称： 输入/输出？ 类型  描述
*
* 返回值      ：
* 作者        ：
* 创建日期    ：2008-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ：     修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void Lcd12864_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_GPIOClockCmd(LCD12864_PORT, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LCD12864_SID | LCD12864_CS | LCD12864_SCK/* | LCD12864_PSB*/;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD12864_PORT, &GPIO_InitStructure);

    // GPIO_ResetBits(LCD12864_PORT, LCD12864_PSB);

}
/******************************************************************************************
* 函数名称    ：Lcd_init
* 功能描述    ：初始化LCD
* 参数        ：  参数名称： 输入/输出？ 类型  描述
*
* 返回值      ：
* 作者        ：
* 创建日期    ：2008-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ：     修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void Lcd_Init(void)
{
    Lcd12864_Config();
    Delay_ms(50); //启动等待，等LCM讲入工作状态
    CS_H;
    Write_Char(0, 0x30); //8 位介面，基本指令集
    Delay_ms(1);
    Write_Char(0, 0x30); //8 位介面，基本指令集
    Write_Char(0, 0x30); //8 位介面，基本指令集
    Write_Char(0, 0x0c); //显示打开，光标关，反白关
    Write_Char(0, 0x01); //清屏，将DDRAM的地址计数器归零
}

/******************************************************************************************
* 函数名称    ：Write_char
* 功能描述    ：写指令或数据
* 参数        ：  参数名称： 输入/输出？ 类型  描述
*
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ：     修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
static void Write_Char(u8 start, u8 ddata)
{
    u8 start_data, Hdata, Ldata;
    if (start == 0)
        start_data = 0xf8; //写指令
    else
        start_data = 0xfa; //写数据

    Hdata = ddata & 0xf0; //取高四位
    Ldata = (ddata << 4) & 0xf0; //取低四位
    Send_Byte(start_data);   //发送起始信号
    Delay_us(10);
    Send_Byte(Hdata);       //发送高四位
    Delay_us(10);
    Send_Byte(Ldata);    //发送低四位
    Delay_us(10);
}

/******************************************************************************************
* 函数名称    ：Send_byte
* 功能描述    ：发送一个字节
* 参数        ：  参数名称： 输入/输出？ 类型  描述
*
* 返回值      ：
* 作者        ：
* 创建日期    ：2009-10-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ：     修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
static void Send_Byte(u8 bbyte)
{
    volatile u8 i;
    for (i = 0; i < 8; i++) {
        if ((bbyte) & 0x80)
            SID_H; //取出最高位
        else
            SID_L;
        SCLK_H;
        Delay_us(100);  //延时 lcd读取数据
        SCLK_L;
        bbyte <<= 1; //左移
    }
}


/******************************************************************************************
* 函数名称    ：LCD12864_Clr_Scr
* 功能描述    ：清屏函数
* 参数        ：  参数名称： 输入/输出？ 类型  描述
*
* 返回值      ：
* 作者        ：
* 创建日期    ：2009-10-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ：     修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void LCD12864_Clr_Scr(void)//清屏函数
{
    Write_Char(0, 0x01);
}

/******************************************************************************************
* 函数名称    ： LCD12864_set_xy
* 功能描述    ：设置LCD显示的起始位置，X为行，Y为列
* 参数        ：  参数名称： 输入/输出？ 类型  描述
*
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ：     修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void LCD12864_Set_XY( u8 X, u8 Y )
{
    u8 address;
    switch (X) {
    case 0:
        address = 0x80 + Y;
        break;
    case 1:
        address = 0x80 + Y;
        break;
    case 2:
        address = 0x90 + Y;
        break;
    case 3:
        address = 0x88 + Y;
        break;
    case 4:
        address = 0x98 + Y;
        break;
    default:
        address = 0x80 + Y;
        break;
    }
    Write_Char(0, address);
}

/******************************************************************************************
* 函数名称    ：LCD12864_Write_string
* 功能描述    ：中英文字符串显示函数
* 参数        ：  参数名称： 输入/输出？ 类型  描述
*
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ：     修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void LCD12864_Write_String(u8 X, u8 Y, uc8  *s)
{
    LCD12864_Set_XY( X, Y );

    while (*s) {
        Write_Char( 1, *s );
        s ++;
        Delay_ms(1);
    }
}

void LCD12864_Write_String_Length(u8 X, u8 Y, uc8  *s, int length)
{
    LCD12864_Set_XY( X, Y );

    while(length--) {
        Write_Char( 1, *s );
        s ++;
        Delay_ms(1);
    }
}

/******************************************************************************************
* 函数名称    ：LCD12864_Write_number
* 功能描述    ：数字显示函数
* 参数        ：  参数名称： 输入/输出？ 类型  描述
*
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ：     修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void LCD12864_Write_Number(u8 s)// 数字显示函数
{
    Write_Char(1, num_lcd[s]);
    Delay_ms(1);
}

/******************************************************************************************
* 函数名称    ：img_disp
* 功能描述    ：显示图形
* 参数        ：  参数名称： 输入/输出？ 类型  描述
*
* 返回值      ：
* 作者        ：
* 创建日期    ：2006-12-19
* 全局变量    ：
* 全局静态变量：
* 局部静态变量：
*----------------------------------------修改历史------------------------------------------
* 当前版本    ：     修改人：                  修改日期：
* 修改说明    ：
******************************************************************************************/
void LCD12864_Display_Img(u8 const *img)
{
    u8 i, j;
    for (j = 0; j < 32; j++) {
        for (i = 0; i < 8; i++) {
            Write_Char(comm, 0x34);
            Delay_ms(10);
            Write_Char(comm, y + j);
            Delay_ms(10);
            Write_Char(comm, x1 + i);
            Delay_ms(10);
            Write_Char(comm, 0x30);
            Delay_ms(10);
            Write_Char(dat, img[j * 16 + i * 2]);
            Delay_ms(10);
            Write_Char(dat, img[j * 16 + i * 2 + 1]);
            Delay_ms(10);
        }
    }
    for (j = 32; j < 64; j++) {
        for (i = 0; i < 8; i++) {
            Write_Char(comm, 0x34);
            Delay_ms(10);
            Write_Char(comm, y + j - 32);
            Delay_ms(10);
            Write_Char(comm, x2 + i);
            Delay_ms(10);
            Write_Char(comm, 0x30);
            Delay_ms(10);
            Write_Char(dat, img[j * 16 + i * 2]);
            Delay_ms(10);
            Write_Char(dat, img[j * 16 + i * 2 + 1]);
            Delay_ms(10);
        }
    }
    Delay_ms(10);
    Write_Char(comm, 0x36);
}
