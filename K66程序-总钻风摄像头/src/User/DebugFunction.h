/*!
* @file       DebugFuction.h
* @brief      用于调试用的函数库头文件
* @details
* @author     pig's grief
* @version    v1.0
* @date       2019-2-26
* @to do
*
*/
# ifndef _DEBUGFUNCTION_H_
# define _DEBUGFUNCTION_H_

# define CRC_Uart_Port UART_3
/// <summary>
///给虚拟示波器发送a,b,c,d取整之后的值
///<para>example:  SEND(a,b,c,d);</para>
///</summary>
/// <param name="a">待发送的变量1</param>
/// <param name="b">待发送的变量2</param>
/// <param name="c">待发送的变量3</param>
/// <param name="d">待发送的变量4</param>
void SEND(float a,float b,float c,float d);
/// <summary>
///在OLED上绘制曲线图(相当于在OLED上使用虚拟示波器)
///程序没运行一次时刻点会加1直至超过t_refresh之后会重新从0时刻开始
///<para>example:  LCD_ShowGraphs(data, 126, 0, 100, 0);</para>
///</summary>
/// <param name="data">本次时刻对应的数据值(y轴的值)</param>
/// <param name="t_refresh">刷新时刻点的阈值，最大不超过126</param>
/// <param name="data_low">要显示的数据的范围的下限</param>
/// <param name="data_high">要显示的数据的范围的上限</param>
/// <param name="WhetherClear">刷新时刻点之后是否清屏，1表示清屏，0表示否</param>
void LCD_ShowGraphs(float data, float t_refresh, float data_low, float data_high, int WhetherClear);
/*===================================================================
功能：串口发送数据用于Matlab数据处理(整数)
===================================================================*/
//void DATA_SEND(long num);
#endif