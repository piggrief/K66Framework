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
extern float everage_piancha;
extern uint8 Left_Flag[ROW];
extern uint8 found_flag;
extern int time;
extern float temp_piancha;
extern uint8 light_flag;
extern uint8 light_delay_flag;
extern uint8 find_loop_flag;

float Data_Test[21] = { 0 };
float Weight_FIR1[21] = { 0, -0.0007, -0.0015, 0.0006, 0.0092, 0.0269, 0.0534, 0.0852, 0.1158, 0.1380, 0.1461
, 0.1380, 0.1158, 0.0852, 0.0534, 0.0269, 0.0092, 0.0006, -0.0015, -0.0007, 0 };
int32 ttt = 0;

extern int time;
extern uint32 SpeedCount[4];

void Init_All();

void main(void)
{
    Init_All();
    PID_init(160, 0, 1, 9000, 9500, 0);//80, 0, 0.1, 100000, 9500, 0

	while (1)
	{
        if (ImageDealState_Now == Image_CollectFinish)
        {
            ImageDealState_Now = Image_Dealing;
            LED_Ctrl(LED3, ON);      //LED指示程序运行状态  
            
	        //MedianFilter();
	        displayimage032(image[0], ThreasHold);
            //OLED_ShowImage(128, 64, 120, 188, ThreasHold, image);
	        Reset();
	        find_light();
	        mid_findlight();
	        find_loop();
                                          
            //seekfree_sendimg_032();                         
            ImageDealState_Now = Image_DealingFinsh;
        }
        else
            LED_Ctrl(LED3, OFF);      //LED指示程序运行状态  
        LCD_PrintFloat_31(0,0,everage_piancha);


	}
}

void Init_All()
{
    DisableInterrupts;           //关闭中断
    PLL_Init(PLL200);            //初始化PLL为200M,总线为40M  

    /*******************GPIO***************************/
    LED_Init();                  //LED初始化    
    TFT_init(SPI_1, SPIn_PCS0);
    LCD_Init();
    ButtonMenu();
    UART_Init(UART_3, 115200);
    
    EncoderMeasure_Init();
    RemoteInit();

    FTM_PWM_Init(FTMType_Use1, FTMChannel_Use_W1_1, 100000, 0);
    FTM_PWM_Init(FTMType_Use1, FTMChannel_Use_W1_2, 100000, 0);
    FTM_PWM_Init(FTMType_Use2, FTMChannel_Use_W2_1, 100000, 0);
    FTM_PWM_Init(FTMType_Use2, FTMChannel_Use_W2_2, 100000, 0);
    FTM_PWM_Init(FTMType_Use3, FTMChannel_Use_W3_1, 100000, 0);
    FTM_PWM_Init(FTMType_Use3, FTMChannel_Use_W3_2, 100000, 0);
    FTM_PWM_Init(FTMType_Use4, FTMChannel_Use_W4_1, 100000, 0);
    FTM_PWM_Init(FTMType_Use4, FTMChannel_Use_W4_2, 100000, 0);

    PIT_Init(PIT0, 8);
    EnableInterrupts;

    TFT_showstr(0, 0, "Initing!", BLACK, WHITE);
    camera_init();
    TFT_showstr(0, 0, "Success!", BLACK, WHITE);

    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            image[i][j] = 0xff;
        }
    }


}

extern uint32 Speed_get[4];
void PIT0_Interrupt()
{
    SpeedClean();
    int i = 0;
    //count_1ms++;
    //if (count_1ms >= 500)
    //{
    //	count_1ms = 0;
    //	Count++;
    //	TFT_showint16(0, 0, Count, BLACK, WHITE);
    //	TFT_showstr(0, 20, "S", BLACK, WHITE);
    //}
    for (i = 0; i < 4; i++)
    {
        GetSpeed(i);
        Speed_get[i] = SpeedCount[i];
    }
    SEND(Speed_get[0], Speed_get[1], Speed_get[2], Speed_get[3]);
        SetSpeed_FromRemote(RunMode);

    PIT_Flag_Clear(PIT0);       //???D??㊣那????


}
uint32 TimeCount_test = 0;
void PIT1_Interrupt()
{
	PIT_Flag_Clear(PIT1);       //清中断标志位
   /*用户添加所需代码*/
	TimeCount_test++;
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