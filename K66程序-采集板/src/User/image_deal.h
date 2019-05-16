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

#define Deal_White  1
#define Deal_Black  0
#define ThreasHold 80//40  100


void find_light();
void mid_findlight();
void calculate_distance();
void find_loop();
void Reset();
//void zhan();
//void dis();

void LookLine(uint8 imagebuff[120][188]);
void RenewLightStatus(int MidLineNum);
void CalError();

extern float Now_Error;
extern int Num_MidLine;
extern float Size_SumMid;

#endif