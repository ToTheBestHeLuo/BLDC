/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-30 16:38:15
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-10-10 08:53:26
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_SensorlessWithADC\BLDC\mcSource\mcControl.c
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include "../include/mcControl.h"

#define FirstOrderLPF(lpfOut,lastOut,nowInput,ARG_A,ARG_B) (lpfOut = (((ARG_A) * lastOut) + (ARG_B - ARG_A) * nowInput) / ARG_B)

#ifdef BLDC_CLOSE_PID
static int32_t currentIntegrator = 0;
static int32_t lastCurrentErr = 0;
#endif

/**
 * @description: 电流闭环控制
 * @param {uint32_t} curMax
 * @param {uint32_t} curNow
 * @param {pwmHandler*} pwm
 * @available param list: none
 * @return {*}
 */
void MotorCurrentCloseLoopControl(uint32_t curMax,uint32_t curNow,PWMHandler* pwm)
{
#ifdef BLDC_CLOSE_PID
    int32_t nowErr = curMax - curNow;

    currentIntegrator += nowErr;

    if(currentIntegrator > BLDC_PID_I_LIMIT)
        currentIntegrator = BLDC_PID_I_LIMIT;
    else if(currentIntegrator < -BLDC_PID_I_LIMIT)
        currentIntegrator = -BLDC_PID_I_LIMIT;

    int32_t pidOut = (nowErr * BLDC_PID_P / BLDC_PID_P_DIV) + \
                    (currentIntegrator * BLDC_PID_I / BLDC_PID_I_DIV) + \
                    ((nowErr - lastCurrentErr) * BLDC_PID_D / BLDC_PID_D_DIV);

    lastCurrentErr = nowErr;

    if(pidOut < 0)
        pidOut = 0;

    if(pidOut < BLDC_PWM_CLOSE_MIN_DUTY)
        pidOut = BLDC_PWM_CLOSE_MIN_DUTY;
    else if(pidOut > BLDC_PWM_CLOSE_MAX_DUTY)
        pidOut = BLDC_PWM_CLOSE_MAX_DUTY;

    pwm->PWMCurrentDuty = pidOut;
#else
    int16_t err = curMax - curNow;

    if(err > 0)
    {
        pwm->PWMCurrentDuty++;
    }
    else
    {
        pwm->PWMCurrentDuty--;
    }

    if(pwm->PWMCurrentDuty < BLDC_PWM_CLOSE_MIN_DUTY)
    {
        pwm->PWMCurrentDuty = BLDC_PWM_CLOSE_MIN_DUTY;
    }
    else if(pwm->PWMCurrentDuty > BLDC_PWM_CLOSE_MAX_DUTY)
    {
        pwm->PWMCurrentDuty = BLDC_PWM_CLOSE_MAX_DUTY;
    }
#endif
}
#ifdef BLDC_CLOSE_PID
static int32_t speedIntegrator = 0;
static int32_t lastSpeedErr = 0;
#endif
/**
 * @description: 速度闭环控制器
 * @param {uint16_t} speedHope
 * @param {uint16_t} speedNow
 * @param {PWMHandler*} pwm
 * @available param list: none
 * @return {*}
 */
void MotorSpeedCloseLoopControl(uint16_t speedHope,uint16_t speedNow,PWMHandler* pwm)
{
#ifdef BLDC_CLOSE_PID

    int16_t nowErr = speedHope - speedNow;

    speedIntegrator += nowErr;

    if(speedIntegrator > BLDC_PID_I_LIMIT)
        speedIntegrator = BLDC_PID_I_LIMIT;
    else if(speedIntegrator < -BLDC_PID_I_LIMIT)
        speedIntegrator = -BLDC_PID_I_LIMIT;

    int32_t pid = (nowErr * BLDC_PID_P / BLDC_PID_P_DIV) + \
                    (speedIntegrator * BLDC_PID_I / BLDC_PID_I_DIV) + \
                    ((nowErr - lastSpeedErr) * BLDC_PID_D / BLDC_PID_D_DIV);

    lastSpeedErr = nowErr;

    if(pid < 0)
        pid = 0;

    if(pid < BLDC_PWM_CLOSE_MIN_DUTY)
        pid = BLDC_PWM_CLOSE_MIN_DUTY;
    else if(pid > BLDC_PWM_CLOSE_MAX_DUTY)
        pid = BLDC_PWM_CLOSE_MAX_DUTY;

    pwm->PWMSpeedDuty = pid;

#else
    int16_t err = speedHope - speedNow;

    if(err > BLDC_PWM_GATE)
    {
        pwm->PWMSpeedDuty++;
    }
    else if(err < -BLDC_PWM_GATE)
    {
        pwm->PWMSpeedDuty--;
    }

    if(pwm->PWMSpeedDuty < BLDC_PWM_CLOSE_MIN_DUTY)
    {
        pwm->PWMSpeedDuty = BLDC_PWM_CLOSE_MIN_DUTY;
    }
    else if(pwm->PWMSpeedDuty > BLDC_PWM_CLOSE_MAX_DUTY)
    {
        pwm->PWMSpeedDuty = BLDC_PWM_CLOSE_MAX_DUTY;
    }
#endif
}
/**
 * @description: 当换相延迟结束后应当调用，他会根据当前所在转子所在的相位和目标转向执行换相
 * @param {SensorlessHandler*} sensor
 * @param {PWMHandler*} pwm
 * @available param list: none
 * @return {*}
 */
void MotorCommutation(SensorlessHandler* sensor,PWMHandler* pwm)
{
    int8_t sector = sensor->rotorSector;
    if(sensor->rotateDirection)
    {
        (*MotorForwardTable[sector])(pwm);
    }
    else
    {
        (*MotorReverseTable[sector])(pwm);
    }
}
//换相需要的延迟
static uint32_t delayCommutation = 0;
//延迟换相计数
static uint32_t delayCommutationCnt = 0;
/**
 * @description: 完成BLDC正常运行的全部实现，应当以10Khz或以上的定时器周期中断高频调用以保证性能
 * @available param list: none
 * @return {*}
 */
void MotorHighFrequencyStateMachine(void)
{
    //电流偏置测量计数
    static int32_t currentOffsetCnt = 0;
    //闭环错误计数
    static int32_t errCloseCnt = 0;
    //初始强托方向
    static bool forceDirect = true;

    //估算出来的转速
    uint32_t eSpeed;

    PWMHandler* pwm = &pwmHandler;
    SensorlessHandler* sensor = &sensorlessHandler;
    BLDCControlHandler* bldc = &bldcControlHandler;

    //时间度量
    pwm->PWMTimeCost++; 
    
    BLDC_GetBusCurrent(sensor,pwm);
    BLDC_GetBusVoltage(sensor);

    switch (bldc->status)
    {
        case eMotorCalOffset:
            BLDC_SwitchOnAllLowSides();
            if(currentOffsetCnt++ == 2047)
            {
                BLDC_SwitchOffAllLowSides();
                sensor->busOffsetCurrent = sensor->busCurrent;
                bldc->status = eMotorWaitCMD;
            }
            break;
        case eMotorWaitCMD:
            pwm->PWMTimeCost = 0;
            //命令等待阶段
            break;
        case eMotorReset:
            stopBLDC();
            //复位电机控制
            resetALL();
#ifdef BLDC_CLOSE_PID
            //复位PID
            speedIntegrator = 0;
            currentIntegrator = 0;
            lastCurrentErr = 0;
            lastSpeedErr = 0;         
#endif
            forceDirect = bldc->rotateDirectionHope;
            bldc->status = eMotorStop;
            break;
        case eMotorStop:
            if(MotorZeroCore(sensor))
            {
                errCloseCnt = 0;
            }
            else
            {
                if(++errCloseCnt > 100)
                {
                    errCloseCnt = 0;
                    pwm->PWMTimeCost = 0;
                    startBLDC();
                    bldc->status = eMotorPreAligned;
                }
            }
            break;
        case eMotorPreAligned:
            MotorForceAlignment(bldc,pwm);
            bldc->status = eMotorPreAlignedDelay;
            break;
        case eMotorPreAlignedDelay:
            if(bldc->alignmentTimeCostCnt++ >= bldc->alignmentTimeCost)
            {
                bldc->alignmentTimeCostCnt = 0;
                bldc->status = eMotorOpenLoop;
            }
            //检测转子的扇区变化情况（同时也能判断出是否发生了堵转或者转速过低的异常）
            if(MotorZeroCore(sensor))
            {
                if(pwm->PWMTimeCost != 0)
                {
                    //开始计算电机在电角度下的转速
                    eSpeed = hardwareUnsignedDiv(BLDC_SPEED_FACTOR,pwm->PWMTimeCost);
                    FirstOrderLPF(sensor->eSpeed,sensor->eSpeed,eSpeed,BLDC_SPEED_LPF_ARG_A,BLDC_SPEED_LPF_ARG_B);
                }
                //获取延迟换相需要的时间点
                delayCommutation = pwm->PWMTimeCost / 2;

                //多转一会儿
                if(sensor->numOfTurns >= BLDC_RUN_TO_CLOSE)
                {
                    /* 切入闭环的必须条件：
                    *   1. sensor获取到的转动的圈数达到一定数目
                    *   2. 开环转向必须和期望的转向一致：否则重启
                    */
                    if(bldc->rotateDirectionHope != sensor->rotateDirection)
                    {
                        sensor->numOfTurns = 0;
                        forceDirect = !forceDirect;
                        bldc->status = eMotorReset;
                        break;
                    }
                    else
                    {
                        sensor->sectorCnt = 0;
                        bldc->status = eMotorCloseAlignedDelay;
                    }
                    errCloseCnt = 0;
                }
                pwm->PWMTimeCost = 0;
            }
            else
            {
                //未发生换相操作就判断换相时间间隔是否太大           
                if(errCloseCnt++ > 10000u)
                {
                    //如果进入到这里说明发生了堵转或者转速过低的异常
                    sensor->eSpeed = 0;
                    errCloseCnt = 0;
                    bldc->status = eMotorReset;
                }
            }
            break;
        case eMotorOpenLoop:
            //变频同时变占空比
            MotorForcePWMDutyChange(bldc,pwm);
            //强迫换相
            MotorForceCommutation(forceDirect,pwm);
            bldc->status = eMotorPreAlignedDelay;
            break;
        case eMotorCloseAlignedDelay:
            if(++delayCommutationCnt >= delayCommutation)
            {
                errCloseCnt = 0;
                delayCommutationCnt = BLDC_RUN_PHASE_DELAY;
                MotorCommutation(sensor,pwm);
                bldc->status = eMotorCloseSpeedLoop;
            }
            break;
        case eMotorCloseSpeedLoop:
            if(MotorZeroCore(sensor) && (sensor->rotateDirection == bldc->rotateDirectionHope))
            {
                if(pwm->PWMTimeCost != 0)
                {
                    eSpeed = hardwareUnsignedDiv(BLDC_SPEED_FACTOR,pwm->PWMTimeCost);
                }
                delayCommutation = pwm->PWMTimeCost / 2; 
                FirstOrderLPF(sensor->eSpeed,sensor->eSpeed,eSpeed,BLDC_SPEED_LPF_ARG_A,BLDC_SPEED_LPF_ARG_B);
                //电流环控制
                MotorCurrentCloseLoopControl(BLDC_RUN_MAX_CURRENT,sensor->busCurrent,pwm);
                //闭环速度控制
                MotorSpeedCloseLoopControl(bldc->eleSpeedHope,sensor->eSpeed,pwm);
                pwm->PWMDuty = (pwm->PWMCurrentDuty > pwm->PWMSpeedDuty) ? (pwm->PWMSpeedDuty) : (pwm->PWMCurrentDuty);
                pwm->PWMTimeCost = 0;
                bldc->status = eMotorCloseAlignedDelay;
            }
            else
            {
                //没有扇区变化的命令时先判断是否发生了异常
                if(errCloseCnt++ == BLDC_RUN_ERR_MAX)
                {
                    errCloseCnt = 0;
                    bldc->status = eMotorReset;
                }
            }
            if(sensor->sectorError)
            {
                errCloseCnt = 0;
                bldc->status = eMotorReset;
            }
            break;
        default:
            while(1);
    }
}
/**
 * @description: 强制电机占空比按照一定顺序逐渐增加，并减少每次预定位延时
 * @param {BLDCControlHandler*} bldc
 * @param {PWMHandler*} pwm
 * @available param list: none
 * @return {*}
 */
void MotorForcePWMDutyChange(BLDCControlHandler* bldc,PWMHandler* pwm)
{
    int16_t timeCost = bldc->alignmentTimeCost;
    uint16_t duty = pwm->PWMDuty;
    
    timeCost = timeCost - BLDC_PRE_ALIGNED_TIME / 32;
    //限制最大换相频率,值越小，换相频率越高
    if(timeCost < BLDC_PRE_ALIGNED_TIME / 8)
    {
        timeCost = BLDC_PRE_ALIGNED_TIME / 8;
    }
    bldc->alignmentTimeCost = timeCost;
    duty = duty + BLDC_PWM_START_STEP_DUTY;
    //限制能够达到的最大换相占空比
    if(duty > BLDC_PWM_START_MAX_DUTY)
    {
        duty = BLDC_PWM_START_MAX_DUTY;
    }

    pwm->PWMCurrentDuty = duty;
    pwm->PWMSpeedDuty = duty;
    pwm->PWMDuty = duty;
}
/**
 * @description: 电机强制换相函数
 * @param {bool*} forceDirect
 * @param {PWMHandler*} pwm
 * @available param list: none
 * @return {*}
 */
void MotorForceCommutation(bool forceDirect,PWMHandler* pwm)
{
    if(forceDirect)
    {
        (*MotorForwardTable[pwm->rotateSector++])(pwm);
    }
    else
    {
        (*MotorReverseTable[pwm->rotateSector--])(pwm);
    }

    if(pwm->rotateSector > 5)
        pwm->rotateSector = 0;
    else if(pwm->rotateSector < 0)
        pwm->rotateSector = 5;
}
/**
 * @description: BLDC强拖预定位函数
 * @param {BLDCControlHandler*} bldc
 * @param {PWMHandler*} pwm
 * @available param list: none
 * @return {*}
 */
void MotorForceAlignment(BLDCControlHandler* bldc,PWMHandler* pwm)
{
    bldc->alignmentTimeCost = BLDC_PRE_ALIGNED_TIME;
    bldc->alignmentTimeCostCnt = 0;
    //强制预定位，U相上桥PWM开通，V相下桥常开
    BLDC_UH_VL(pwm);
}

