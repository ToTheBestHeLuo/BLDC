/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-30 15:31:56
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-08-31 16:07:07
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_Sensorless\BLDC\include\mcTable.h
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef _MC_TABLE_H
#define _MC_TABLE_H

#include "mcConfig.h"
/*本文件包含BLDC控制所需要的换相操作*/

extern void (*MotorReverseTable[6])(PWMHandler*);
extern void (*MotorForwardTable[6])(PWMHandler*);

#endif


