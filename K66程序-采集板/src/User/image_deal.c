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

# define BoardWidthThreshold 0



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

float AverageGray_All = 0;
float GraySum_All = 0;
uint8 imageDealBuff_1[ROW][COL];
uint8 imageDealBuff_2[ROW][COL];
void SunDeal()
{
  GraySum_All = 0;
  AverageGray_All = 0;
  for(int i = 0; i < ROW; i++)
  {
    for(int j = 0; j < COL; j++)
    {
      imageDealBuff_1[i][j] = image_2[i][j];
    }
  }
  for(int i = 15; i < 70; i++)
  {
    for(int j = 0; j < COL; j++)
    {
      GraySum_All+=image_2[i][j];
      //image_2[i][j] -= 50;
    }
  }
  AverageGray_All = GraySum_All / ROW / COL;
//  for(int i = 0; i < ROW; i++)
//  {
//    for(int j = 0; j < COL; j++)
//    {
//      if(i < 70)
//        image_2[i][j] -= AverageGray_All;
//      else
//        image_2[i][j] = 0;
//    }
//  }

    //3窗口灰度级腐蚀
  uint8 MinGray = 255;
  for(int i = 0; i < ROW; i++)
  {
    for(int j = 0; j < COL; j++)
    {
      if(i < 70)
      {
        if (i - 1 >= 0 && j - 1 >= 0 && i + 1 <= ROW && j+1<=COL)
        {
          MinGray = 255;
          for (int ii = i - 1; ii <= i + 1; ii++)
          {
            for(int jj = j - 1; jj <= j + 1; jj++)
            {
              if(image_2[ii][jj] < MinGray)
              {
                MinGray = image_2[ii][jj];
              }
            }
          }
          imageDealBuff_1[i][j] = MinGray;
          
        }
      }
      else
      {
        imageDealBuff_1[i][j] = 0;
      }
    }
  }
    //3窗口灰度级膨胀
  uint8 MaxGray = 0;
  for(int i = 0; i < ROW; i++)
  {
    for(int j = 0; j < COL; j++)
    {
      if(i < 70)
      {
        if (i - 1 >= 0 && j - 1 >= 0 && i + 1 <= ROW && j+1<=COL)
        {
          MaxGray = 0;
          for (int ii = i - 1; ii <= i + 1; ii++)
          {
            for(int jj = j - 1; jj <= j + 1; jj++)
            {
              if(imageDealBuff_1[ii][jj] > MaxGray)
              {
                MaxGray = imageDealBuff_1[ii][jj];
              }
            }
          }
          imageDealBuff_2[i][j] = MaxGray;
          
        }
      }
      else
      {
        imageDealBuff_2[i][j] = 0;
      }
    }
  } 

  int buff = 0;
  for(int i = 0; i < ROW; i++)
  {
    for(int j = 0; j < COL; j++)
    {
//      buff = imageDealBuff_2[i][j] - image_2[i][j];
//      if(buff < 0)
//      {
//        image_2[i][j] = -buff;
//      }
//      else
//      {
//        image_2[i][j] = 0;
//      }
      image_2[i][j] = imageDealBuff_2[i][j];
    }
  }
}

void SunDeal_2()
{
  GraySum_All = 0;
  AverageGray_All = 0;
  for(int i = 0; i < ROW; i++)
  {
    for(int j = 0; j < COL; j++)
    {
      imageDealBuff_1[i][j] = image_2[i][j];
    }
  }
  for(int i = 15; i < 70; i++)
  {
    for(int j = 0; j < COL; j++)
    {
      GraySum_All+=image_2[i][j];
      //image_2[i][j] -= 50;
    }
  }
  AverageGray_All = GraySum_All / ROW / COL;
//  for(int i = 0; i < ROW; i++)
//  {
//    for(int j = 0; j < COL; j++)
//    {
//      if(i < 70)
//        image_2[i][j] -= AverageGray_All;
//      else
//        image_2[i][j] = 0;
//    }
//  }
    //3窗口灰度级膨胀
  uint8 MaxGray = 0;
  for(int i = 0; i < ROW; i++)
  {
    for(int j = 0; j < COL; j++)
    {
      if(i < 70)
      {
        if (i - 1 >= 0 && j - 1 >= 0 && i + 1 <= ROW && j+1<=COL)
        {
          MaxGray = 0;
          for (int ii = i - 1; ii <= i + 1; ii++)
          {
            for(int jj = j - 1; jj <= j + 1; jj++)
            {
              if(image_2[ii][jj] > MaxGray)
              {
                MaxGray = image_2[ii][jj];
              }
            }
          }
          imageDealBuff_1[i][j] = MaxGray;
          
        }
      }
      else
      {
        imageDealBuff_1[i][j] = 0;
      }
    }
  } 
    //3窗口灰度级腐蚀
  uint8 MinGray = 255;
  for(int i = 0; i < ROW; i++)
  {
    for(int j = 0; j < COL; j++)
    {
      if(i < 70)
      {
        if (i - 1 >= 0 && j - 1 >= 0 && i + 1 <= ROW && j+1<=COL)
        {
          MinGray = 255;
          for (int ii = i - 1; ii <= i + 1; ii++)
          {
            for(int jj = j - 1; jj <= j + 1; jj++)
            {
              if(imageDealBuff_1[ii][jj] < MinGray)
              {
                MinGray = imageDealBuff_1[ii][jj];
              }
            }
          }
          imageDealBuff_2[i][j] = MinGray;
          
        }
      }
      else
      {
        imageDealBuff_2[i][j] = 0;
      }
    }
  }


  int buff = 0;
  for(int i = 0; i < ROW; i++)
  {
    for(int j = 0; j < COL; j++)
    {
      buff = imageDealBuff_2[i][j] - image_2[i][j];
      if(buff > 0)
      {
        image_2[i][j] = buff;
      }
      else
      {
        image_2[i][j] = 0;
      }
      //image_2[i][j] = imageDealBuff_2[i][j];
    }
  }
}
uint8 imageTemp_1[ROW][COL];
uint8 imageCha_1[ROW][COL];
uint8 imageTemp_2[ROW][COL];
uint8 imageCha_2[ROW][COL];
unsigned long LightNum_ChaImage = 0;
uint8 CheckImageEffective(uint8 ImageNow[ROW][COL], uint8 ImageTemp[ROW][COL], uint8 ImageCha[ROW][COL])
{
    int ChaBuff = 0;
    LightNum_ChaImage = 0;
    for (int i = StartLookLine; i < EndLookLine; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            ChaBuff = ImageNow[i][j] - ImageTemp[i][j];
            if(ChaBuff < 0)
              ChaBuff = -ChaBuff;
            ImageCha[i][j] = (uint8)(ChaBuff);
            ImageTemp[i][j] = ImageNow[i][j];
            if (ImageCha[i][j] > JumpThreshold)
            {
                LightNum_ChaImage++;
            }
        }
    }
    if (LightNum_ChaImage >= ThreasHold_Fail)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int MaxGray_1 = 0;
int MaxRow_1 = 0;
int MaxCol_1 = 0;
int MaxGray_2 = 0;
int MaxRow_2 = 0;
int MaxCol_2 = 0;
uint8 SunDeal_3(int index)
{
  int MaxRowBuff = 0, MaxColBuff = 0;
    if (index == 1)
    {
        if (CheckImageEffective(image_1, imageTemp_1, imageCha_1) == 1)
        {
            //LookLine(&ImageDeal_Camera1, imageCha_1);
            //CalError(&ImageDeal_Camera1);
            MaxGray_1 = 0;
            
            for (int i = StartLookLine; i < EndLookLine; i++)
            {
                for (int j = 0; j < COL; j++)
                {
                    if (imageCha_1[i][j] > MaxGray_1)
                    {
                        MaxGray_1 = imageCha_1[i][j];
                        MaxRowBuff = i;
                        MaxColBuff = j;
                    }
                }
            }
            if (MaxGray_1 >= MaxJumpThreshold_1)
            {
                MaxRow_1 = MaxRowBuff;
                MaxCol_1 = MaxColBuff;
                return 1;
            }
            return 0;
        }
        else
            return 0;
    }
    else if (index == 2)
    {
        if (CheckImageEffective(image_2, imageTemp_2, imageCha_2) == 1)
        {
            MaxGray_2 = 0;

            for (int i = StartLookLine; i < EndLookLine; i++)
            {
                for (int j = 0; j < COL; j++)
                {
                    if (imageCha_2[i][j] > MaxGray_2)
                    {
                        MaxGray_2 = imageCha_2[i][j];
                        MaxRowBuff = i;
                        MaxColBuff = j;
                    }
                }
            }
            if (MaxGray_2 >= MaxJumpThreshold_2)
            {
                MaxRow_2 = MaxRowBuff;
                MaxCol_2 = MaxColBuff;
                return 1;
            }
            return 0;
        }
        else
            return 0;
    }
    
}