#ifndef _TFTDRIVER_H
#define _TFTDRIVER_H
#include "common.h"
#include "include.h"

#define	TFT_X_MAX	128	//液晶X方宽度
#define TFT_Y_MAX	160 //液晶Y方宽度


//#define DC_PIN		PTB18	//液晶命令位硬件定义
//#define REST_PIN	PTB19 //液晶复位引脚定义

#define DC(x)   	GPIO_Ctrl(PTB,18,x);
#define REST(x) 	GPIO_Ctrl(PTB,19,x);
#define DCInit		GPIO_Init(PTB,18,GPO,1);
#define RESTInit	GPIO_Init(PTB,19,GPO,1);

//-------常用颜色----------
#define RED     	0XF800    //红色
#define GREEN   	0X07E0    //绿色
#define BLUE    	0X001F    //蓝色
#define BRED    	0XF81F
#define GRED    	0XFFE0    //灰色
#define GBLUE   	0X07FF    //
#define BLACK   	0X0000    //黑色
#define WHITE   	0XFFFF    //白色
#define YELLOW  	0xFFE0    //黄色



//定义写字笔的颜色
#define PENCOLOR RED

//定义背景颜色
#define BGCOLOR	 WHITE

void TFT_init(SPIn_e spin, SPIn_PCSn_e pcs);
void dsp_single_colour(int color);
void TFT_showchar(uint16 x, uint16 y, uint8 dat, int PenColor, int BackColor);
void TFT_showstr(uint16 x, uint16 y, uint8 dat[], int PenColor, int BackColor);
int TFT_showfloat(uint16 x, uint16 y, float dat, int ZhengshuWeishu, int XiaoshuWeishu, int PenColor, int BackColor);
int TFT_showUfloat(uint16 x, uint16 y, float dat, int ZhengshuWeishu, int XiaoshuWeishu, int PenColor, int BackColor);
void TFT_showint8(uint16 x, uint16 y, int8 dat, int PenColor, int BackColor);
void TFT_showuint8(uint16 x, uint16 y, uint8 dat, int PenColor, int BackColor);
void TFT_showint16(uint16 x, uint16 y, int16 dat, int PenColor, int BackColor);
void TFT_showuint16(uint16 x, uint16 y, uint16 dat, int PenColor, int BackColor);
void TFT_showimage(const unsigned char *p);
void TFT_showimage_all(const unsigned char *p, int Size_x, int Size_y);
void TFT_showimage_gray(const unsigned char p[40][200], int SizeX_Image, int SizeY_Image, int SizeX_Show, int SizeY_Show);
void displayimage032(uint8 *p);

extern const uint8 tft_ascii[95][16];
extern const uint8  asc2_1608[1520];

#endif