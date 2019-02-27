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

//主函数
int count_1ms = 0;
extern int16 GET_CFG[9 - 1][2];
extern uint8 image[120][188];

void main(void) 
{   
    int i = 0, j = 0;
   DisableInterrupts;           //关闭中断
   PLL_Init(PLL200);            //初始化PLL为200M,总线为40M  
//     GPIO_Init(PTC,8,GPI,1);
//     GPIO_Init(PTC,9,GPI,1);
//     GPIO_Init(PTC,10,GPI,1);
//     GPIO_Init(PTC,11,GPI,1);
//     GPIO_Init(PTC,12,GPI,1);
//     GPIO_Init(PTC,13,GPI,1);
//     GPIO_Init(PTC,14,GPI,1);
//     GPIO_Init(PTC,15,GPI,1);
//     
   //PIT_Init(PIT0, 1);
   /*******************GPIO***************************/
   LED_Init();                  //LED初始化    
   TFT_init(SPI_1, SPIn_PCS0);
   //MT9V032_Init();

   //FTM_PWM_Init(FTM0, FTM_CH0, 65535, 0);
   //FTM_PWM_Init(FTM0, FTM_CH1, 65535, 0);
   //FTM_PWM_Init(FTM0, FTM_CH2, 65535, 0);
   //FTM_PWM_Init(FTM0, FTM_CH3, 65535, 0);

   //EnableInterrupts;
   //dsp_single_colour(RED);
   ////UART_Put_Str(UART_3, "33333333\n");

   //LPTMR_delay_ms(5000);


   //FTM_PWM_Duty(FTM0, FTM_CH0, 0);
   //FTM_PWM_Duty(FTM0, FTM_CH1, 58000);//58000
   //FTM_PWM_Duty(FTM0, FTM_CH2, 0);
   //FTM_PWM_Duty(FTM0, FTM_CH3, 58000);

   //LPTMR_delay_ms(2000);
   EnableInterrupts;
   TFT_showstr(0, 0, "Test!", BLACK, WHITE);
   camera_init();

   TFT_showstr(1, 0, "Success!", BLACK, WHITE);

   for (i = 0; i < 120; i++)
   {
       for (j = 0; j < 188; j++)
       {
           image[i][j] = 0xff;
       }
   }
   while (1)
   {
       if (ImageDealState_Now == Image_CollectFinish)
       {
           ImageDealState_Now = Image_Dealing;
           LED_Ctrl(LED3, ON);      //LED指示程序运行状态  

           displayimage032(image[0]);

           ImageDealState_Now = Image_DealingFinsh;
       }
       else
           LED_Ctrl(LED3, OFF);      //LED指示程序运行状态  

	   //FTM_PWM_Duty(FTM0, FTM_CH0, 0);
	   //FTM_PWM_Duty(FTM0, FTM_CH1, 0);
	   //FTM_PWM_Duty(FTM0, FTM_CH2, 0);
	   //FTM_PWM_Duty(FTM0, FTM_CH3, 0);

	  //dsp_single_colour(RED);

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