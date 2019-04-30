#include "include.h" 

float Series_deviation_received = 0;
extern float average_piancha;
///<summary>速度环参数</summary>
float P_Set[4] = {33, 30, 33, 30};//{18, 18, 19, 18} 33
float D_Set[4] = {12, 10, 11, 10};//12
float I_Set[4] = {5, 5, 5, 5};//{5, 5, 5, 5};//3
float DeadBand_Set[4] = {709-27, 660-27, 680-28, 660-27};//{700, 680, 680, 710}
float I_limit = 8000;
float Max_output = 9500;

extern uint32 SpeedCount[4];
void Init_All();
extern float angle_GYRO_z;
void main(void)
{
    Init_All();
    PID_Speedloop_init(P_Set, D_Set, I_Set, I_limit, Max_output, DeadBand_Set);
    //PID_locationloop_init(1.6, 0.2, 0, 0, 180, 0);//位置环参数
    PID_locationloop_init(2.5, 0, 0, 0, 180, 0);

    dsp_single_colour(WHITE);
    while (1)
    {
        TFT_showint16(0, 0, (int)(angle_GYRO_z * 10),BLACK,WHITE);


        if (ImageDealState_Now == Image_CollectFinish)
        {
            ImageDealState_Now = Image_Dealing;
            LED_Ctrl(LED3, ON);      //LED????????????  

            Reset();
	    find_light();
	    mid_findlight();
	    find_loop();
            
	    displayimage032(image[0], ThreasHold);

                                          
            //seekfree_sendimg_032();                         
            ImageDealState_Now = Image_DealingFinsh;
        }
        else
            LED_Ctrl(LED3, OFF);      //LED???????????? 
        

        GetRemoteCMDData();
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
    Motor_init();
    BatteryVoltageCollect_Init(1);
    InitMPU6050();//陀螺仪
    ButtonMenu();
    //UART_Init(UART_3, 115200);
    EncoderMeasure_Init();
    RemoteInit();
    Series_Sendout_init();
    PIT_Init(PIT0, 3);//中断定时为3ms
    EnableInterrupts;
    TFT_showstr(0, 0, "Initing!", BLACK, WHITE);
    //camera_init();
    TFT_showstr(0, 0, "Success!", BLACK, WHITE);
}

int f = 0;
extern uint32 Speed_get[4];
long Speed_watch[4];
extern struct PIDControl Car_Speed_Rotate;
extern struct RunSpeed RS_Now;
void PIT0_Interrupt()
{
    if (f != 4)
    {
        SpeedClean();
        int i = 0;
        for (i = 0; i < 4; i++)
        {
            GetSpeed(i);
            Speed_get[i] += SpeedCount[i];
        }
        f += 1;
    }
    else 
    {

        //遥控测试程序
        //SEND(Speed_get[0], Speed_get[1], Speed_get[2], Speed_get[3]);
        SetSpeed_FromRemote(RunMode);
        
      //  if (Series_deviation_received - 94 < 0)
      //    PID_SetTarget(&Car_Speed_Rotate, -9);
      //else 
      //    PID_SetTarget(&Car_Speed_Rotate,9);
        //PID_SetTarget()




        //编码器观测
        int j = 0;
        for (j = 0; j < 4; j++)
        {
            Speed_watch[j] = Speed_get[j];
            Speed_get[j] = 0;//速度计清零
        }
      
        Get_Gyro(&GYRO_OriginData);
        GetAngle_FromGYRO(GYRO_OriginData.X, GYRO_OriginData.Y, GYRO_OriginData.Z);
        Series_Control(angle_GYRO_z - 0);
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