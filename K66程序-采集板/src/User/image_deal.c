/*!
* @file       image_deal.c
* @brief      用于调试用的函数库
* @details
* @author     lv丶fox
* @version    v1.0
* @date       2019-2-26
* @to do
*
*/


#include "include.h"
#include "image_deal.h"


#define Height              ROW
#define Wide                COL

ImageDeal_ForLight ImageDeal_Camera1;
ImageDeal_ForLight ImageDeal_Camera2;

void ImageDealStructInit(ImageDeal_ForLight * ImageDeal)
{
    ImageDeal->Num_MidLine = 0;
    ImageDeal->LastNum_MidLine = 0;
    ImageDeal->Num_EffectiveMidLine = 0;
    ImageDeal->Temp_Error = 0;
    ImageDeal->Now_Error = 0;
    ImageDeal->Mid_MidLine = 0;
    ImageDeal->Sum_WhiteBlock = 0;
    ImageDeal->LastSum_WhiteBlock = 0;
    ImageDeal->Size_SumMid = 0;
    
    ImageDeal->LightStatus = Light_Disappear;
    
    for(int i = 0;i < EndLookLine - StartLookLine; i++)
    {
      ImageDeal->LeftLine[i] = -1;
      ImageDeal->RightLine[i] = -1;
      ImageDeal->MidLine[i] = -1;
    }
    
    ImageDeal->Count_LightOff = 0;
}

# define BoardWidthThreshold 2
# define JumpThreshold 35


uint8 flag_LookedLeftLine = 0;
uint8 flag_LookedRightLine = 0;

///寻线程序，主要用于寻找图像中的左右边线
void LookLine(ImageDeal_ForLight * ImageDeal,uint8 imagebuff[120][188])
{
    flag_LookedLeftLine = 0;
    flag_LookedRightLine = 0;

    for (int i = StartLookLine, LineIndex = 0; i < EndLookLine; i++, LineIndex++)
    {
        for (int j = 0; j < Wide - BoardWidthThreshold; j++)
        {
            if ((imagebuff[i][j + BoardWidthThreshold] - imagebuff[i][j]) > JumpThreshold)
            {
                ImageDeal->LeftLine[LineIndex] = j;
                flag_LookedLeftLine = 1;
                break;
            }
        }
        if (flag_LookedLeftLine == 1)
        {
            for (int j = ImageDeal->LeftLine[i]; j < Wide - BoardWidthThreshold; j++)
            {
                if (imagebuff[i][j] - (imagebuff[i][j + BoardWidthThreshold]) > JumpThreshold)
                {
                    ImageDeal->RightLine[LineIndex] = j;
                    ImageDeal->ChaLine[LineIndex] = ImageDeal->RightLine[LineIndex] - ImageDeal->LeftLine[LineIndex];
                    flag_LookedRightLine = 1;
                    break;
                }
            }
        }
        else//全黑行
        {
            ImageDeal->LeftLine[i] = -1;
            ImageDeal->RightLine[i] = -1;
            ImageDeal->ChaLine[i] = -1;
        }
    }
}

# define LightDisappearCountThreshold 10

///更新灯亮状态
void RenewLightStatus(ImageDeal_ForLight * ImageDeal, int MidLineNum)
{
    if (ImageDeal->LightStatus == Light_Disappear)
    {
        if (MidLineNum > 2)
        {
            ImageDeal->LightStatus = Light_On;
            ImageDeal->Count_LightOff = 0;
        }
    }
    else if (ImageDeal->LightStatus == Light_On)
    {
        if (MidLineNum <= 0)
        {
            ImageDeal->LightStatus = Light_Off;
        }
    }
    else
    {
        if (MidLineNum > 0)
        {
            ImageDeal->LightStatus = Light_On;
            ImageDeal->Count_LightOff = 0;
        }
        else
        {
            ImageDeal->Count_LightOff++;
            if (ImageDeal->Count_LightOff >= LightDisappearCountThreshold)
            {
                ImageDeal->LightStatus = Light_Disappear;
                ImageDeal->Count_LightOff = 0;
            }
        }
    }
}

uint8 flag_FindTopLine = 0;
uint8 flag_FindBottomLine = 0;
///计算偏差
void CalError(ImageDeal_ForLight * ImageDeal)
{
    ImageDeal->Temp_Error = ImageDeal->Now_Error;
    ImageDeal->LastSum_WhiteBlock = ImageDeal->Sum_WhiteBlock;
    ImageDeal->LastNum_MidLine = ImageDeal->Num_MidLine;
    float MidSum = 0;
    int MidLineIndex = 0;
    ImageDeal->Sum_WhiteBlock = 0;

    ImageDeal->Sum_LineSize = 0;
    flag_FindTopLine = 0;
    flag_FindBottomLine = 0;
    for (int i = 0; i < EndLookLine - StartLookLine; i++)
    {
        if (ImageDeal->LeftLine[i] >= 0 && ImageDeal->RightLine[i] >= 0)
        {
          if(flag_FindTopLine)
          {
            ImageDeal->Sum_LineSize += ImageDeal->ChaLine[i];
          }
          else
          {
            ImageDeal->TopLineIndex = i;
            flag_FindTopLine = 1;
          }
          MidLineIndex++;
        }
        else
        {
          if(flag_FindTopLine && !flag_FindBottomLine)
          {
            ImageDeal->BottomLineIndex = i; 
            flag_FindBottomLine = 1;
          }
        }
    }
    ImageDeal->Num_MidLine = MidLineIndex;
    ImageDeal->AverageLineSize = ImageDeal->Sum_LineSize / MidLineIndex;

    
    MidLineIndex = 0;
    ImageDeal->MidSum = 0;

    for (int i = ImageDeal->TopLineIndex; i < ImageDeal->BottomLineIndex; i++)
    {
        ImageDeal->MidLine[MidLineIndex] = 0.5 * (ImageDeal->LeftLine[i] + ImageDeal->RightLine[i]);
        ImageDeal->MidSum += ImageDeal->MidLine[MidLineIndex];

        //ImageDeal->Sum_WhiteBlock += (ImageDeal->RightLine[i] - ImageDeal->LeftLine[i]);
        MidLineIndex++;
    }
    ImageDeal->Num_EffectiveMidLine = MidLineIndex;
    
    RenewLightStatus(ImageDeal, ImageDeal->Num_MidLine);

    if (ImageDeal->LightStatus == Light_Disappear)
    {
        ImageDeal->Now_Error = 0;
    }
    else if (ImageDeal->LightStatus == Light_On)
    {
        ImageDeal->Mid_MidLine = 0.5 * ImageDeal->Num_EffectiveMidLine;
        if(ImageDeal->Mid_MidLine > ImageDeal->Num_MidLine)
          ImageDeal->Mid_MidLine = ImageDeal->Num_MidLine;
        if(ImageDeal->Mid_MidLine < 0)
          ImageDeal->Mid_MidLine = 0;
        ImageDeal->Now_Error = ImageDeal->MidLine[ImageDeal->Mid_MidLine]; // MidSum / MidLineIndex;
        //ImageDeal->Size_SumMid = ImageDeal->Sum_WhiteBlock* 10.0 / ImageDeal->Num_MidLine / 3.5;
    }
    else
    {
        ImageDeal->Now_Error = ImageDeal->Temp_Error;
        ImageDeal->Num_MidLine = ImageDeal->LastNum_MidLine;
        ImageDeal->Sum_WhiteBlock = ImageDeal->LastSum_WhiteBlock;
    }
}


RegionImageInfo NowRegionImage;

void CalRegionGrayMinMax(uint8 imagebuff[120][188], uint8 startx, uint8 starty, uint8 height, uint8 width)
{
    float GraySum = 0;
    unsigned int AddIndex = 0;
    NowRegionImage.MaxGray = imagebuff[startx][starty];
    NowRegionImage.MinGray = imagebuff[startx][starty];
    NowRegionImage.MeanGray = imagebuff[startx][starty];

    for (int i = startx; i < startx + width; i++)
    {
        for (int j = starty; j < starty + height; j++,AddIndex++)
        {
            if (imagebuff[i][j] > NowRegionImage.MaxGray)
                NowRegionImage.MaxGray = imagebuff[i][j];
            if (imagebuff[i][j] < NowRegionImage.MinGray)
                NowRegionImage.MinGray = imagebuff[i][j];
            GraySum += imagebuff[i][j];
        }
    }
    NowRegionImage.MeanGray = (uint8)(GraySum / AddIndex);
}
