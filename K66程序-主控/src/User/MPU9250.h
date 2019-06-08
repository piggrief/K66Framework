/*
 * MPU9250.h
 *
 *  Created on: August 24, 2016
 *      Author: PAk
 *      https://e2e.ti.com/members/521000
 */
#ifndef _MPU9250_H
#define _MPU9250_H



void ConfigMPU9250();
//void CalibrateMPU9250(float * dest1, float * dest2);
//void MPU9250SelfTest(float * destination) ;
//void ConfigAK8963(float * destination);
//int16 ReadTempData();
//void ReadMagData(int16 * destination);
//void ReadGyroData(int16 * destination);
//void ReadAccelData(int16 * destination);
//void ResetMPU9250();
//void Setup();
//void MahonyQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
//void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
//void UpdateData();
//void loop();
//
//void GetAres();
//void GetGres();
//void GetMres();
//
//void delayMS(int ms) ;
//
//void delay_uS(int us) ;

extern float pfData[16];
extern float *pfAccel, *pfGyro, *pfMag, *pfEulers, *pfQuaternion;


#endif
