/*!
* @file       MecanumWheelControl.h
* @brief      四轮麦轮控制部分程序头文件
* @details    
* @author     pig's grief
* @version    v1.0
* @date       2019-2-12
* @History    
*             v1.0:2019-2-12 构建了RunSpeed、PIDControl两大结构体
                             编写函数PIDControl_Constructor、PIDControl_FunctionDefine
                             编写了PID控制的主要函数：GetPIDControlValue、Para_Refresh
                             编写电机输出函数:MotorOutput
                             !!以上函数均为测试
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

typedef struct
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

    ///<summary>PIDControl结构体的构造函数，用于初始化该结构体</summary>
    void(*f_Constructor)(PIDControl* PID, float P_set, float D_set, float I_set,
                         float I_Limit_Set, float MaxOutput_Set, float DeadBand_Set);
    //获得PID控制量
    float(*f_GetPIDControlValue)(PIDControl* PID, PIDControlModel PIDModel,float MeasuredValue);
    //PID参数的刷新
    void(*f_para_Refresh)(PIDControl* PID, float kp, float kd, float ki);
}PIDControl;

#endif