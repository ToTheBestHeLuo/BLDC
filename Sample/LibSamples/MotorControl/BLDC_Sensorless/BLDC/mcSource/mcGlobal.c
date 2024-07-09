/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-30 15:30:16
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-10-09 17:07:16
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_SensorlessWithADC\BLDC\mcSource\mcGlobal.c
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include "../include/mcGlobal.h"

BLDCControlHandler bldcControlHandler;
PWMHandler pwmHandler;
SensorlessHandler sensorlessHandler;
CMDHandler cmdHandler;

/**
 * @description: 全局复位，所有在mcGloabl.h（.c）中出现的全局变量都会被复位
 * @available param list: none
 * @return {*}
 */
void resetALL(void)
{
    resetBLDCHandler(&bldcControlHandler);
    resetPWMHandler(&pwmHandler);
    resetSensorlessHandler(&sensorlessHandler);
}
/**
 * @description: 复位BLDC
 * @param {BLDCControlHandler*} bldcHandler
 * @available param list: none
 * @return {*}
 */
void resetBLDCHandler(BLDCControlHandler* bldcHandler)
{
    bldcHandler->rotateDirectionHope = cmdHandler.dirSet;
    bldcHandler->status = eMotorReset;
    bldcHandler->alignmentTimeCostCnt = 0;
    bldcHandler->alignmentTimeCost = 0;
    bldcHandler->eleSpeedHope = cmdHandler.speedSet;
}
/**
 * @description: 复位PWM
 * @param {PWMHandler*} pwmHandler
 * @available param list: none
 * @return {*}
 */
void resetPWMHandler(PWMHandler* pwmHandler)
{
    pwmHandler->PWMTimeCost = 0;
    pwmHandler->PWMCurrentDuty = BLDC_PWM_START_DUTY;
    pwmHandler->PWMSpeedDuty = BLDC_PWM_START_DUTY;
    pwmHandler->PWMDuty = BLDC_PWM_START_DUTY;
    pwmHandler->rotateSector = 0;
}
/**
 * @description: 复位Sensorless
 * @param {SensorlessHandler*} sensor
 * @available param list: none
 * @return {*}
 */
void resetSensorlessHandler(SensorlessHandler* sensor)
{
    sensor->sectorCnt = 0;
    sensor->sectorError = false;
    sensor->lastSector = 0;
    sensor->dirCnt = 0;
    sensor->filterZeroCnt = 0;
    sensor->rotateDirection = false;
    sensor->numOfTurns = 0;
    sensor->rotorSector = 0;
    sensor->eSpeed = 0;
    sensor->busCurrent = 0;
    sensor->busVoltage = 0;
}

