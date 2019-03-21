/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】CHIUSIR
【备    注】
【软件版本】V1.0
【最后更新】2016年08月20日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
【交流邮箱】chiusir@163.com
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef _LQOLED_H
#define _LQOLED_H
#include "include.h"
extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LCD_P6x8Str(unsigned char x, unsigned char y, unsigned char ch[]);
extern void LCD_P8x16Str(unsigned char x, unsigned char y, unsigned char ch[]);
extern void LCD_P14x16Str(unsigned char x, unsigned char y, unsigned char ch[]);
extern void LCD_Print(unsigned char x, unsigned char y, unsigned char ch[]);
extern void LCD_PutPixel(unsigned char x, unsigned char y);
extern void LCD_Rectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char gif);
extern void Draw_LQLogo(void);
extern void Draw_LibLogo(void);
extern void Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char * bmp);
//extern void Draw_Road(void);
void Draw_Frame(void);
//显示3位整数和1位小数！（慎用，在主函数循环里面一直跑的话，比较占单片机资源）
void LCD_PrintFloat_31(unsigned char x, unsigned char y, float num);
//显示5位整数
void LCD_PrintInt5(unsigned char x, unsigned char y, int num);
//显示4位整数
void LCD_PrintInt4(unsigned char x, unsigned char y, int num);
//显示3位整数
void LCD_PrintInt3(unsigned char x, unsigned char y, int num);
//OLED图像更新
void OLED_Refresh_Gram(void);
//OLED画点程序
void OLED_DrawPoint(uint8 x, uint8 y, uint8 t);
/// <summary>
///OLED显示二值化图像
///<para>example:  OLED_ShowImage(128,64,120,188,10,image);</para>
///</summary>
/// <param name="ShowSizeX">OLED显示区域大小的X值</param>
/// <param name="ShowSizeY">OLED显示区域大小的Y值</param>
/// <param name="ImageSizeHeight">待显示图像的高度</param>
/// <param name="ImageSizeWidth">待显示图像的宽度</param>
void OLED_ShowImage(int ShowSizeX, int ShowSizeY, int ImageSizeHeight, int ImageSizeWidth, int Gate_To2, unsigned char imagebuff[120][188]);
#endif

