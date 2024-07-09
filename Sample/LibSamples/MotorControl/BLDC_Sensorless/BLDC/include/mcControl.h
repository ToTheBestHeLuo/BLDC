/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-30 16:28:57
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-07 10:50:22
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_Sensorless\BLDC\include\mcControl.h
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef _MC_CONTROL_H
#define _MC_CONTROL_H

#include "mcGlobal.h"
#include "mcTable.h"
#include "mcConfig.h"
#include "mcSensorless.h"

/*本文件包含BLDC从启动到平稳运行的一系列函数*/

extern void MotorHighFrequencyStateMachine(void);

extern void MotorForcePWMDutyChange(BLDCControlHandler* bldc,PWMHandler* pwm);
extern void MotorForceCommutation(bool forceDirect,PWMHandler* pwm);
extern void MotorForceAlignment(BLDCControlHandler* bldc,PWMHandler* pwm);
#endif




