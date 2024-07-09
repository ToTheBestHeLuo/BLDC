/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-30 15:31:50
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-11 09:46:00
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_Sensorless\BLDC\mcSource\mcTable.c
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include "../include/mcTable.h"

//电机强拖动正转换相，函数指针数组
void (*MotorForwardTable[6])(PWMHandler*) = {
    &BLDC_UH_VL,&BLDC_UH_WL,&BLDC_VH_WL,&BLDC_VH_UL,&BLDC_WH_UL,&BLDC_WH_VL
};

//电机强拖反转换相，函数指针数组
void (*MotorReverseTable[6])(PWMHandler*) = {
    &BLDC_WH_VL,&BLDC_UH_VL,&BLDC_UH_WL,&BLDC_VH_WL,&BLDC_VH_UL,&BLDC_WH_UL
};


