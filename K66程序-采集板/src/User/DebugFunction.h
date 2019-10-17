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

# define CRC_Uart_Port UART_4
# define Series_Uart_Port UART_4
# define Remote_Uart_Port UART_0
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
void DATA_SEND(long num);



typedef enum
{
    Left_Return0,
    Left_Left,
    Left_Right,
    Left_Up,
    Left_Down,
    Right_Return0,
    Right_Left,
    Right_Right,
    Right_Up,
    Right_Down,
    Start
}RemoteCMDMode;
/// <summary>
///接受遥控指令程序，应放入对应的串口中断内
///</summary>
void ReceiveCMD_Remote();
/// <summary>
///初始化遥控器的串口
///</summary>
void RemoteInit();
extern RemoteCMDMode RunMode;

typedef enum
{
    Press,
    NotPress
}ButtonStatus;//按键的状态，Press为按下,NotPress为弹起

extern ButtonStatus Button1;//PTE12
extern ButtonStatus Button2;//PTE11
extern ButtonStatus Button3;//PTE10
///<summary>按键初始化</summary>
void ButtonInit();
///<summary>按键扫描中断</summary>
void ButtonScan_interrupt();
/// <summary>
///按键菜单程序，用于参数设定等功能，放于主函数的主要功能前
///<para>注：一定要放在TFT初始化后，另外其他有中断的模块初始化都必须放在这个函数后面</para>
///</summary>
void ButtonMenu();

typedef enum
{
  ShowImage_1,
  ShowImage_2,
  NotShow
}TFTShowMode;
extern TFTShowMode NowTFTShowMode;
void Series_Sendout_init(void);
#endif