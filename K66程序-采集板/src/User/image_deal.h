/*!
* @file       image_deal.h
* @brief      用于调试用的函数库头文件
* @details
* @author     lv丶fox
* @version    v1.0
* @date       2019-2-26
* @to do
*
*/
# ifndef _IMAGE_DEAL_H_
# define _IMAGE_DEAL_H_


#include "SEEKFREE_MT9V032.h"

# define JumpThreshold 30 // 35
# define ThreasHold_Fail 1150 // 440
# define MaxJumpThreshold_1 42 // 30
# define MaxJumpThreshold_2  42 // 30

#define Deal_White  1
#define Deal_Black  0
#define ThreasHold 40//40  100

# define StartLookLine 9
# define EndLookLine 75//不包含这个值

typedef enum
{
    Light_Disappear,//灯不见了
    Light_On,//灯亮
    Light_Off//灯灭
}LookLight_Status;

typedef struct
{
    int Num_MidLine;
    int Num_EffectiveMidLine;
    int TopLineIndex;
    int BottomLineIndex;
    int LastNum_MidLine;
    float Temp_Error;
    float Now_Error;
    int Mid_MidLine;
    float Sum_WhiteBlock;
    float Sum_LineSize;
    float AverageLineSize;
    float LastSum_WhiteBlock;
    float Size_SumMid;
    float MidSum;
    
    LookLight_Status LightStatus;

    int LeftLine[EndLookLine - StartLookLine];
    int RightLine[EndLookLine - StartLookLine];
    int ChaLine[EndLookLine - StartLookLine];
    float MidLine[EndLookLine - StartLookLine];
    
    uint8 Count_LightOff;
}ImageDeal_ForLight;

extern ImageDeal_ForLight ImageDeal_Camera1;
extern ImageDeal_ForLight ImageDeal_Camera2;

void find_light();
void mid_findlight();
void calculate_distance();
void find_loop();
void Reset();
//void zhan();
//void dis();

void LookLine(ImageDeal_ForLight * ImageDeal,uint8 imagebuff[120][188]);
void RenewLightStatus(ImageDeal_ForLight * ImageDeal, int MidLineNum);
void CalError(ImageDeal_ForLight * ImageDeal);

extern float Now_Error;
extern int Num_MidLine;
extern float Size_SumMid;

typedef struct
{
  uint8 MaxGray;
  uint8 MinGray;
  uint8 MeanGray; 
}RegionImageInfo;

void CalRegionGrayMinMax(uint8 imagebuff[120][188], uint8 startx, uint8 starty, uint8 height, uint8 width);

void SunDeal();
void SunDeal_2();
extern uint8 SunDeal_3(int index);
extern uint8 imageTemp_1[ROW][COL];
extern uint8 imageCha_1[ROW][COL];
extern uint8 imageTemp_2[ROW][COL];
extern uint8 imageCha_2[ROW][COL];
extern int MaxRow_1;
extern int MaxCol_1;
extern int MaxRow_2;
extern int MaxCol_2;
extern int MaxGray_1;
extern int MaxGray_2;
extern unsigned long LightNum_ChaImage;
#endif