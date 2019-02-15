/****************************************************************************************************
【平    台】北京龙邱智能科技MK66FX1M0VLQ18核心板
【编    写】CHIUSIR
【E-mail  】chiusir@aliyun.com
【软件版本】V1.0
【最后更新】2016年08月24日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://shop36265907.taobao.com
------------------------------------------------
【dev.env.】IAR7.3
【Target  】K66FX1M0VLQ18
【Crystal 】 50.000Mhz
【busclock】 40.000MHz
【pllclock】200.000MHz
=============================================================
LPTMR延时功能演示：1S钟板载LED(PTE26)闪烁
=============================================================
修改历史：
******************************************************************************************************/

#include "include.h" 

uint8   uart_receive_flag = 1;
uint8   receive[3];
uint8   receive_num = 0;
#define MT9V032_COF_UART    UART_3  //配置摄像头所使用到的串口     

void MT9V032_Config_interrupt()
{
	receive[receive_num] = UART_Get_Char(MT9V032_COF_UART);;
	receive_num++;

	if (1 == receive_num && 0XA5 != receive[0])  receive_num = 0;
	if (3 == receive_num)
	{
		receive_num = 0;
		uart_receive_flag = 1;
	}

}
#define COL     188//图像宽度   范围1-752     K60采集不允许超过188
#define ROW     120//图像高度	范围1-480

//摄像头命令枚举
typedef enum
{
	INIT = 0,               //摄像头初始化命令
	AUTO_EXP,               //自动曝光命令
	EXP_TIME,               //曝光时间命令
	FPS,                    //摄像头帧率命令
	SET_COL,                //图像列命令
	SET_ROW,                //图像行命令
	LR_OFFSET,              //图像左右偏移命令
	UD_OFFSET,              //图像上下偏移命令
	GAIN,                   //图像偏移命令
	CONFIG_FINISH,          //非命令位，主要用来占位计数

	SET_EXP_TIME = 0XF0,    //单独设置曝光时间命令
	GET_STATUS,             //获取摄像头配置命令
	GET_VERSION,            //固件版本号命令

	SET_ADDR = 0XFE,        //寄存器地址命令
	SET_DATA                //寄存器数据命令
}CMD;

//需要配置到摄像头的数据
int16 MT9V032_CFG[CONFIG_FINISH][2] =
{
	{ AUTO_EXP, 0 },   //自动曝光设置      范围1-63 0为关闭 如果自动曝光开启  EXP_TIME命令设置的数据将会变为最大曝光时间，也就是自动曝光时间的上限
	//一般情况是不需要开启这个功能，因为比赛场地光线一般都比较均匀，如果遇到光线非常不均匀的情况可以尝试设置该值，增加图像稳定性
	{ EXP_TIME, 300 }, //曝光时间          摄像头收到后会自动计算出最大曝光时间，如果设置过大则设置为计算出来的最大曝光值
	{ FPS, 50 },  //图像帧率          摄像头收到后会自动计算出最大FPS，如果过大则设置为计算出来的最大FPS
	{ SET_COL, COL }, //图像列数量        范围1-752     K60采集不允许超过188
	{ SET_ROW, ROW }, //图像行数量        范围1-480
	{ LR_OFFSET, 0 },   //图像左右偏移量    正值 右偏移   负值 左偏移  列为188 376 752时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
	{ UD_OFFSET, 0 },   //图像上下偏移量    正值 上偏移   负值 下偏移  行为120 240 480时无法设置偏移    摄像头收偏移数据后会自动计算最大偏移，如果超出则设置计算出来的最大偏移
	{ GAIN, 32 },  //图像增益          范围16-64     增益可以在曝光时间固定的情况下改变图像亮暗程度


	{ INIT, 0 }    //摄像头开始初始化
};

//从摄像头内部获取到的配置数据
int16 GET_CFG[CONFIG_FINISH - 1][2] =
{
	{ AUTO_EXP, 0 },   //自动曝光设置      
	{ EXP_TIME, 0 },   //曝光时间          
	{ FPS, 0 },   //图像帧率          
	{ SET_COL, 0 },   //图像列数量        
	{ SET_ROW, 0 },   //图像行数量        
	{ LR_OFFSET, 0 },   //图像左右偏移量    
	{ UD_OFFSET, 0 },   //图像上下偏移量    
	{ GAIN, 0 },   //图像增益          
};
uint16 MT9V032_temp;
uint8  send_buffer[4];

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取摄像头内部配置信息
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				调用该函数前请先初始化uart3
//-------------------------------------------------------------------------------------------------------------------
void get_config(void)
{
	uint16 temp, i;
	uint8  send_buffer[4];

	for (i = 0; i<CONFIG_FINISH - 1; i++)
	{
		send_buffer[0] = 0xA5;
		send_buffer[1] = GET_STATUS;
		temp = GET_CFG[i][0];
		send_buffer[2] = temp >> 8;
		send_buffer[3] = (uint8)temp;

		UART_Put_Str(MT9V032_COF_UART, send_buffer);

		//等待接受回传数据
		while (!uart_receive_flag);
		uart_receive_flag = 0;

		GET_CFG[i][1] = receive[1] << 8 | receive[2];

	}
}
void MT9V032_Init()
{
	int i = 0;
	UART_Init(UART_3, 9600);
	UART_Irq_En(UART_3);

	LPTMR_delay_ms(1000);
	uart_receive_flag = 0;

	////开始配置摄像头并重新初始化
	//for (i = 0; i<CONFIG_FINISH; i++)
	//{
	//	send_buffer[0] = 0xA5;
	//	send_buffer[1] = MT9V032_CFG[i][0];
	//	MT9V032_temp = MT9V032_CFG[i][1];
	//	send_buffer[2] = MT9V032_temp >> 8;
	//	send_buffer[3] = (uint8)MT9V032_temp;
	//	UART_Put_Str(MT9V032_COF_UART, send_buffer);
	//	LPTMR_delay_ms(2);
	//}
	////等待摄像头初始化成功
	//while (!uart_receive_flag);
	//uart_receive_flag = 0;
	//while ((0xff != receive[1]) || (0xff != receive[2]));
	////以上部分对摄像头配置的数据全部都会保存在摄像头上51单片机的eeprom中
	////利用set_exposure_time函数单独配置的曝光数据不存储在eeprom中
	get_config();
}

//主函数
int count_1ms = 0;
void main(void) 
{   
   DisableInterrupts;           //关闭中断
   PLL_Init(PLL180);            //初始化PLL为200M,总线为40M  
   //PIT_Init(PIT0, 1);
   /*******************GPIO***************************/
   //LED_Init();                  //LED初始化    
   TFT_init(SPI_1, SPIn_PCS0);
   //MT9V032_Init();

   FTM_PWM_Init(FTM0, FTM_CH0, 65535, 0);
   FTM_PWM_Init(FTM0, FTM_CH1, 65535, 0);
   FTM_PWM_Init(FTM0, FTM_CH2, 65535, 0);
   FTM_PWM_Init(FTM0, FTM_CH3, 65535, 0);

   EnableInterrupts;
   dsp_single_colour(RED);
   //UART_Put_Str(UART_3, "33333333\n");

   LPTMR_delay_ms(5000);


   FTM_PWM_Duty(FTM0, FTM_CH0, 0);
   FTM_PWM_Duty(FTM0, FTM_CH1, 58000);//58000
   FTM_PWM_Duty(FTM0, FTM_CH2, 0);
   FTM_PWM_Duty(FTM0, FTM_CH3, 58000);

   LPTMR_delay_ms(2000);

   while(1)
   {
	   FTM_PWM_Duty(FTM0, FTM_CH0, 0);
	   FTM_PWM_Duty(FTM0, FTM_CH1, 0);
	   FTM_PWM_Duty(FTM0, FTM_CH2, 0);
	   FTM_PWM_Duty(FTM0, FTM_CH3, 0);

	  //dsp_single_colour(RED);

      //LED_Ctrl(LED3, RVS);      //LED指示程序运行状态  
   }
}
int Count = 0;
void PIT0_Interrupt()
{
	count_1ms++;
	if (count_1ms >= 500)
	{
		count_1ms = 0;
		Count++;
		TFT_showint16(0, 0, Count, BLACK, WHITE);
		TFT_showstr(0, 20, "S", BLACK, WHITE);
	}

  PIT_Flag_Clear(PIT0);       //清中断标志位


}

void PIT1_Interrupt()
{
  PIT_Flag_Clear(PIT1);       //清中断标志位
 /*用户添加所需代码*/
}

void PIT2_Interrupt()
{
  PIT_Flag_Clear(PIT2);       //清中断标志位
  /*用户添加所需代码*/
}

void PIT3_Interrupt()
{
  PIT_Flag_Clear(PIT3);       //清中断标志位
  /*用户添加所需代码*/
}