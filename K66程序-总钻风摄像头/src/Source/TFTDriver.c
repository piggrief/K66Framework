#include "common.h"
#include "include.h"
SPIn_e TFTSPI;
SPIn_PCSn_e TFTSPICS;

void tft_delay(long t)
{
	while (t--);
}

void  Lcd_WriteIndex(uint8 dat)			//写命令
{
	DC(0);
	spi_mosi(TFTSPI, TFTSPICS, &dat, &dat, 1);

}

void Lcd_WriteData(uint8 dat)			//写数据
{
	DC(1);
	spi_mosi(TFTSPI, TFTSPICS, &dat, &dat, 1);
}

void  LCD_WriteData_16Bit(uint16 dat)	//向液晶屏写一个16位数据
{
	uint8 h, l;
	h = dat >> 8;
	l = (uint8)dat;

	DC(1);
	spi_mosi(TFTSPI, TFTSPICS, &h, &h, 1); 	//写入高8位数据
	spi_mosi(TFTSPI, TFTSPICS, &l, &l, 1);	//写入低8位数据
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶坐标设置
//  @param      x_start     	坐标x方向的起点
//  @param      y_start     	坐标y方向的起点
//  @param      x_end       	坐标x方向的终点
//  @param      y_end       	坐标y方向的终点
//  @return     void
//  @since      v1.0
//  Sample usage:               Lcd_SetRegion(0,0,10,10);// x、y的起点都是0，终点都是10
//-------------------------------------------------------------------------------------------------------------------
void Lcd_SetRegion(unsigned int x_start, unsigned int y_start, unsigned int x_end, unsigned int y_end)
{
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_start + 2);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_end + 2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x01);
	Lcd_WriteData(y_start + 1);
	Lcd_WriteData(0x01);
	Lcd_WriteData(y_end + 1);

	Lcd_WriteIndex(0x2c);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶清屏函数
//  @param      color     		颜色设置
//  @return     void
//  @since      v1.0
//  Sample usage:               dsp_single_colour(YELLOW);// 全屏设置为黄色
//-------------------------------------------------------------------------------------------------------------------
void dsp_single_colour(int color)
{
	uint8 i, j;
	Lcd_SetRegion(0, 0, TFT_X_MAX - 1, TFT_Y_MAX - 1);
	for (i = 0; i<TFT_Y_MAX; i++)
	for (j = 0; j<TFT_X_MAX; j++)
		LCD_WriteData_16Bit(color);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶初始化
//  @return     void
//  @since      v1.0
//  Sample usage:               
//-------------------------------------------------------------------------------------------------------------------
void TFT_init(SPIn_e spin, SPIn_PCSn_e pcs)
{
	(void)spi_init(spin, pcs, MASTER, 25 * 1000 * 1000);//硬件SPI初始化

	TFTSPI = spin;
	TFTSPICS = pcs;

	DCInit
	RESTInit

	REST(0);
	tft_delay(999999);
	REST(1);
	tft_delay(999999);
	Lcd_WriteIndex(0x11);
	tft_delay(999999);

	Lcd_WriteIndex(0xB1);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);

	Lcd_WriteIndex(0xB2);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);

	Lcd_WriteIndex(0xB3);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);
	Lcd_WriteData(0x01);
	Lcd_WriteData(0x2C);
	Lcd_WriteData(0x2D);

	Lcd_WriteIndex(0xB4);
	Lcd_WriteData(0x07);

	Lcd_WriteIndex(0xC0);
	Lcd_WriteData(0xA2);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x84);
	Lcd_WriteIndex(0xC1);
	Lcd_WriteData(0xC5);

	Lcd_WriteIndex(0xC2);
	Lcd_WriteData(0x0A);
	Lcd_WriteData(0x00);

	Lcd_WriteIndex(0xC3);
	Lcd_WriteData(0x8A);
	Lcd_WriteData(0x2A);
	Lcd_WriteIndex(0xC4);
	Lcd_WriteData(0x8A);
	Lcd_WriteData(0xEE);

	Lcd_WriteIndex(0xC5);
	Lcd_WriteData(0x0E);

	Lcd_WriteIndex(0x36);
	Lcd_WriteData(0xC0);
	Lcd_WriteData(0xC8);

	Lcd_WriteIndex(0xe0);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x1a);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x18);
	Lcd_WriteData(0x2f);
	Lcd_WriteData(0x28);
	Lcd_WriteData(0x20);
	Lcd_WriteData(0x22);
	Lcd_WriteData(0x1f);
	Lcd_WriteData(0x1b);
	Lcd_WriteData(0x23);
	Lcd_WriteData(0x37);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x07);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x10);

	Lcd_WriteIndex(0xe1);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x1b);
	Lcd_WriteData(0x0f);
	Lcd_WriteData(0x17);
	Lcd_WriteData(0x33);
	Lcd_WriteData(0x2c);
	Lcd_WriteData(0x29);
	Lcd_WriteData(0x2e);
	Lcd_WriteData(0x30);
	Lcd_WriteData(0x30);
	Lcd_WriteData(0x39);
	Lcd_WriteData(0x3f);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x07);
	Lcd_WriteData(0x03);
	Lcd_WriteData(0x10);

	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00 + 2);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80 + 2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00 + 3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x80 + 3);

	Lcd_WriteIndex(0xF0);
	Lcd_WriteData(0x01);
	Lcd_WriteIndex(0xF6);
	Lcd_WriteData(0x00);

	Lcd_WriteIndex(0x3A);
	Lcd_WriteData(0x05);


	Lcd_WriteIndex(0x29);

	dsp_single_colour(0xffff);//全白
}

void TFT_showchar(uint16 x, uint16 y, uint8 dat, int PenColor, int BackColor)
{
	uint8 i, j;
	uint8 temp;
	for (i = 0; i<16; i++)
	{
		Lcd_SetRegion(x, y + i, x + 7, y + i);
		temp = tft_ascii[dat - 32][i];//减32因为是取模是从空格开始取得 空格在ascii中序号是32
		for (j = 0; j<8; j++)
		{
			if (temp & 0x01)	LCD_WriteData_16Bit(PenColor);
			else			LCD_WriteData_16Bit(BackColor);
			temp >>= 1;
		}
	}
}


void TFT_showstr(uint16 x, uint16 y, uint8 dat[], int PenColor, int BackColor)
{
	//0~9行，
	uint16 j;

	j = 0;
	while (dat[j] != '\0')
	{
		TFT_showchar(x + 8 * j, y * 16, dat[j], PenColor, BackColor);
		j++;
	}
}
int TFT_Pow(int x, int cimi)
{
	int i = 0;
	int sum = 1;
	for (i = 0; i < cimi; i++)
	{
		sum *= x;
	}
	return sum;
}

int TFT_showfloat(uint16 x, uint16 y, float dat, int ZhengshuWeishu,int XiaoshuWeishu, int PenColor, int BackColor)
{
	int flag_StartShow = 0;
	int flag_fushu = 0;
	int weishu_buff = ZhengshuWeishu - 1;
	int num = 0;
	int i = 0;
	int j = 0;
	if (dat < 0)
	{
		flag_fushu = 1;
		dat = -dat;
	}
	while (1)
	{
		num = (int)(dat) / (TFT_Pow(10, weishu_buff)) % 10;
		if (flag_StartShow == 0 && num != 0)
		{
			flag_StartShow = 1;
			if (flag_fushu == 1)
			{
				TFT_showchar(x + (8 * i), y * 16, '-', PenColor, BackColor);
			}
			else
			{
				TFT_showchar(x + (8 * i), y * 16, '+', PenColor, BackColor);
			}
			i++;
		}
		if (flag_StartShow == 1)
		{
			TFT_showchar(x + (8 * i), y * 16, '0' + num, PenColor, BackColor);
			i++;
		}
		else
		{
		}

		weishu_buff--;
		if (weishu_buff < 0)
		{
			TFT_showchar(x + (8 * i), y * 16, '.', PenColor, BackColor);
			i++;
			break;
		}
	}
	weishu_buff = XiaoshuWeishu;

	for (j = 1; j <= weishu_buff; j++, i++)
	{
		num = (long)(dat * TFT_Pow(10,j)) % 10;
		TFT_showchar(x + (8 * i), y * 16, '0'+num, PenColor, BackColor);
	}
	return x + (8 * i);
}

int TFT_showUfloat(uint16 x, uint16 y, float dat, int ZhengshuWeishu, int XiaoshuWeishu, int PenColor, int BackColor)
{
	int flag_StartShow = 0;
	int flag_fushu = 0;
	int weishu_buff = ZhengshuWeishu - 1;
	int num = 0;
	int i = 0;
	int j = 0;
	while (1)
	{
		num = (int)(dat) / (TFT_Pow(10, weishu_buff)) % 10;
		if (flag_StartShow == 0 && num != 0)
		{
			flag_StartShow = 1;
		}
		if (flag_StartShow == 1)
		{
			TFT_showchar(x + (8 * i), y * 16, '0' + num, PenColor, BackColor);
			i++;
		}
		else
		{
		}

		weishu_buff--;
		if (weishu_buff < 0)
		{
			TFT_showchar(x + (8 * i), y * 16, '.', PenColor, BackColor);
			i++;
			break;
		}
	}
	weishu_buff = XiaoshuWeishu;

	for (j = 1; j <= weishu_buff; j++, i++)
	{
		num = (long)(dat * TFT_Pow(10, j)) % 10;
		TFT_showchar(x + (8 * i), y * 16, '0' + num, PenColor, BackColor);
	}

	return x + (8 * i);
}
void TFT_showint8(uint16 x, uint16 y, int8 dat, int PenColor, int BackColor)
{
	uint8 a[3];
	uint8 i;
	if (dat<0)
	{
		TFT_showchar(x, y * 16, '-', PenColor, BackColor);
		dat = -dat;
	}
	else	TFT_showchar(x, y * 16, ' ', PenColor, BackColor);

	a[0] = dat / 100;
	a[1] = dat / 10 % 10;
	a[2] = dat % 10;
	i = 0;
	while (i<3)
	{
		TFT_showchar(x + (8 * (i+1)), y * 16, '0' + a[i], PenColor, BackColor);
		i++;
	}

}

void TFT_showuint8(uint16 x, uint16 y, uint8 dat, int PenColor, int BackColor)
{
	uint8 a[3];
	uint8 i;

	a[0] = dat / 100;
	a[1] = dat / 10 % 10;
	a[2] = dat % 10;
	i = 0;
	while (i<3)
	{
		TFT_showchar(x + (8 * (i)), y * 16, '0' + a[i], PenColor, BackColor);
		i++;
	}

}

void TFT_showint16(uint16 x, uint16 y, int16 dat, int PenColor, int BackColor)
{
	uint8 a[5];
	uint8 i;
	if (dat<0)
	{
		TFT_showchar(x, y * 16, '-', PenColor, BackColor);
		dat = -dat;
	}
	else	TFT_showchar(x, y * 16, ' ', PenColor, BackColor);

	a[0] = dat / 10000;
	a[1] = dat / 1000 % 10;
	a[2] = dat / 100 % 10;
	a[3] = dat / 10 % 10;
	a[4] = dat % 10;

	i = 0;
	while (i<5)
	{
		TFT_showchar(x + (8 * (i+1)), y * 16, '0' + a[i], PenColor, BackColor);
		i++;
	}
}

void TFT_showuint16(uint16 x, uint16 y, uint16 dat, int PenColor, int BackColor)
{
	uint8 a[5];
	uint8 i;

	a[0] = dat / 10000;
	a[1] = dat / 1000 % 10;
	a[2] = dat / 100 % 10;
	a[3] = dat / 10 % 10;
	a[4] = dat % 10;

	i = 0;
	while (i<5)
	{
		TFT_showchar(x + (8 * (i)), y * 16, '0' + a[i], PenColor, BackColor);
		i++;
	}
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示40*40 QQ图片
//  @param      *p     			图像数组地址
//  @return     void
//  @since      v1.0
//  Sample usage:          
//-------------------------------------------------------------------------------------------------------------------
void TFT_showimage(const unsigned char *p)
{
	int i, j, k;
	unsigned char picH, picL;
	dsp_single_colour(WHITE); //清屏  

	for (k = 0; k<4; k++)
	{
		for (j = 0; j<3; j++)
		{
			Lcd_SetRegion(40 * j, 40 * k, 40 * j + 39, 40 * k + 39);		//坐标设置
			for (i = 0; i<40 * 40; i++)
			{
				picL = *(p + i * 2);	//数据低位在前
				picH = *(p + i * 2 + 1);
				LCD_WriteData_16Bit(picH << 8 | picL);
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示Size_x*Size_y 图片
//  @param      *p     			图像数组地址
//  @return     void
//  @since      v1.0
//  Sample usage:          
//-------------------------------------------------------------------------------------------------------------------
void TFT_showimage_all(const unsigned char *p, int Size_x, int Size_y)
{
	uint8 i, j;
	unsigned char picH, picL;
	unsigned int color;

	//dsp_single_colour(WHITE); //清屏  

	Lcd_SetRegion(0, 0, Size_x - 1, Size_y - 1);
	for (i = 0; i < Size_y; i++)
	{
		for (j = 0; j < Size_x; j++)
		{
			picL = *(p + (i*Size_x + j) * 2);	//数据低位在前
			picH = *(p + (i*Size_x + j) * 2 + 1);
			color = picH << 8 | picL;
			LCD_WriteData_16Bit(color);
		}
	}
}
void TFT_DrawPoint(uint16 x, uint16 y, int PointColor)
{

}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      显示Size_x*Size_y 灰度
//  @param      *p     			图像数组地址
//  @return     void
//  @since      v1.0
//  Sample usage:          
//-------------------------------------------------------------------------------------------------------------------
void TFT_showimage_gray(const unsigned char p[40][200], int SizeX_Image, int SizeY_Image, int SizeX_Show, int SizeY_Show)
{
	uint8 i, j;
	unsigned int temp;
	unsigned int color;
	
	Lcd_SetRegion(0 + (128 - SizeX_Show) / 2, 160 - SizeY_Show, 127-(128 - SizeX_Show) / 2, 159);
	for (i = 0; i < SizeY_Show; i++)
	{
		for (j = 0; j < SizeX_Show; j++)
		{
			temp = p[(uint8)(i * SizeX_Image / SizeY_Show)][(uint8)(j*SizeY_Image / SizeX_Show)];
			color = (0x001f & ((temp) >> 3)) << 11;
			color = color | (((0x003f)&((temp) >> 2)) << 5);
			color = color | (0x001f & ((temp) >> 3));

			LCD_WriteData_16Bit(color);
		}
	}
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风(灰度摄像头)液晶显示函数
//  @param      *p     			图像数组地址
//  @return     void
//  @since      v1.0
//  Sample usage:              
//-------------------------------------------------------------------------------------------------------------------
void displayimage032(uint8 *p)
{
    int i, j;

    uint16 color = 0;
    uint16 temp = 0;

    for (i = 0; i<120; i++)
    {
        Lcd_SetRegion(i, 0, i, 160 - 1);		//坐标设置
        for (j = 0; j<160; j++)
        {
            temp = *(p + i*120 / 120 * 188 + (188 - 1) - j*(188 - 1) / (160 - 1));//读取像素点

            color = (0x001f & ((temp) >> 3)) << 11;
            color = color | (((0x003f)&((temp) >> 2)) << 5);
            color = color | (0x001f & ((temp) >> 3));
            LCD_WriteData_16Bit(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
//字体部分
//-------------------------------------------------------------------------------------------------------------------
const uint8 tft_ascii[95][16] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*" ",0*/

	{ 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x08, 0x08, 0x00, 0x00 },/*"!",1*/

	{ 0x00, 0x48, 0x24, 0x24, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*""",2*/

	{ 0x00, 0x00, 0x00, 0x48, 0x48, 0x48, 0x7E, 0x24, 0x24, 0x24, 0x7E, 0x24, 0x24, 0x24, 0x00, 0x00 },/*"#",3*/

	{ 0x00, 0x00, 0x10, 0x3C, 0x52, 0x52, 0x12, 0x1C, 0x30, 0x50, 0x50, 0x52, 0x52, 0x3C, 0x10, 0x10 },/*"$",4*/

	{ 0x00, 0x00, 0x00, 0x22, 0x25, 0x15, 0x15, 0x0D, 0x2A, 0x58, 0x54, 0x54, 0x52, 0x22, 0x00, 0x00 },/*"%",5*/

	{ 0x00, 0x00, 0x00, 0x0C, 0x12, 0x12, 0x12, 0x0A, 0x76, 0x25, 0x29, 0x19, 0x91, 0x6E, 0x00, 0x00 },/*"&",6*/

	{ 0x00, 0x06, 0x04, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*"'",7*/

	{ 0x00, 0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x00 },/*"(",8*/

	{ 0x00, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x00 },/*")",9*/

	{ 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x6B, 0x1C, 0x1C, 0x6B, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00 },/*"*",10*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x10, 0xFE, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00 },/*"+",11*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x04, 0x04, 0x02 },/*",",12*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*"-",13*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00 },/*".",14*/

	{ 0x00, 0x00, 0x40, 0x20, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x00 },/*"/",15*/

	{ 0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00 },/*"0",16*/

	{ 0x00, 0x00, 0x00, 0x10, 0x1C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7C, 0x00, 0x00 },/*"1",17*/

	{ 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x40, 0x20, 0x10, 0x08, 0x04, 0x42, 0x7E, 0x00, 0x00 },/*"2",18*/

	{ 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x40, 0x20, 0x18, 0x20, 0x40, 0x42, 0x42, 0x3C, 0x00, 0x00 },/*"3",19*/

	{ 0x00, 0x00, 0x00, 0x20, 0x30, 0x30, 0x28, 0x24, 0x24, 0x22, 0xFE, 0x20, 0x20, 0xF8, 0x00, 0x00 },/*"4",20*/

	{ 0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x02, 0x1E, 0x22, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00 },/*"5",21*/

	{ 0x00, 0x00, 0x00, 0x18, 0x24, 0x02, 0x02, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x44, 0x38, 0x00, 0x00 },/*"6",22*/

	{ 0x00, 0x00, 0x00, 0x7E, 0x42, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00 },/*"7",23*/

	{ 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00 },/*"8",24*/

	{ 0x00, 0x00, 0x00, 0x1C, 0x22, 0x42, 0x42, 0x42, 0x62, 0x5C, 0x40, 0x40, 0x24, 0x18, 0x00, 0x00 },/*"9",25*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00 },/*":",26*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08 },/*";",27*/

	{ 0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00 },/*"<",28*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*"=",29*/

	{ 0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00 },/*">",30*/

	{ 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x46, 0x20, 0x10, 0x10, 0x10, 0x00, 0x18, 0x18, 0x00, 0x00 },/*"?",31*/

	{ 0x00, 0x00, 0x00, 0x1C, 0x22, 0x5A, 0x55, 0x55, 0x55, 0x55, 0x55, 0x3A, 0x42, 0x3C, 0x00, 0x00 },/*"@",32*/

	{ 0x00, 0x00, 0x00, 0x08, 0x08, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x42, 0x42, 0xE7, 0x00, 0x00 },/*"A",33*/

	{ 0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x1E, 0x22, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00 },/*"B",34*/

	{ 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x01, 0x01, 0x01, 0x01, 0x01, 0x42, 0x22, 0x1C, 0x00, 0x00 },/*"C",35*/

	{ 0x00, 0x00, 0x00, 0x1F, 0x22, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00 },/*"D",36*/

	{ 0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x42, 0x42, 0x3F, 0x00, 0x00 },/*"E",37*/

	{ 0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00 },/*"F",38*/

	{ 0x00, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x01, 0x01, 0x01, 0x71, 0x21, 0x22, 0x22, 0x1C, 0x00, 0x00 },/*"G",39*/

	{ 0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00 },/*"H",40*/

	{ 0x00, 0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00 },/*"I",41*/

	{ 0x00, 0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x0F },/*"J",42*/

	{ 0x00, 0x00, 0x00, 0x77, 0x22, 0x12, 0x0A, 0x0E, 0x0A, 0x12, 0x12, 0x22, 0x22, 0x77, 0x00, 0x00 },/*"K",43*/

	{ 0x00, 0x00, 0x00, 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x42, 0x7F, 0x00, 0x00 },/*"L",44*/

	{ 0x00, 0x00, 0x00, 0x77, 0x36, 0x36, 0x36, 0x36, 0x36, 0x2A, 0x2A, 0x2A, 0x2A, 0x6B, 0x00, 0x00 },/*"M",45*/

	{ 0x00, 0x00, 0x00, 0xE3, 0x46, 0x46, 0x4A, 0x4A, 0x52, 0x52, 0x52, 0x62, 0x62, 0x47, 0x00, 0x00 },/*"N",46*/

	{ 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00 },/*"O",47*/

	{ 0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00 },/*"P",48*/

	{ 0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x4D, 0x32, 0x1C, 0x60, 0x00 },/*"Q",49*/

	{ 0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x3E, 0x12, 0x12, 0x22, 0x22, 0x42, 0xC7, 0x00, 0x00 },/*"R",50*/

	{ 0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x02, 0x04, 0x18, 0x20, 0x40, 0x42, 0x42, 0x3E, 0x00, 0x00 },/*"S",51*/

	{ 0x00, 0x00, 0x00, 0x7F, 0x49, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00 },/*"T",52*/

	{ 0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00 },/*"U",53*/

	{ 0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x22, 0x24, 0x24, 0x14, 0x14, 0x18, 0x08, 0x08, 0x00, 0x00 },/*"V",54*/

	{ 0x00, 0x00, 0x00, 0x6B, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x36, 0x14, 0x14, 0x14, 0x14, 0x00, 0x00 },/*"W",55*/

	{ 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x24, 0x24, 0x42, 0xE7, 0x00, 0x00 },/*"X",56*/

	{ 0x00, 0x00, 0x00, 0x77, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00 },/*"Y",57*/

	{ 0x00, 0x00, 0x00, 0x7E, 0x21, 0x20, 0x10, 0x10, 0x08, 0x04, 0x04, 0x42, 0x42, 0x3F, 0x00, 0x00 },/*"Z",58*/

	{ 0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00 },/*"[",59*/

	{ 0x00, 0x00, 0x02, 0x04, 0x04, 0x04, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x40, 0x40 },/*"\",60*/

	{ 0x00, 0x1E, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x00 },/*"]",61*/

	{ 0x00, 0x18, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*"^",62*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*"-",63*/

	{ 0x00, 0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*"`",64*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x30, 0x2C, 0x22, 0x32, 0x6C, 0x00, 0x00 },/*"a",65*/

	{ 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x26, 0x1A, 0x00, 0x00 },/*"b",66*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x02, 0x02, 0x02, 0x44, 0x38, 0x00, 0x00 },/*"c",67*/

	{ 0x00, 0x00, 0x00, 0x00, 0x60, 0x40, 0x40, 0x7C, 0x42, 0x42, 0x42, 0x42, 0x62, 0xDC, 0x00, 0x00 },/*"d",68*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x7E, 0x02, 0x42, 0x3C, 0x00, 0x00 },/*"e",69*/

	{ 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00 },/*"f",70*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x22, 0x22, 0x1C, 0x02, 0x3C, 0x42, 0x42, 0x3C },/*"g",71*/

	{ 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00 },/*"h",72*/

	{ 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00 },/*"i",73*/

	{ 0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1E },/*"j",74*/

	{ 0x00, 0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x72, 0x12, 0x0A, 0x0E, 0x12, 0x22, 0x77, 0x00, 0x00 },/*"k",75*/

	{ 0x00, 0x00, 0x00, 0x08, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00 },/*"l",76*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x92, 0x92, 0x92, 0x92, 0x92, 0xB7, 0x00, 0x00 },/*"m",77*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00 },/*"n",78*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00 },/*"o",79*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x26, 0x42, 0x42, 0x42, 0x26, 0x1A, 0x02, 0x07 },/*"p",80*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 0x64, 0x42, 0x42, 0x42, 0x64, 0x58, 0x40, 0xE0 },/*"q",81*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x4C, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x00, 0x00 },/*"r",82*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x02, 0x3C, 0x40, 0x42, 0x3E, 0x00, 0x00 },/*"s",83*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x48, 0x30, 0x00, 0x00 },/*"t",84*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x42, 0x42, 0x42, 0x42, 0x62, 0xDC, 0x00, 0x00 },/*"u",85*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x00, 0x00 },/*"v",86*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDB, 0x91, 0x52, 0x5A, 0x2A, 0x24, 0x24, 0x00, 0x00 },/*"w",87*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6E, 0x24, 0x18, 0x18, 0x18, 0x24, 0x76, 0x00, 0x00 },/*"x",88*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x08, 0x08, 0x06 },/*"y",89*/

	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x22, 0x10, 0x08, 0x08, 0x44, 0x7E, 0x00, 0x00 },/*"z",90*/

	{ 0x00, 0xC0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0xC0, 0x00 },/*"{",91*/

	{ 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },/*"|",92*/

	{ 0x00, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x08, 0x04, 0x04, 0x04, 0x04, 0x04, 0x03, 0x00 },/*"}",93*/

	{ 0x04, 0x5A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },/*"~",94*/
};

const uint8  asc2_1608[1520] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x48, 0x6C, 0x24, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x7F, 0x12, 0x12, 0x12, 0x7F, 0x12, 0x12, 0x12, 0x00, 0x00,
	0x00, 0x00, 0x08, 0x1C, 0x2A, 0x2A, 0x0A, 0x0C, 0x18, 0x28, 0x28, 0x2A, 0x2A, 0x1C, 0x08, 0x08,
	0x00, 0x00, 0x00, 0x22, 0x25, 0x15, 0x15, 0x15, 0x2A, 0x58, 0x54, 0x54, 0x54, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0C, 0x12, 0x12, 0x12, 0x0A, 0x76, 0x25, 0x29, 0x11, 0x91, 0x6E, 0x00, 0x00,
	0x00, 0x06, 0x06, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x40, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x40, 0x00,
	0x00, 0x02, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x6B, 0x1C, 0x1C, 0x6B, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x7F, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x04, 0x03,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x04, 0x02, 0x02, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x20, 0x20, 0x10, 0x08, 0x04, 0x42, 0x7E, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x20, 0x18, 0x20, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x20, 0x30, 0x28, 0x24, 0x24, 0x22, 0x22, 0x7E, 0x20, 0x20, 0x78, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x02, 0x1A, 0x26, 0x40, 0x40, 0x42, 0x22, 0x1C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x38, 0x24, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7E, 0x22, 0x22, 0x10, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x64, 0x58, 0x40, 0x40, 0x24, 0x1C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x04,
	0x00, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x46, 0x40, 0x20, 0x10, 0x10, 0x00, 0x18, 0x18, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1C, 0x22, 0x5A, 0x55, 0x55, 0x55, 0x55, 0x2D, 0x42, 0x22, 0x1C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x08, 0x08, 0x18, 0x14, 0x14, 0x24, 0x3C, 0x22, 0x42, 0x42, 0xE7, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1F, 0x22, 0x22, 0x22, 0x1E, 0x22, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x01, 0x01, 0x01, 0x01, 0x01, 0x42, 0x22, 0x1C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1F, 0x22, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x22, 0x1F, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x42, 0x42, 0x3F, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3F, 0x42, 0x12, 0x12, 0x1E, 0x12, 0x12, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x01, 0x01, 0x01, 0x71, 0x21, 0x22, 0x22, 0x1C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x11, 0x0F,
	0x00, 0x00, 0x00, 0x77, 0x22, 0x12, 0x0A, 0x0E, 0x0A, 0x12, 0x12, 0x22, 0x22, 0x77, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x07, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x42, 0x7F, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x77, 0x36, 0x36, 0x36, 0x36, 0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x6B, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xE3, 0x46, 0x46, 0x4A, 0x4A, 0x52, 0x52, 0x52, 0x62, 0x62, 0x47, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x22, 0x1C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x02, 0x02, 0x07, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x1C, 0x22, 0x41, 0x41, 0x41, 0x41, 0x41, 0x4D, 0x53, 0x32, 0x1C, 0x60, 0x00,
	0x00, 0x00, 0x00, 0x3F, 0x42, 0x42, 0x42, 0x3E, 0x12, 0x12, 0x22, 0x22, 0x42, 0xC7, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7C, 0x42, 0x42, 0x02, 0x04, 0x18, 0x20, 0x40, 0x42, 0x42, 0x3E, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7F, 0x49, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xE7, 0x42, 0x42, 0x22, 0x24, 0x24, 0x14, 0x14, 0x18, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x6B, 0x49, 0x49, 0x49, 0x49, 0x55, 0x55, 0x36, 0x22, 0x22, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x18, 0x18, 0x18, 0x24, 0x24, 0x42, 0xE7, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x77, 0x22, 0x22, 0x14, 0x14, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x7E, 0x21, 0x20, 0x10, 0x10, 0x08, 0x04, 0x04, 0x42, 0x42, 0x3F, 0x00, 0x00,
	0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00,
	0x00, 0x00, 0x02, 0x02, 0x04, 0x04, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x20, 0x40, 0x40,
	0x00, 0x1E, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1E, 0x00,
	0x00, 0x38, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
	0x00, 0x06, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x78, 0x44, 0x42, 0x42, 0xFC, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x1A, 0x26, 0x42, 0x42, 0x42, 0x26, 0x1A, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x02, 0x02, 0x02, 0x44, 0x38, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x60, 0x40, 0x40, 0x40, 0x78, 0x44, 0x42, 0x42, 0x42, 0x64, 0xD8, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x7E, 0x02, 0x02, 0x42, 0x3C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xF0, 0x88, 0x08, 0x08, 0x7E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x22, 0x22, 0x1C, 0x02, 0x3C, 0x42, 0x42, 0x3C,
	0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x3A, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22, 0x1E,
	0x00, 0x00, 0x00, 0x03, 0x02, 0x02, 0x02, 0x72, 0x12, 0x0A, 0x16, 0x12, 0x22, 0x77, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x0E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x3E, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x92, 0x92, 0x92, 0x92, 0x92, 0xB7, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3B, 0x46, 0x42, 0x42, 0x42, 0x42, 0xE7, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x26, 0x42, 0x42, 0x42, 0x22, 0x1E, 0x02, 0x07,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x44, 0x42, 0x42, 0x42, 0x44, 0x78, 0x40, 0xE0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x4C, 0x04, 0x04, 0x04, 0x04, 0x1F, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x42, 0x02, 0x3C, 0x40, 0x42, 0x3E, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x30, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x63, 0x42, 0x42, 0x42, 0x42, 0x62, 0xDC, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x14, 0x08, 0x08, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEB, 0x49, 0x49, 0x55, 0x55, 0x22, 0x22, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0x24, 0x18, 0x18, 0x18, 0x24, 0x6E, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE7, 0x42, 0x24, 0x24, 0x14, 0x18, 0x08, 0x08, 0x07,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x22, 0x10, 0x08, 0x08, 0x44, 0x7E, 0x00, 0x00,
	0x00, 0xC0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xC0, 0x00,
	0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
	0x00, 0x06, 0x08, 0x08, 0x08, 0x08, 0x08, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x06, 0x00,
	0x0C, 0x32, 0xC2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
