#include "include.h"
#define TSL1401_SIPin  PTB, 5
#define TSL1401_CLKPin PTB, 2
#define TSL1401_SI(x)   GPIO_Ctrl(TSL1401_SIPin, x)
#define TSL1401_CLK(x)  GPIO_Ctrl(TSL1401_CLKPin, x)
#define TSL1401_ADC ADC0_SE13

#define PRINT_AD        1       //设置串口打印数据类型，0:打印二值化数据，1：打印AD值
#define THRESHOLD       100     //设置二值化阈值
#define WINDOW_WIDTH    128     //设置串口打印的像素个数，最大128，最小0

void TSL1401_Init()
{
    GPIO_Init(TSL1401_SIPin, GPO, 0);
    GPIO_Init(TSL1401_CLKPin, GPO, 0);
    ADC_Init(TSL1401_ADC);
}

/*
* 延时几微妙
*/
void TSL1401delay()
{
    unsigned int i;
    for (i = 0; i<10; i++)
    {
        asm("nop");
    }
}

void TSL1401_GetLine(uint8 *pixel)
{
    uint8 i;

    //开始SI
    TSL1401_SI(0);
    TSL1401_CLK(0);
    TSL1401delay();
    TSL1401_SI(1);
    TSL1401delay();
    TSL1401_CLK(1);
    TSL1401delay();
    TSL1401_SI(0);
    TSL1401delay();
    //采集第1个点
    pixel[0] = ADC_Once(ADC_0, TSL1401_ADC, ADC_8bit);
    TSL1401_CLK(0);

    //采集第2~128个点
    for (i = 1; i<128; i++)
    {
        TSL1401delay();
        TSL1401_CLK(1);
        TSL1401delay();
        pixel[i] = ADC_Once(ADC_0, TSL1401_ADC, ADC_8bit);
        TSL1401_CLK(0);
    }

    //发送第129个clk
    TSL1401delay();
    TSL1401_CLK(1);
    TSL1401delay();
    TSL1401_CLK(0);
    TSL1401delay();

}
void TSL1401_SendLine(uint8 *LineImage)
{
    for (int i = (64 - WINDOW_WIDTH / 2); i<(64 + WINDOW_WIDTH / 2); i++)
    {
        if (LineImage[i] == 0xFF)
            LineImage[i] = 0xFE; //遇到FF用FE替换即可

        UART_Put_Char(TSL1401_DebugSerials, LineImage[i]);
    }
    UART_Put_Char(TSL1401_DebugSerials, 0xFF);  //此字节用于线性CCD调试助手识别换行
}

void LCD_ShowOneLineImage(uint8 * image)
{
    uint8 pixbuff = 0;
    for (int i = 0; i < 128; i++)
    {
        pixbuff = image[i] * 63 / 255;
        LCD_PutPixel(i, 63-pixbuff);
    }
}

/*线性CCD图像处理部分*/
# define Binary_GateValue 150
# define RoadBlockCheckGateValue 25
int Black_Count = 0;
int White_Count = 0;
int CheckRoadBlock(uint8 * image)
{
    Black_Count = 0;
    White_Count = 0;
    for (int i = 0; i < 128; i++)
    {
        if (image[i] >= Binary_GateValue)
        {
            White_Count++;
        }
        else
        {
            Black_Count++;
        }
    }
    LCD_PrintInt3(0, 5, White_Count);
    LCD_PrintInt3(0, 6, Black_Count);
    if (White_Count <= 25)
    {
        GPIO_Ctrl(BeepIOPortType, BeepIOPortIndex, 0);
    }
    else
    {
        GPIO_Ctrl(BeepIOPortType, BeepIOPortIndex, 1);
    }
    if (White_Count < RoadBlockCheckGateValue)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*线性CCD图像处理部分_赛道*/
# define ChangeGateValue 35
uint8 flag_leftline = 0;
uint8 flag_rightline = 0;
uint8 leftline = 0;
uint8 rightline = 0;

void lookline(uint8 * image)
{
    flag_rightline = 0;
    flag_leftline = 0;
    /*右扫*/
    for (int i = 64; i < 128 - 3; i++)
    {
        if ((image[i] - image[i + 3]) > ChangeGateValue)
        {
            flag_rightline = 1;
            rightline = i + 1;
            break;
        }
    }
    for (int i = 64; i >= 0+3; i--)
    {
        if ((image[i] - image[i - 3]) > ChangeGateValue)
        {
            flag_leftline = 1;
            leftline = i + 1;
            break;
        }
    }
}
float midline = 0;
float lastmidline = 0;
float CalPiancha()
{
    float errorbuff = 0;
    lastmidline = midline;
    if (flag_leftline && flag_rightline)
    {
        midline = 0.5*(leftline + rightline);
        errorbuff = midline - 64;
    }
    else if (flag_leftline)
    {
        midline = leftline + 20;
        errorbuff = 1.5*(midline - 64);
    }
    else if (flag_rightline)
    {
        midline = rightline - 20;
        errorbuff = 1.5*(midline - 64);
    }
    else//全丢线
    {
        midline = lastmidline;
        errorbuff = 1.5*(midline - 64);
    }

    return errorbuff;
}