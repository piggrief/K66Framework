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
extern int16 GET_CFG_1[9 - 1][2];
extern int16 GET_CFG_2[9 - 1][2];
extern float average_piancha;
extern uint8 Left_Flag[ROW];
extern uint8 found_flag;
extern int time;
extern float temp_piancha;
extern uint8 light_flag;
extern uint8 light_delay_flag;
extern uint8 find_loop_flag;
float true_row;
extern float distance;
extern float radius1;
extern uint8 row1;

char Deviation_Sendout = 0;
float Series_deviation_received = 0;

///<summary>速度环参数</summary>
float P_Set[4] = {80, 80, 80, 80};
float D_Set[4] = {20, 20, 20, 20};
float I_Set[4] = {1.4, 1.4, 1.4, 1.4};
float DeadBand_Set[4] = {680, 740, 740, 780};//{680, 740, 740, 780}
float I_limit = 8000;
float Max_output = 9500;

float Data_Test[21] = { 0 };
float Weight_FIR1[21] = { 0, -0.0007, -0.0015, 0.0006, 0.0092, 0.0269, 0.0534, 0.0852, 0.1158, 0.1380, 0.1461
    , 0.1380, 0.1158, 0.0852, 0.0534, 0.0269, 0.0092, 0.0006, -0.0015, -0.0007, 0 };
int32 ttt = 0;

uint8 LastImage[120][188];
uint8 ImageMinus[120][188];

extern int time;
extern uint32 SpeedCount[4];
uint32 TimeMeassure = 0;
void Init_All();

void DataSend()
{
    //SEND(LightNum_ChaImage, MaxGray_1, MaxRow_1, MaxCol_1);
    Deviation_Sendout = (uint8)MaxCol_1;
    UART_Put_Char(UART_4, 0xFF);
    UART_Put_Char(UART_4, Deviation_Sendout);
    UART_Put_Char(UART_4, (uint8)MaxRow_1);
    Deviation_Sendout = (uint8)MaxCol_2;
    UART_Put_Char(UART_4, Deviation_Sendout);
    UART_Put_Char(UART_4, (uint8)MaxRow_2);            
    UART_Put_Char(UART_4, 0xFF); 
}

uint8 testbuff=0;
uint8 ImageSizeToTFTSize(uint8 ImageSize, uint8 IfWidth);
uint8 DisappearCount_1 = 0;
uint8 DisappearCount_2 = 0;
void main(void)
{
    int minusresult = 0;
    Init_All();
    
    //NowTFTShowMode = ShowImage_1;
    
    //PID_Speedloop_init(P_Set, D_Set, I_Set, I_limit, Max_output, DeadBand_Set);//80, 0, 0.1, 100000, 9500, 0
    //PID_locationloop_init(1, 0.5, 0, 0, 60, 0);
    while (1)
    {
        if (ImageDealState_Now == Image1_Dealing)
        {
            pit_time_start(PIT2);
            LED_Ctrl(LED3, RVS);      //LED指示程序运行状态
            if(NowTFTShowMode == ShowImage_1)
            {
              //TFT_showuint8(0,0,1,WHITE,BLACK);
              displayimage032(imageCha_1[0], 1,FindThreshold_OTSUNormal(imageCha_1, 2), ROW, COL);
            }

            
	    //MedianFilter();未定义状态
            //LookLine(&ImageDeal_Camera1, image_1);
            //CalError(&ImageDeal_Camera1);
            
            if (SunDeal_3(1) == 1)//阳光处理成功
            {
              DisappearCount_1 = 0;
              //TFT_showuint8(0, 4, 001, WHITE, BLACK);
            }   
            else
            {
                DisappearCount_1++;
                if(DisappearCount_1 >= 7)
                {
                  MaxCol_1 = 0;
                  MaxRow_1 = 0;
                  DisappearCount_1 = 0;
                }
                if (NowTFTShowMode == ShowImage_1)
                {
                    //TFT_showuint8(0, 4, 000, WHITE, BLACK);
                }
            }       
            
            
            
//            if (NowTFTShowMode == ShowImage_1)
//            {
//                  //TFT_showuint8(0, 0, MaxCol_1, WHITE, BLACK);
//                  //TFT_showuint8(0, 1, MaxRow_1, WHITE, BLACK);
//                  //TFT_showuint8(0, 2, MaxGray_1, WHITE, BLACK);
//            }
//            //seekfree_sendimg_032();   串口发送            
//            //testbuff = image_1[ImageDeal_Camera1.TopLineIndex + 1][ImageDeal_Camera1.LeftLine[ImageDeal_Camera1.TopLineIndex + 1]];
//            DataSend();            
            #ifndef UseTwoCamera
            ImageDealState_Now =  Image_DealingFinish;     
            #endif
            
          ImageDealState_Now = Image2_Wait;
          TimeMeassure = pit_time_get_ms(PIT2);
        }
#ifdef UseTwoCamera
        if(ImageDealState_Now == Image2_Dealing)
        {
            if(NowTFTShowMode == ShowImage_2)
            {
              //SunDeal_2();
              //TFT_showuint8(0,0,2,WHITE,BLACK);
              displayimage032(image_2[0], 0,80, ROW, COL);
            }
            if (SunDeal_3(2) == 1)//阳光处理成功
            {
              DisappearCount_2 = 0;
                if (NowTFTShowMode == ShowImage_2)
                {
                    TFT_showuint8(0, 0, MaxCol_2, WHITE, BLACK);
                    TFT_showuint8(0, 1, MaxRow_2, WHITE, BLACK);
                    TFT_showuint8(0, 2, MaxGray_2, WHITE, BLACK);
                }
            }
            else
            {
                DisappearCount_2++;
                if(DisappearCount_2 >= 7)
                {
                  MaxCol_2 = 0;
                  MaxRow_2 = 0;
                  DisappearCount_2 = 0;
                }
                if (NowTFTShowMode == ShowImage_2)
                {
                    TFT_showuint8(0, 0, 999, WHITE, BLACK);
                }
            }
            LED_Ctrl(LED2, RVS);      //LED指示程序运行状态 

            ImageDealState_Now = Image1_Wait;
            ////	   //MedianFilter();未定义状态
            DataSend();
        }
#endif        
    }
}

uint8 ImageSizeToTFTSize(uint8 ImageSize, uint8 IfWidth)
{
    if (IfWidth)
    {
        return ImageSize * (159.0 / 187.0);
    }
    else
    {
        return ImageSize * (121.0 / 119);
    }
}

void Init_All()
{
    DisableInterrupts;           //关闭中断
    PLL_Init(PLL235);            //初始化PLL为200M,总线为40M  

    /*******************GPIO***************************/
    LED_Init();                  //LED初始化    
    TFT_init(SPI_1, SPIn_PCS0);
    LCD_Init();
    ButtonInit();
    //UART_Init(UART_3, 256000);
    //EncoderMeasure_Init();
    //RemoteInit();
    Series_Sendout_init();
pit_time_start(PIT2);
    //PIT_Init(PIT0, 1);
    EnableInterrupts;
    
    
    TFT_showstr(0, 0, "Initing!", BLACK, WHITE);
    camera_init_1();
    camera_init_2();
    
    TFT_showstr(0, 0, "Success!", BLACK, WHITE);
    dsp_single_colour(0xffff);//全白
    
    ImageDealStructInit(&ImageDeal_Camera1);
    ImageDealStructInit(&ImageDeal_Camera2);

}

int f = 0;
extern uint32 Speed_get[4];
long Speed_watch[4];

void PIT0_Interrupt()
{
    if (f != 4)
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
            Speed_get[i] += SpeedCount[i];
        }
        f += 1;
    }
    else 
    {
        //SEND(Speed_get[0], Speed_get[1], Speed_get[2], Speed_get[3]);
        //SetSpeed_FromRemote(RunMode);
        //Series_Control(Series_deviation_received);

        int j = 0;
        for (j = 0; j < 4; j++)
        {
            Speed_watch[j] = Speed_get[j];
            Speed_get[j] = 0;
        }
        f = 0;
    }
    PIT_Flag_Clear(PIT0);       //清中断标志位
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