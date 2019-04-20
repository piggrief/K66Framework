#ifndef _ATTITUDECALCULATION_H_
#define _ATTITUDECALCULATION_H_

//说明：my_atan(x,y)   ->    arctan(y/x)的近似值
//补充：y<0显示负角，x<0结果+180度，没有负角，如果x<0且<0则结果为-arctan(y/x)+180

float my_atan(float x, float y);
float GetAngle_FromAcc(float Ax, float Ay, float Az);

#endif