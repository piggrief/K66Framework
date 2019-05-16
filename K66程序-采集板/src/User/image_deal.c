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

uint8 top_row;
uint8 bottom_row;
uint8 low_row;
int row;
uint8 First_Find[ROW];
uint8 Left[ROW];
float Mid[ROW];
uint8 Right[ROW];
uint8 Left_Flag[ROW];            //左边界确认标志，找到后边界
uint8 Right_Flag[ROW];           //右边界确认标志，找到后边界
uint8 Mid_Flag[ROW];
uint8 found_flag;
uint8 light_delay_flag;
uint8 find_loop_flag;
uint8 light_flag;
float Mid_temp;
float Right_temp;
float temp_piancha;
float average_piancha;
//int length[5] = {0};
uint8 arrl_flag = 0;
//int true_row;
int distance;
float temp_distance = 0;
//int arrl[5];
//float r = -1.8744; // 距离系数

/***************************快速中值化滤波**********************************/
/*void MedianFilter()
{
	int Pic[ROW][COL];
	int a, b, c, d;
	int i,j;
	for (j = 0; j < ROW; j++)
	{
		Pic[0][j] = image[0][j];
		Pic[Height - 1][j] = image[Height - 1][j];
	}
	for (i = 1; i < Height - 1; i++)
	{
		Pic[i][0] = image[i][0];
		Pic[i][Wide - 1] = image[i][Wide - 1];
	}
	for (i = 1; i < Height - 1; i++)
	{
		j = 1;
		a = qic_Sort(image[i - 1][j - 1], image[i - 1][j], image[i - 1][j + 1]);
		b = qic_Sort(image[i][j - 1], image[i][j], image[i][j + 1]);
		c = qic_Sort(image[i + 1][j - 1], image[i + 1][j], image[i + 1][j + 1]);
		d = qic_Sort(a, b, c);
		Pic[i][1] = d;
		for (j = 2; j < Wide - 1; j++)
		{
			if (image[i - 1][j + 1] == image[i - 1][j - 2] && image[i][j + 1] == image[i][j - 2] && image[i + 1][j + 1] == image[i + 1][j - 2])
			{
				Pic[i][j] = d;
			}
			else
			{
				a = qic_Sort(image[i - 1][j - 1], image[i - 1][j], image[i - 1][j + 1]);
				b = qic_Sort(image[i][j - 1], image[i][j], image[i][j + 1]);
				c = qic_Sort(image[i + 1][j - 1], image[i + 1][j], image[i + 1][j + 1]);
				d = qic_Sort(a, b, c);
				Pic[i][j] = d;
			}
		}
	}
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			image[i][j] = Pic[i][j];
		}
	}
}
/*************************快速三元素以任意顺序排序***************************************/
/*int qic_Sort(int a, int b, int c)
{
	if ((b >= a && b <= c) || (b <= a && b >= c))
	{
		return b;
	}
	else if ((a >= b && a >= c) || (a <= b && a <= c))
	{
		return c;
	}
	else if ((c >= a && c >= b) || (c <= a && c <= b))
	{
		return a;
	}
}
/********************************
             初始化
********************************/
void Reset()
{
    found_flag = 0;
	find_loop_flag = 0;
//	average_piancha = 0;
    for (int i = 0; i < ROW; i++)
    {
	  Left_Flag[i] = 0;
	  Mid_Flag[i] = 0;
	  Right_Flag[i] = 0;
     }
}

/**************************************
                  找灯
***************************************/
int flag_all = 1;
void find_light()
{
	for (int i = 15; i < ROW;i++)
		{
			for (int j = 1; j < COL; j+=2)//j++
			{
				if (image[i][j] > ThreasHold)
				{
					if (image[i + 1][j] > ThreasHold)
					{
						found_flag = 1;
						light_flag = 1;
						light_delay_flag = 0;
						top_row = i;
						find_loop_flag = 0;
						First_Find[i] = j;
                        flag_all = 1;
						return ;
					}
				}
			}
		}
	if (found_flag == 0)
	{
		find_loop_flag = 1;
		light_delay_flag += 1;
	}
}
/**************************************
			  中值找灯
**************************************/
void mid_findlight()
{
	int i, j, k;
	if (found_flag == 1)
	{
		for (i = top_row; i < ROW; i++)
		{
			for (j = 1; j < COL; j++)
			{
				if (image[i][j] < ThreasHold)
				{

				}
				else
				{
					Left[i] = j;
					Left_Flag[i] = 1;
					break;
				}
			}
			if (Left_Flag[i] == 0)//补线
			{
				Left[i] = 0;
				Left_Flag[i] = 1;
			}
			for (k = First_Find[i - 1]; k > 0; k--)
			{
				if (image[i][k] > ThreasHold)
				{

				}
				else
				{
					Right[i] = k;
					Right_Flag[i] = 1;
					break;
				}
			}
			if (Right_Flag[i] == 0)
			{
				Right[i] = COL;
				Right_Flag[i] = 1;
			}
			if (Left_Flag[i] == 1 && Right_Flag[i] == 1)
			{
				Mid_Flag[i] = 1;
				Mid[i] = (Left[i] + Right[i]) / 2;				
                                bottom_row = i;
			}
			if (image[i + 1][First_Find[i]] < ThreasHold && image[i + 1][First_Find[i + 2]] < ThreasHold)
            {
                //bottom_row = i-1;
                break;
            }
		}
/****************************************
					   计算偏差
*****************************************/
		    
        	for (int i = top_row; i <= bottom_row-1; i++)
	        {   
				for (int j = top_row;j <= bottom_row - i - 1;j++)
				{
					if (Mid[j] > Mid[j + 1])
					{
						Mid_temp = Mid[j + 1];
						Mid[j + 1] = Mid[j];
						Mid[j] = Mid_temp;
					}
				}
 	         }
			average_piancha = Mid[top_row+1];//- 94;
		//	temp_piancha = average_piancha;
		    if (average_piancha == 0)//-94)
			{
				average_piancha = temp_piancha;
			}
			else
                temp_piancha = average_piancha;        
	}

	if (find_loop_flag == 1)
	{
          average_piancha = temp_piancha;
		  temp_piancha = average_piancha;       
          return;
    }
}
/***************************************
               计算距离
****************************************/
void calculate_distance()
{
	int i, j, k = 0;
	if (found_flag == 1)
	{
		for (i = top_row ; i < ROW; i++)
		{
			for (j = 1; j < COL; j++)
			{
				if (image[i][j] >= ThreasHold)
				{
					k++;
					break;
				}
			}
		}
		distance = k;
		//distance = r * row + 94.5167;//回归直线计算得出
    }
}
/*void calculate_distance()
{
	int i, temp_true_row;
	calculate_true_row();
	for (i = 1; i <= 5; i++)
	{
		true_row[i] = row;
	}
	for (i = 1; i <= 4; i++)
	{
		if (true_row[i] > true_row[i + 1])
		{
			temp_true_row = true_row[i + 1];
			true_row[i + 1] = true_row[i];
			true_row[i] = temp_true_row;
		}
	}
	distance = r * true_row[5] + 94.5167;//回归直线计算得出
}*/
/*float Fiter_float(float num, float*queue, int*weighted, int num_q)
{
	int i = 0;
	for (i = num_q - 1; i > 0; i--)
	{
		queue[i] = queue[i - 1];
	}
	queue[0] = num;
	return Weighted_Avg_float(queue, weighted, num_q);
}*/

/****************************************
			   重复找灯
*****************************************/
void find_loop()
{
	if (find_loop_flag == 1)
	{
		find_light;
	}
	if (light_delay_flag > 2  || distance > 30 && light_delay_flag > 1)
	{
		light_flag = 0;//灯灭标志
		find_loop_flag = 0;
		light_delay_flag = 0;
        average_piancha = 0;
		temp_piancha = 0;
		row = 0;
        flag_all = 0;
		/*for (int i = 0; i <= 4; i++)
		{
			length[i] = 0;
		}*/
	}
}

/**/
/*void zhan()
{
	if (arrl_flag != 5)
	{
		length[arrl_flag] = true_row;
		arrl_flag++;
		return;
	}
	else
	{
		int k = 0;
		for( k = 0;k < 4;k++)
		{
			length[k] = length[k + 1];
		}
		length[k] = true_row;
	}

}
void dis()
{
	for (int i = 0; i <= 4; i++)
	{
		arrl[i] = length[i];
	}
	for (int i = 0; i < 4; i++)
	{
                if (arrl[i] > arrl[i + 1])
                {
                        temp_distance = arrl[i + 1];
                        arrl[i + 1] = arrl[i];
                        arrl[i] = temp_distance;
                }
	}
                 distance = arrl[4];
}*/

typedef enum
{
    Light_Disappear,//灯不见了
    Light_On,//灯亮
    Light_Off//灯灭
}LookLight_Status;

LookLight_Status LightStatus = Light_Disappear;

# define BoardWidthThreshold 4
# define JumpThreshold 35
# define StartLookLine 10
# define EndLookLine 90//不包含这个值

int LeftLine[EndLookLine - StartLookLine];
int RightLine[EndLookLine - StartLookLine];
float MidLine[EndLookLine - StartLookLine];

int LastMidLine = 0;
uint8 flag_LookedLeftLine = 0;
uint8 flag_LookedRightLine = 0;

///寻线程序，主要用于寻找图像中的左右边线
void LookLine(uint8 imagebuff[120][188])
{
    flag_LookedLeftLine = 0;
    flag_LookedRightLine = 0;

    for (int i = StartLookLine, LineIndex = 0; i < EndLookLine; i++, LineIndex++)
    {
        for (int j = 0; j < Wide - BoardWidthThreshold; j++)
        {
            if ((imagebuff[i][j + BoardWidthThreshold] - imagebuff[i][j]) > JumpThreshold)
            {
                LeftLine[LineIndex] = j;
                flag_LookedLeftLine = 1;
                break;
            }
        }
        if (flag_LookedLeftLine == 1)
        {
            for (int j = LeftLine[i]; j < Wide - BoardWidthThreshold; j++)
            {
                if (imagebuff[i][j] - (imagebuff[i][j + BoardWidthThreshold]) > JumpThreshold)
                {
                    RightLine[LineIndex] = j;
                    flag_LookedRightLine = 1;
                    break;
                }
            }
        }
        else//全黑行
        {
            LeftLine[i] = -1;
            RightLine[i] = -1;
        }
    }
}

# define LightDisappearCountThreshold 4
uint8 Count_LightOff = 0;
///更新灯亮状态
void RenewLightStatus(int MidLineNum)
{
    if (LightStatus == Light_Disappear)
    {
        if (MidLineNum > 2)
        {
            LightStatus = Light_On;
            Count_LightOff = 0;
        }
    }
    else if (LightStatus == Light_On)
    {
        if (MidLineNum <= 0)
        {
            LightStatus = Light_Off;
        }
    }
    else
    {
        if (MidLineNum > 0)
        {
            LightStatus = Light_On;
            Count_LightOff = 0;
        }
        else
        {
            Count_LightOff++;
            if (Count_LightOff >= LightDisappearCountThreshold)
            {
                LightStatus = Light_Disappear;
                Count_LightOff = 0;
            }
        }
    }
}

int Num_MidLine = 0;
int LastNum_MidLine = 0;
float Temp_Error = 0;
float Now_Error = 0;
int Mid_MidLine = 0;
float Sum_WhiteBlock = 0;
float LastSum_WhiteBlock = 0;

float Size_SumMid = 0;
///计算偏差
void CalError()
{
    Temp_Error = Now_Error;
    LastSum_WhiteBlock = Sum_WhiteBlock;
    LastNum_MidLine = Num_MidLine;
    float MidSum = 0;
    int MidLineIndex = 0;
    Sum_WhiteBlock = 0;
    
    for (int i = 0; i < EndLookLine - StartLookLine; i++)
    {
        if (LeftLine[i] >= 0 && RightLine[i] >= 0)
        {
            MidLine[MidLineIndex] = 0.5 * (LeftLine[i] + RightLine[i]);
            //MidSum += MidLine[MidLineIndex];
            MidLineIndex++;
            Sum_WhiteBlock += (RightLine[i] - LeftLine[i]);
        }
    }
    Num_MidLine = MidLineIndex;
        
    RenewLightStatus(Num_MidLine);

    if (LightStatus == Light_Disappear)
    {
        Now_Error = 0;
    }
    else if (LightStatus == Light_On)
    {
        Mid_MidLine = Num_MidLine / 3.0;
        if(Mid_MidLine > Num_MidLine)
          Mid_MidLine = Num_MidLine;
        if(Mid_MidLine < 0)
          Mid_MidLine = 0;
        Now_Error = MidLine[Mid_MidLine]; // MidSum / MidLineIndex;
        Size_SumMid = Sum_WhiteBlock* 10.0 / Num_MidLine / 3.5;
    }
    else
    {
        Now_Error = Temp_Error;
        Num_MidLine = LastNum_MidLine;
        Sum_WhiteBlock = LastSum_WhiteBlock;
    }
}