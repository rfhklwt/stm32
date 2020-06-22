#include "LCD5110.h"
#include "lcdlib.h"

/***************************************************************
*  ALIENTEK miniSTM32开发板
*  名称：LCD5110驱动代码
*  作者：rfhklwt
*  修改日期：2017/07/26
*  版本：V2.0
*  功能：LCD函数
***************************************************************/


/***************************************************************
*                     LCD5110  管脚配置
*       LCD_rst     ->    PA7
*       LCD_ce      ->    PA6
*       LCD_dc      ->    PA5
*       LCD_din     ->    PA4
*       LCD_clk     ->    PA3
***************************************************************/

#define LCD_rst PAout(7)
#define LCD_ce  PAout(6)
#define LCD_dc  PAout(5)
#define LCD_din PAout(4)
#define LCD_clk PAout(3)

/***************************************************************
*             名称：LCD_GPIO_Init(IO口的初始化)
*                 功能：配置IO口为推挽输出
***************************************************************/

void LCD_GPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PB端口时钟

  //LCD_rst -> PB4配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_7);

  //LCD_ce -> PB5配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_3);

  //LCD_dc -> PB6配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_4);

  //LCD_din -> PB7配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_5);

  //LCD_clk -> PB8配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_6);
}


/***************************************************************
* 名称：LCD_Send_Data
* 功能：模拟SPI接口时序写入数据到LCD
* 参数：
*       byte ：要写入的1字节数据
***************************************************************/

void LCD_Send_Data (unsigned char byte)
{
  unsigned char i;

  LCD_ce = 0;           //使能片选
  LCD_dc = 1;           //使能写数据

//以下这段是检测byte的每一位的数值,若为1则LCD_din = 1; 若为0则LCD_din = 0;
  for (i = 0; i < 8; i++)
  {
    if (byte & 0x80)    //检查byte的最高位是不是为1
      LCD_din = 1;
    else
      LCD_din = 0;

    LCD_clk = 0;
    byte = byte << 1;   //通过移位来不断读出byte的每一位的值
    LCD_clk = 1;
  }
  LCD_ce = 1;          //片选禁止
}

/***************************************************************
* 名称：LCDSend_Commond
* 功能：模拟SPI接口时序写命令到LCD
* 参数：
*       byte ：要写入的命令
***************************************************************/

void LCD_Send_Commond(unsigned char byte)
{
  unsigned char i;

  LCD_ce = 0;           //使能片选
  LCD_dc = 0;           //使能写数据

  for (i = 0; i < 8; i++)
  {
    if (byte & 0x80)    //检查byte的最高位是不是为1
      LCD_din = 1;
    else
      LCD_din = 0;

    LCD_clk = 0;
    byte = byte << 1;   //通过移位来不断读出byte的每一位的值
    LCD_clk = 1;
  }

  LCD_ce = 1;           //片选禁止
}

/***************************************************************
* 名称：LCD_Set_XY
* 功能：设置LCD坐标函数
* 参数：
*       行 X：0－83
*       列 Y：0－5
***************************************************************/

void LCD_Set_XY (unsigned char x, unsigned char y)
{
  LCD_Send_Commond (y | 0x40);
  LCD_Send_Commond (x | 0x80);
}

/***************************************************************
*                     名称：LCD_clear
*                     功能：LCD5110清屏
***************************************************************/

void LCD_Clear (void)
{
  unsigned char t, k;

  for (t = 0; t < 6; t++)
  {
    for (k = 0; k < 84; k++)
    {
      LCD_Send_Data (0x00);
    }
  }
}

/***************************************************************
*                     名称：LCD5110_Init
*            功能：LCD5110初始化，包括对IO的初始化
***************************************************************/

void LCD5110_Init (void)
{
  /* 初始化IO引脚 */
  LCD_GPIO_Init();

  LCD_rst = 0;
  LCD_rst = 1;                      //产生一个复位脉冲
  LCD_ce = 0;                       //片选使能

  LCD_Send_Commond( 0x21 );         // 使用扩展命令设置LCD模式
  LCD_Send_Commond( 0xbb );         // 设置液晶偏置电压
  LCD_Send_Commond( 0x10 );         // 温度校正
  LCD_Send_Commond( 0x13 );         //  1:48
  LCD_Send_Commond( 0x20 );         // 使用基本命令，V = 0，水平寻址

  /*设置显示模式*/
  LCD_Send_Commond( 0x0C );         // 普通模式
//LCD_Send_Commond( 0x08 );         // 显示空白
//LCD_Send_Commond( 0x09 );         // 显示所有像素
//LCD_Send_Commond( 0x0D );         // 反显模式

  LCD_Clear();                      //清屏幕
  LCD_Set_XY(0,0);                  //地址回归左上角
}

/***************************************************************
* 名称：LCD_Show_Char
* 功能：输出ASCII码代表的字符(点阵大小为6*8)
* 参数：
*       行 X：0－83
*       列 Y：0－5
*       ASCII ：字符的ASCII码,当输入的数值在0~9时，自动识别为数字
***************************************************************/


void LCD_Show_Char(u8 x, u8 y, u8 ASCII)
{
    unsigned char i;
    unsigned int index;

	/* 下面的if判断是为了显示单个字符时，
	   无需再判断参数是字母还是数字 */
	if(ASCII <= 9)
	{
		ASCII += 48;            //ASCII中的48-57是数字0-9
	}

    index = ASCII - 32;			//字模数据是由空格' '开始,空格的ASCII码是32
    LCD_Set_XY(x,y);

    for(i = 0; i < 6; i++)
    {
			LCD_Send_Data(lib_char[index][i]);
    }
}

/***************************************************************
* 名称：LCD_Show_BigChar
* 功能：输出ASCII码代表的大尺寸字符(点阵大小为6*12和8*16)
* 参数：
*       行 X：0－83
*       列 Y：0－5
*       width：字宽
*       height：字高
*       ASCII ：字符的ASCII码,当输入的数值在0~9时，自动识别为数字
***************************************************************/

void LCD_Show_BigChar(u8 x, u8 y, u8 width, u8 height, u8 ASCII)
{
  unsigned char i, j;
  unsigned int index;
  /*下面的if判断是为了显示单个字符时，
     无需再判断参数是字母还是数字 */

  if (ASCII <= 9)
  {
    ASCII += 48;          //ASCII中的48-57是数字0-9
  }
  index = ASCII-32;        //字模数据是由空格' '开始,空格的ASCII码是32
  index *= 2;             //每个字符在lib_char_6_12数组里有2行,因此,要把index乘以2，而且显示时显示2个数组,故j取2个值

  for (j = 0; j < 2; j++)
  {
    LCD_Set_XY (x, y + j);
    for (i = 0; i < width; i++)
    {
      //我们在lcdlib头文件里放了6*12和8*16的字模
      if ((width == 6) && (height == 12))
        LCD_Send_Data (lib_char_6_12[index + j][i]);

      else if ((width == 8) && (height == 16))
        LCD_Send_Data (lib_char_8_16[index + j][i]);
    }

  }
}


/***************************************************************
* 名称：LCD_Show_String
* 功能：显示字符串
* 参数：
*       行 X：0－83
*       列 Y：0－5
*       width：6
*       height：8
*       *str ：要显示的字符串指针
***************************************************************/


void LCD_Show_String(u8 x, u8 y, u8 *str)
{
    unsigned i = 0;
    while(*str)
    {
        LCD_Show_Char(x + i*6, y, *str++);
        i++;
    }
}


/***************************************************************
* 名称：LCD_Show_BigString
* 功能：显示大尺寸字符串
* 参数：
*       行 X：0－83
*       列 Y：0－5
*       width：字宽
*       height：字高
*       *str ：要显示的字符串指针
***************************************************************/


void LCD_Show_BigString(u8 x, u8 y, u8 width, u8 height, u8 *str)
{
    unsigned i = 0;
    while(*str)
    {
        LCD_Show_BigChar(x + (i*width), y, width, height, *str++);
        i++;		//i为要显示第几个字符
    }
}


/***************************************************************
* 名称：LCD_Show_Chinese
* 功能：输出12*12或16*16点阵大小的汉字
* 参数：
*       行 X：0－83
*       列 Y：0－5
*       width：字宽（对于一般汉字来说，字宽和字高是相等的）
*       height：字高
*       index：字模数据在数组中的索引
***************************************************************/

void LCD_Show_Chinese (u8 x, u8 y, u8 width, u8 index)
{
  unsigned char i, j;
  index *= 2;
  for (j = 0; j < 2; j++)
  {
    LCD_Set_XY (x, y + j);
    for (i = 0; i < width; i++)
    {
      if (width == 12)
        LCD_Send_Data (lib_Chinese_12[index + j][i]);
      else if (width == 16)
        LCD_Send_Data (lib_Chinese_16[index + j][i]);
    }
  }
}
