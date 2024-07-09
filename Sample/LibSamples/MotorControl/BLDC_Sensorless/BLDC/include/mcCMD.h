/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-12 14:21:54
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-28 19:53:57
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_SensorlessWithADC\BLDC\include\mcCMD.h
 * @Description: 可供用户直接调用的方波控制函数
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */

#ifndef _MC_CMD_H
#define _MC_CMD_H

#include "mcGlobal.h"

extern void BLDC_CmdInit(void);
extern void BLDC_Exc(void);
extern void BLDC_Delay(uint32_t delay);
extern void BLDC_CMD_WaitExc(void);
extern void BLDC_CMD_SetSpeed(uint32_t eleSpeed);
extern void BLDC_CMD_SetSpeedWithACC(uint32_t eleSpeed,uint32_t excTime);
extern void BLDC_CMD_Reverse(void);
extern void BLDC_CMD_SetDirection(bool dir);
extern void BLDC_CMD_Start(void);
extern void BLDC_CMD_Stop(void);

#endif



