/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-30 15:33:25
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-09-28 15:49:38
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_SensorlessWithADC\BLDC\include\mcType.h
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef _MC_TYPE_H
#define _MC_TYPE_H

#include <stdbool.h>

#ifndef __IO
#define __IO volatile
#endif

/*本文件包含BLDC控制所需要的主要参数类型*/

//基本数据类型定义
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;
typedef unsigned long long uint64_t;
typedef signed long long int64_t;

typedef enum motorStatus{
    eMotorCalOffset = 0,
    eMotorWaitCMD = 1,
    eMotorReset = 2,
    eMotorStop = 3,
    eMotorPreAligned = 4,
    eMotorPreAlignedDelay = 5,
    eMotorOpenLoop = 6,
    eMotorCloseAlignedDelay = 7,
    eMotorCloseSpeedLoop = 8
}MotorStatus;

typedef struct 
{
    //期望的电机运行速度
    __IO uint32_t eleSpeedHope;
    //电机当前的运行状态
    __IO MotorStatus status;
    //期望的电机旋转方向，默认为0表示翻转，1表示正转
    __IO bool rotateDirectionHope;
    //电机预对齐用时
    __IO int16_t alignmentTimeCost;
    //电机预对齐用时计数
    __IO int16_t alignmentTimeCostCnt;

}BLDCControlHandler;

typedef enum mosLowOn{
    eMos_U_ON,
    eMOS_V_ON,
    eMOS_W_ON
}MosLowOn;

typedef struct 
{
    //通知当前对应哪一个MOS下端被打开
    MosLowOn whichOn;
    //当前PWM执行过的次数，也就是定时器中断发生的次数，根据这个值，我们能够计算出来电机的运行时间
    __IO uint32_t PWMTimeCost;
    //需要写入PWM的速度环占空比
    __IO uint16_t PWMSpeedDuty;
    //需要写入PWM的电流环占空比
    __IO uint16_t PWMCurrentDuty;
    //需要写入PWM的实际占空比
    __IO uint16_t PWMDuty;
    //当前的换相步数（扇区位置）
    __IO int8_t rotateSector;
}PWMHandler;

typedef struct 
{
    //母线电压
    __IO uint32_t busVoltage;
    //三相反电动势
    __IO int32_t bemfUVW[3];
    //母线电流
    __IO uint32_t busCurrent;
    //母线偏置电流
    __IO uint32_t busOffsetCurrent;
    //估算出来的的电速度
    __IO uint32_t  eSpeed;
    //估算出来的电机当前旋转的圈数(电角度)
    __IO uint16_t numOfTurns;
    //扇区跃变错误计数
    __IO uint8_t sectorCnt;
    //判断计数
    __IO int8_t dirCnt;
    //上次所在的扇区
    __IO int8_t lastSector;
    //转子所在扇区
    __IO int8_t rotorSector;
    //UVW相过零信号计数
    __IO int8_t filterZeroCnt;
    //估计出来的电机当前真实旋转的方向,默认为0表示翻转，1表示正转
    __IO bool rotateDirection;
    //扇区跃变错误
    __IO bool sectorError;
}SensorlessHandler;

typedef enum userCmd{
    eCMD_NONE = 0,
    eCMD_STOP = 1,
    eCMD_START = 2,
    eCMD_REVERSE = 3,
    eCMD_SETSPEED = 4,
    eCMD_SETDIR = 5,
    eCMD_ACC = 6
}UserCmd;

typedef struct 
{
    //用户设定的速度参数
    __IO uint32_t speedSet;
    //用户设定的加速用时
    __IO uint32_t timeExc;
    //用户设定的正反转方向
    __IO bool dirSet;
    //用户命令缓冲区
    __IO UserCmd cmdBuffer;
}CMDHandler;

#endif


