/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-12 14:21:48
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-10-08 15:26:59
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_SensorlessWithADC\BLDC\mcConfig\mcCMD.c
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include "../include/mcCMD.h"

/**
 * @description: 初始化命令控制缓冲区
 * @available param list: none
 * @return {*}
 */
void BLDC_CmdInit(void)
{
    CMDHandler* cmd = &cmdHandler;
    cmd->cmdBuffer = eCMD_NONE;
}
//延迟函数使用
static __IO uint32_t timeDelay = 0;
/**
 * @description: 应当被Systick等低频中断调用，主要负责一系列安全任务以及控制命令的处理
 * @available param list: none
 * @return {*}
 */
void BLDC_Exc(void)
{
    static uint16_t excDelay = 0;

    BLDCControlHandler* bldc = &bldcControlHandler;
    SensorlessHandler* sensor = &sensorlessHandler;
    CMDHandler* cmd = &cmdHandler;

    int32_t tmp;
    int32_t acc;

    if(0u != timeDelay)
    {
        timeDelay--;
    }

    if(excDelay++ > 25)
    {
        excDelay = 0;
        UserCmd cm = cmd->cmdBuffer;
        if(bldc->status != eMotorCalOffset && cm != eCMD_NONE)
        {
            switch (cm)
            {
                case eCMD_STOP:
                    stopBLDC();
                    bldc->status = eMotorWaitCMD;
                    cmd->cmdBuffer = eCMD_NONE;
                    break;
                case eCMD_START:
                    bldc->status = eMotorReset;
                    cmd->cmdBuffer = eCMD_NONE;
                    break;
                case eCMD_REVERSE:
                    bldc->rotateDirectionHope = cmd->dirSet;
                    cmd->cmdBuffer = eCMD_NONE;
                    break;
                case eCMD_SETSPEED:
                    bldc->eleSpeedHope = cmd->speedSet;
                    cmd->cmdBuffer = eCMD_NONE;
                    break;
                case eCMD_SETDIR:
                    bldc->rotateDirectionHope = cmd->dirSet;
                    cmd->cmdBuffer = eCMD_NONE;
                    break;
                case eCMD_ACC:
                    tmp = cmd->speedSet - sensor->eSpeed;
                    if(tmp > 0)
                    {
                        acc = hardwareUnsignedDiv(tmp,cmd->timeExc);
                        //更新期望速度
                        bldc->eleSpeedHope += acc;
                    }
                    else
                    {
                        tmp = -tmp;
                        acc = hardwareUnsignedDiv(tmp,cmd->timeExc);
                        //更新期望速度
                        bldc->eleSpeedHope -= acc;
                    }

                    if(cmd->timeExc-- == 0)
                    {
                        bldc->eleSpeedHope = cmd->speedSet;
                        cmd->cmdBuffer = eCMD_NONE;
                    }
                    break;
                default:
                    break;
            }
        }
    }

}
/**
 * @description: 检查命令缓冲区是否能容纳下新的命令
 * @available param list: none
 * @return {bool} true : 能容纳，false ： 不能容纳
 */
bool checkCmdBufferIsFree(void)
{
    CMDHandler* cmd = &cmdHandler;
    if(cmd->cmdBuffer == eCMD_NONE)
        return true;
    else
        return false;
}
/**
 * @description: 延迟函数，应当以此作为系统的唯一延迟函数
 * @param {uint32_t} delay
 * @available param list: none
 * @return {*}
 */
void BLDC_Delay(uint32_t delay)
{
    timeDelay = delay;
    while(timeDelay != 0u);
}
/**
 * @description: 等待命令执行完成
 * @available param list: none
 * @return {*}
 */
void BLDC_CMD_WaitExc(void)
{
    CMDHandler* cmd = &cmdHandler;
    while(cmd->cmdBuffer != eCMD_NONE);
}
/**
 * @description: 设置目标转速
 * @param {uint32_t} eleSpeed
 * @available param list: none
 * @return {*}
 */
void BLDC_CMD_SetSpeed(uint32_t eleSpeed)
{
    CMDHandler* cmd = &cmdHandler;
    if(checkCmdBufferIsFree())
    {
        cmd->speedSet = eleSpeed;
        cmd->cmdBuffer = eCMD_SETSPEED;
    }
}
/**
 * @description: 设置目标转速，并且设置达到目标转速所需要的时间（可折算成加速度）
 * @param {uint32_t} eleSpeed
 * @param {uint32_t} excTime
 * @available param list: none
 * @return {*}
 */
void BLDC_CMD_SetSpeedWithACC(uint32_t eleSpeed,uint32_t excTime)
{
    CMDHandler* cmd = &cmdHandler;
    if(checkCmdBufferIsFree())
    {
        cmd->timeExc = excTime;
        cmd->speedSet = eleSpeed;
        cmd->cmdBuffer = eCMD_ACC;
    }
}
/**
 * @description: 电机反转
 * @available param list: none
 * @return {*}
 */
void BLDC_CMD_Reverse(void)
{
    CMDHandler* cmd = &cmdHandler;
    if(checkCmdBufferIsFree())
    {
        cmd->dirSet = !cmd->dirSet;
        cmd->cmdBuffer = eCMD_REVERSE;
    }
}
/**
 * @description: 
 * @param {bool} dir
 * @available param list: none
 * @return {*}
 */
void BLDC_CMD_SetDirection(bool dir)
{
    CMDHandler* cmd = &cmdHandler;
    if(checkCmdBufferIsFree())
    {
        cmd->dirSet = dir;
        cmd->cmdBuffer = eCMD_SETDIR;
    }
}
/**
 * @description: 启动电机！
 * @available param list: none
 * @return {*} 
 */
void BLDC_CMD_Start(void)
{
    CMDHandler* cmd = &cmdHandler;
    if(checkCmdBufferIsFree())
    {
        cmd->cmdBuffer = eCMD_START;
    }
}
/**
 * @description: 停止电机！
 * @available param list: none
 * @return {*}
 */
void BLDC_CMD_Stop(void)
{
    CMDHandler* cmd = &cmdHandler;
    if(checkCmdBufferIsFree())
    {
        cmd->cmdBuffer = eCMD_STOP;
    }
}



