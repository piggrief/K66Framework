#ifndef _ATTITUDECALCULATION_H_
#define _ATTITUDECALCULATION_H_

//说明：my_atan(x,y)   ->    arctan(y/x)的近似值
//补充：y<0显示负角，x<0结果+180度，没有负角，如果x<0且<0则结果为-arctan(y/x)+180

float my_atan(float x, float y);
float GetAngle_FromAcc(float Ax, float Ay, float Az);
/// <summary>
///递推式FIR滤波器
///<para>example:  FIRFilter_ToFloat(Num1, Num, Weight, 5);</para>
///</summary>
/// <param name="Data">本次采集的数据</param>
/// <param name="DataArr">数据缓存数组</param>
/// <param name="weight">FIR滤波器系数(分子多项式系数Num)</param>
/// <param name="size">FIR滤波器阶数+1(数据缓存数组大小)</param>
float FIRFilter_ToFloat(float Data, float * DataArr, float * weight, int size);

#endif