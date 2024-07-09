/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-31 15:25:37
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-10-09 17:39:55
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_SensorlessWithADC\BLDC\mcSource\mcSensorless.c
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include "../include/mcSensorless.h"
//扇区映射表（注意第一个索引是无效的，只是为了方便后续计算）
static const int8_t sectorMapping[1 + 6] = {0,1,3,2,5,0,4};
/**
 * @description: 过零检测函数并计算出扇区以及对应的速度，并能进一步判断出当前电机的转向
 * @param {SensorlessHandler*} sensor
 * @param {bool*} reset
 * @available param list: none
 * @return {bool} true：稳定的扇区转变信号，false：不稳定的信号
 */
bool MotorZeroCore(SensorlessHandler* sensor)
{
    //记录上一次的信号
    static uint32_t lastSig = 0;

    //获取本次的比较信号

#ifndef BLDC_METHOD_ADC
    /*采用三路ADC采集反电动势的方案*/
    uint32_t cmpSig[3] = {BLDC_ReadHallLessU(),BLDC_ReadHallLessV(),BLDC_ReadHallLessW()};
#else
    uint32_t cmpSig[3];
    int32_t adcDat[3] = {BLDC_ReadHallLessU(),BLDC_ReadHallLessV(),BLDC_ReadHallLessW()};
    int32_t total = adcDat[0] + adcDat[1] + adcDat[2];
    for (int i = 0; i < 3; i++)
    {
        int32_t bemf = adcDat[i] * 3;
        sensor->bemfUVW[i] = bemf - total;
        if(sensor->bemfUVW[i] > 0)
            cmpSig[i] = 1u;
        else
            cmpSig[i] = 0u;
    }
#endif

    uint32_t nowSig = (cmpSig[0] * 1) + (cmpSig[1] * 2) + (cmpSig[2] * 4);

    int8_t lastSector = sensor->lastSector;

    if(sensor->sectorCnt >= 7)
    {
        sensor->sectorError = true;
        return false;
    }

    if(nowSig != lastSig)
    {
        sensor->filterZeroCnt = 0;
        lastSig = nowSig;
    }
    else
    {
        if(++sensor->filterZeroCnt > BLDC_ZERO_CROSS_FILTER_CNT)
        {
            //能够进入这里就说明3个比较信号全稳定了
            sensor->filterZeroCnt = 0;
            //查看是否是合法的扇区
            if(nowSig == 0x00 || nowSig == 0x07)
            {
                return false;
            }

            //查表得到转子当前所在的扇区
            int8_t nowSector = sectorMapping[nowSig];
            //判断是否发生了扇区切换并确定转向
            if(lastSector != nowSector)
            {
                //转子转向的判断(正转或者反转)
                if(lastSector == 0 && nowSector == 1)
                {
                    if(++sensor->dirCnt == +5)
                    {
                        sensor->dirCnt = 0;
                        sensor->rotateDirection = true;
                    }
                    sensor->numOfTurns++;
                }
                else if(lastSector == 0 && nowSector == 5)
                {
                    if(--sensor->dirCnt == -5)
                    {
                        sensor->dirCnt = 0;
                        sensor->rotateDirection = false;             
                    }
                    sensor->numOfTurns++;
                }

                int8_t err = lastSector - nowSector;
                err = err > 0 ? err : -err;

                if(err != 1 && err != 5)
                {
                    sensor->sectorCnt++;
                }
                        
                sensor->lastSector = nowSector;
                sensor->rotorSector = nowSector;

                return true;
            }
            
            //如果没发生扇区切换的情况就返回false
            return false;
        }
    }
    
    return false;
}

