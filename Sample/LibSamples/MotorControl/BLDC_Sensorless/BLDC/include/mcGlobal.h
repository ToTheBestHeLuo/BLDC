/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-30 15:20:01
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-12 14:38:37
 * @FilePath: \BLDC\include\mcGlobal.h
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef _MC_GLOBAL_H
#define _MC_GLOBAL_H

#include "mcType.h"
#include "mcConfig.h"

/*本头文件包含了所有的全局变量，仅供BLDC控制时使用*/

extern BLDCControlHandler bldcControlHandler;
extern PWMHandler pwmHandler;
extern SensorlessHandler sensorlessHandler;
extern CMDHandler cmdHandler;

extern void resetALL(void);
extern void resetBLDCHandler(BLDCControlHandler* bldcHandler);
extern void resetPWMHandler(PWMHandler* pwmHandler);
extern void resetSensorlessHandler(SensorlessHandler* sensor);
#endif

