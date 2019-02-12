/*!
* @file       MecanumWheelControl.h
* @brief      四轮麦轮控制部分的程序头文件
* @details    
* @author     pig's grief
* @version    v1.0
* @date       2019-2-12
* @to do      编写四轮麦轮运动控制解算
*/

# ifndef _MECANUMWHEELCONTROL_H
# define _MECANUMWHEELCONTROL_H

/*
定义轮子顺序为：
                1：右前轮
                2：左前轮
                3：左后轮
                4：右后轮
*/
# define FTMType_Use FTM0
# define FTMChannel_Use_W1_1 FTM_CH0
# define FTMChannel_Use_W1_2 FTM_CH1
# define FTMChannel_Use_W2_1 FTM_CH2
# define FTMChannel_Use_W2_2 FTM_CH3
# define FTMChannel_Use_W3_1 FTM_CH4
# define FTMChannel_Use_W3_2 FTM_CH5
# define FTMChannel_Use_W4_1 FTM_CH6
# define FTMChannel_Use_W4_2 FTM_CH7

//RunState麦轮车运动速度
//用三个速度来表示麦轮车的运动速度
typedef struct 
{
    float XSpeed;   //x轴(左右)平动速度
    float YSpeed;   //y轴(前后)平动速度
    float YawSpeed; //yaw轴转动的角速度
}RunSpeed;

typedef enum
{
    P_Control,
    PD_Control,
    PI_Control,
    PID_Control
}PIDControlModel;

typedef struct PIDControl
{
    float KP;
    float KD;
    float KI;

    float P_Output;
    float D_Output;
    float I_Output;
    float Output;
    float Last_Output;

    float MeasuredValue;
    float TargetValue;
    float Error;
    float Last_Error;

    float IntegralLimit;
    float MaxOutput;
    float DeadBand;
    /*
    * @brirf                PIDControl结构体的构造函数，用于初始化该结构体
    * @param-PID            要构造的结构体
    * @param-P_set          KP参数的设定值
    * @param-D_set          KD参数的设定值
    * @param-I_set          KI参数的设定值
    * @param-I_Limit_Set    积分限幅上限
    * @param-MaxOutput_Set  最大电机输出，即电机限幅
    * @param-DeadBand_Set   电机死区设定值
    * @return        void
    * @note
    * @example       PIDTest.f_Constructor(&PIDTest, 20, 0, 0, 2000, 9600, 0);
    */
    void(*f_Constructor)(struct PIDControl* PID, float P_set, float D_set, float I_set
        , float I_Limit_Set, float MaxOutput_Set, float DeadBand_Set);
    /*
    * @brirf               根据本次测量了计算并获得本次的PID控制量
    * @param-PID           确定是哪个PID控制
    * @param-PIDModel      确定控制器：纯P、PD、PI、PID
    * @param-MeasuredValue 本次测量值
    * @return              本次PID控制量
    * @note
    * @example             out = PIDTest.f_GetPIDControlValue(&PIDTest, PID_Control, Measure);
    */
    float(*f_GetPIDControlValue)(struct PIDControl* PID, PIDControlModel PIDModel, float MeasuredValue);
    /*
    * @brirf               PID控制参数刷新
    * @param-PID           确定是哪个PID控制
    * @param-kp            要更新的参数KP
    * @param-kd            要更新的参数KD
    * @param-ki            要更新的参数KI
    * @note
    * @example             PIDTest.f_para_Refresh(&PIDTest, 20, 1, 0.1);
    */
    void(*f_para_Refresh)(struct PIDControl* PID, float kp, float kd, float ki);
}PIDTest;

#endif