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
uint8 row1;
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
float true_row;
float temp_radius;
float distance;
float temp_distance = 0;
float r = -1.8744; // 距离系数

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
	for (int i = 15; i < ROW;i+=2)
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
		for (i = top_row + 1; i < ROW; i++)
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
			for (k = First_Find[i - 1]; k < COL; k++)
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
                            break;
		}
/****************************************
					   计算偏差
*****************************************/
		   
        	for (int i = top_row; i <= bottom_row-1; i++)
	        {   
		    	if (Mid[i] > Mid[i + 1])
		    	{
			    	Mid_temp = Mid[i + 1];
			    	Mid[i + 1] = Mid[i];
			    	Mid[i] = Mid_temp;
		        }
 	         }
			average_piancha = Mid[bottom_row];//- 94;
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
		true_row = k;
		distance = r * true_row + 94.5167;//回归直线计算得出
		/*if (abs(distance - temp_distance) > 10)
		{
			distance = temp_distance;
		}
		else
		{
			temp_distance = distance;
		}*
    }
}
/*		temp_radius = radius;
		if (radius == 0)
		{
			radius = temp_radius;
		}
		else
			temp_radius = radius;
	}
	if (find_loop_flag == 1)
      {
		radius = temp_radius;
	    temp_radius = radius;
		return;
      }
}
*/
	//	radius1 = bottom_row - top_row;
		//distance = r * radius;
	//	if (average_piancha == 0)//-94)
		//{
	//		average_piancha = temp_piancha;
	//	}
	//	else
	//		temp_piancha = average_piancha;
//	}

	//if (find_loop_flag == 1)
	//{
	//	average_piancha = temp_piancha;
	//	temp_piancha = average_piancha;
		//distance = temp_distance;
		//temp_distance = distance;


	//	return;
	//   }
        }
}

/****************************************
			   重复找灯
*****************************************/
void find_loop()
{
	if (find_loop_flag == 1)
	{
		find_light;
	}
	if (light_delay_flag > 3)
	{
		light_flag = 0;//灯灭标志
		find_loop_flag = 0;
		light_delay_flag = 0;
        average_piancha = 0;
		temp_piancha = 0;
		true_row = 0;
                flag_all = 0;
	}
}