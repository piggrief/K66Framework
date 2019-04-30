/*!
* @file       MPU6050Drive.h
* @brief      MPU6050驱动部分的程序头文件
* @details    
* @author     pig's grief
* @version    v1.0
* @date       2019-2-23
* @to do      
*/

# ifndef _MPU6050DRIVE_H
# define _MPU6050DRIVE_H

//#define MPU6050UseDMP 1


//三轴角加速度
struct GYROData
{
    float X;
    float Y;
    float Z;
    float Offset_X;
    float Offset_Y;
    float Offset_Z;
};

//三轴加速度
struct AccelData
{
    float X;
    float Y;
    float Z;
    float Offset_X;
    float Offset_Y;
    float Offset_Z;

};

//三轴角度
struct AngleData
{
    float X;
    float Y;
    float Z;

};

extern struct GYROData GYRO_OriginData;
extern struct AccelData Accel_OriginData;

// 定义MPU6050内部地址
#define	MPU6050_DEV_ADDR	    0xD0>>1	//IIC写入时的地址字节数据，+1为读取

#define	SMPLRT_DIV				0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG					0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG				0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	        0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define INT_PIN_CFG	  			0x37    //设置6050辅助I2C为直通模式寄存器
#define	ACCEL_XOUT_H	        0x3B
#define	ACCEL_XOUT_L	        0x3C
#define	ACCEL_YOUT_H	        0x3D
#define	ACCEL_YOUT_L	        0x3E
#define	ACCEL_ZOUT_H	        0x3F
#define	ACCEL_ZOUT_L	        0x40
#define	GYRO_XOUT_H				0x43
#define	GYRO_XOUT_L				0x44	
#define	GYRO_YOUT_H				0x45
#define	GYRO_YOUT_L				0x46
#define	GYRO_ZOUT_H				0x47
#define	GYRO_ZOUT_L				0x48
#define User_Control 			0x6A    //关闭6050对辅助I2C设备的控制
#define	PWR_MGMT_1				0x6B	//电源管理，典型值：0x00(正常启用)
#define	WHO_AM_I				0x75	//IIC地址寄存器(默认数值0x68，只读)

///DMP////////////////////////////////////////
#ifdef MPU6050UseDMP


#endif

void InitMPU6050(void);
void Get_AccData(struct AccelData * Accel_Data);
void Get_Gyro(struct GYROData * GYRO_Data);
void Get_Offset(struct AccelData * Accel_Data, struct GYROData * GYRO_Data);
int MPU6050DMP_Init();
extern int dmp_read_fifo(short *gyro, short *accel, long *quat,
unsigned long *timestamp, short *sensors, unsigned char *more);


#endif