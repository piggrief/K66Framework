#include "include.h" 

float Series_deviation_received = 0;
extern float average_piancha;
///<summary>速度环参数</summary>
float P_Set[4] = {33, 30, 30, 30};//
float D_Set[4] = {12, 12, 12, 12};//
float I_Set[4] = {5, 5, 5, 5};//{
float DeadBand_Set[4] = {0, 0, 0, 0};
float I_limit = 3000;
float Max_output = 9500;

float Wheel_Distance = 0;

extern float SpeedCount[4];
void Init_All();
extern float angle_GYRO_z;

float LineCCDerror = 0;
uint8 image_oneline[128];
uint8 Flag_MeasureDistance = 1;
uint32 time_consume = 0;

void main(void)
{
    Init_All();
    PID_Speedloop_init(P_Set, D_Set, I_Set, I_limit, Max_output, DeadBand_Set);
    //PID_locationloop_init(1.6, 0.2, 0, 0, 180, 0);//位置环参数
    PID_locationloop_init(2.5, 6.5, 0, 0, 180, 0);
    
    dsp_single_colour(WHITE);
    while (1)
    {
        //READ_MPU9250_GYRO();
        TFT_showint16(0, 0, (int)(angle_GYRO_z * 10),BLACK,WHITE);

        TFT_showint16(0, 2, (int)(Wheel_Distance / 10), BLACK, WHITE);
        //TFT_showint16(50, 2, (int)(Wheel_Distance / 10), BLACK, WHITE);
        
        //TSL1401_GetLine(image_oneline);
        //lookline(image_oneline);
        //LineCCDerror = CalPiancha();
        LCD_CLS();
        LCD_ShowOneLineImage(image_oneline);
        LCD_PrintInt3(0,0,(int)(LineCCDerror));

        //if (Flag_MeasureDistance)
        //{
            Get_Gyro(&GYRO_OriginData);
            GetAngle_FromGYRO(GYRO_OriginData.X, GYRO_OriginData.Y, GYRO_OriginData.Z);
        //}
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
    //BatteryVoltageCollect_Init(1);
    InitMPU6050();//陀螺仪
    //ConfigMPU9250();
    //ButtonMenu();
    UART_Init(UART_0, 115200);
    EncoderMeasure_Init();
    RemoteInit();
    Series_Sendout_init();
    PIT_Init(PIT0, 3);//中断定时为3ms
    EnableInterrupts;
    TFT_showstr(0, 0, "Initing!", BLACK, WHITE);
    //TSL1401_Init();
    //TSL1401_GetLine(image_oneline);
    camera_init();
    TFT_showstr(0, 0, "Success!", BLACK, WHITE);
    GPIO_Init(BeepIOPortType, BeepIOPortIndex, GPO, 1);
    
}


int f = 0;
extern float Speed_get[4];
float Speed_watch[4];
extern struct PIDControl Car_Speed_Rotate;
extern struct RunSpeed RS_Now;

struct RunSpeed RSTest;
float TargetAngle = 0;
float LeftWheel_Buff = 0;
float RightWheel_Buff = 0;

typedef enum
{
    Left_Turn,
    Right_Turn,
    Go,
    Return_Right_Turn,
    Return_Left_Turn,
    Stop
}ConstAngle;
ConstAngle CA1 = Left_Turn;

float rate_distoangle = 0.014;
float angle_const = 50;
uint32 starttime = 0;
void PIT0_Interrupt()
{
    if (f != 4)
    {
        GPIO_Ctrl(BeepIOPortType, BeepIOPortIndex, 1);
        SpeedClean();
        int i = 0;
        for (i = 0; i < 4; i++)
        {
            GetSpeed(i);
            Speed_get[i] += SpeedCount[i];
        }
        GPIO_Ctrl(BeepIOPortType, BeepIOPortIndex, 0);

        f += 1;
    }
    else 
    {
        //CalWholeSpeed(&RSTest, Speed_watch);

        //遥控测试程序
        //SEND(Speed_get[0], Speed_get[1], Speed_get[2], Speed_get[3]);
        //SetSpeed_FromRemote(RunMode);
        
        //if (Series_deviation_received - 94 < 0)
        //PID_SetTarget(&Car_Speed_Rotate, -9);
        //else 
        //PID_SetTarget(&Car_Speed_Rotate,9);
        //PID_SetTarget()
        ControlCar_FromAnalog();
        //Series_Control(0);
        if (CA1 == Left_Turn)
        {
            if (Flag_MeasureDistance)
            {
                LeftWheel_Buff = 0.5 * (Speed_get[0] + Speed_get[2]);
                RightWheel_Buff = 0.5 * (Speed_get[1] + Speed_get[3]);
                Wheel_Distance += 0.5 * (LeftWheel_Buff + RightWheel_Buff);
                TargetAngle = rate_distoangle*Wheel_Distance;
                if (TargetAngle >= angle_const)
                {
                    TargetAngle = angle_const;
                    Wheel_Distance = 0;
                    CA1 = Right_Turn;
                    //Flag_MeasureDistance = 0;
                }
            }
        }
        else if (CA1 == Right_Turn)
        {
            if (Flag_MeasureDistance)
            {
                LeftWheel_Buff = 0.5 * (Speed_get[0] + Speed_get[2]);
                RightWheel_Buff = 0.5 * (Speed_get[1] + Speed_get[3]);
                Wheel_Distance += 0.5 * (LeftWheel_Buff + RightWheel_Buff);
                TargetAngle = angle_const - rate_distoangle*Wheel_Distance;
                if (TargetAngle <= 0)
                {
                    //angle_GYRO_z = 0;
                    TargetAngle = 0;
                    Wheel_Distance = 0;
                    CA1 = Go;
                    //Flag_MeasureDistance = 0;
                }
            }
        }
        else if (CA1 == Go)
        {
            if (Flag_MeasureDistance)
            {
                LeftWheel_Buff = 0.5 * (Speed_get[0] + Speed_get[2]);
                RightWheel_Buff = 0.5 * (Speed_get[1] + Speed_get[3]);
                Wheel_Distance += 0.5 * (LeftWheel_Buff + RightWheel_Buff);
                //TargetAngle = 45 - 0.014*Wheel_Distance;
                TargetAngle = 0;
                if (Wheel_Distance >= 1300)
                {
                    //angle_GYRO_z = 0;
                    TargetAngle = 0;
                    Wheel_Distance = 0;
                    CA1 = Return_Right_Turn;
                    //Flag_MeasureDistance = 0;
                }
            }
        }
        else if (CA1 == Return_Right_Turn)
        {
            if (Flag_MeasureDistance)
            {
                LeftWheel_Buff = 0.5 * (Speed_get[0] + Speed_get[2]);
                RightWheel_Buff = 0.5 * (Speed_get[1] + Speed_get[3]);
                Wheel_Distance += 0.5 * (LeftWheel_Buff + RightWheel_Buff);
                TargetAngle = -rate_distoangle*Wheel_Distance;
                if (TargetAngle <= -angle_const)
                {
                    //angle_GYRO_z = 0;
                    TargetAngle = -angle_const;
                    Wheel_Distance = 0;
                    CA1 = Return_Left_Turn;
                    //Flag_MeasureDistance = 0;
                }
            }
        }
        else if (CA1 == Return_Left_Turn)
        {
            if (Flag_MeasureDistance)
            {
                LeftWheel_Buff = 0.5 * (Speed_get[0] + Speed_get[2]);
                RightWheel_Buff = 0.5 * (Speed_get[1] + Speed_get[3]);
                Wheel_Distance += 0.5 * (LeftWheel_Buff + RightWheel_Buff);
                TargetAngle = -angle_const + rate_distoangle*Wheel_Distance;
                if (TargetAngle >= 0)
                {
                    //angle_GYRO_z = 0;
                    TargetAngle = 0;
                    Wheel_Distance = 0;
                    CA1 = Stop;
                    Flag_MeasureDistance = 0;
                }
            }
        }
        if (CA1 == Stop)
        {
            TargetAngle = 0;
        }
        Series_Control(angle_GYRO_z - TargetAngle);
        //Series_Control(LineCCDerror);
        //编码器观测
        int j = 0;
        for (j = 0; j < 4; j++)
        {
            Speed_watch[j] = Speed_get[j];
            Speed_get[j] = 0;//速度计清零
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