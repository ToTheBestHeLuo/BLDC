/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-08-30 15:20:56
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-10-09 17:29:14
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_SensorlessWithADC\BLDC\include\mcConfig.h
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef _MC_CONFIG_H
#define _MC_CONFIG_H

#include "mcType.h"
#include "mcCMD.h"

/*本文件包含了所有需要由用户配置的函数以及相应参数的设定*/

/*
*   在下面区域添加需要的头文件.............<
*/

#include "hal_adc.h"
#include "hal_gpio.h"

/*
*   在上面区域添加需要的头文件.............>
*/

/*以下为由宏定义决定的控制方法的设定--------------------------------------------*/
//#define BLDC_METHOD_ADC     /*BLDC过零方案，如果开启宏定义则使能ADC方案，反之使能比较器方案*/
#define BLDC_CLOSE_PID      /*闭环控制策略，如果开启该宏则为PID控制，反之则为PWM控制*/
/*以上为由宏定义决定的控制方法的设定------------------------------------------*/

/*以下为参数的设定--------------------------------------------<*/
#define BLDC_PWM_PERIOD_ARR 1499             /*对应的是实际ARR寄存器的值，也就是定时器中断*/
#define BLDC_PWM_PERIOD 2.5e-5             /*定时器中断周期，单位是s*/
#define BLDC_PWM_START  0.02f            /*对应启动时的PWM占空比，注意这个值需要多调试*/
#define BLDC_PWM_OPEN_STEP     0.005f      /*对应的是BLDC的启动强拖占空比步进值*/
#define BLDC_PWM_OPEN_DUTY_MAX 0.085f      /*对应的是BLDC的启动强拖时的最大占空比*/

#define BLDC_RUN_PWM_MIN 0.05f          /*电机闭环运行时的最小占空比*/
#define BLDC_RUN_PWM_MAX 0.90f          /*电机闭环运行时的最大占空比*/

#define BLDC_PRE_ALIGNED_TIME 300   /*电机强拖预定位执行时间，注意这个值需要多调试*/
#define BLDC_ZERO_CROSS_FILTER_CNT 8  /*电机无感运行时需要滤除比较器尖峰的次数，类似按键消抖，越大越稳定但速度越慢，反之亦然*/
#define BLDC_RUN_TO_CLOSE   7u       /*电机从开环进入闭环时需要转动的圈数，这个值需要多调试*/
#define BLDC_RUN_ERR_MAX    300u      /*极其重要的参数，关乎到系统能否正常运行，这个参数决定了丢步最大容许值，一旦系统达到最大值就会复位！建议刚开始时取比较大的值*/
#define BLDC_RUN_PHASE_DELAY      4u        /*极其重要的参数，关乎到能够产生最大力矩，这个参数是30°换相时的硬件、软件计算等方面造成的延时*/
#define BLDC_RUN_MIN_CURRENT    500u    /*电机正常运行的最小电流*/
#define BLDC_RUN_MAX_CURRENT    5500u   /*电机正常运行的最大电流*/

#define BLDC_SPEED_LPF_ARG_A 56    /*速度低通滤波器时使用的参数1，这个参数必须为整数大于0且比BLDC_SPEED_LPF_ARG_B小*/
#define BLDC_SPEED_LPF_ARG_B 64   /*速度低通滤波器时使用的参数2，这个参数必须是2的整数次幂，例如1,2,4,8..*/

#ifdef BLDC_CLOSE_PID
#define BLDC_PID_I_LIMIT 1050000  /*PID控制器的I的上限*/
#define BLDC_PID_P       17       /*PID控制器的P*/
#define BLDC_PID_P_DIV   512      /*PID控制器的P需要除以的数，这个参数必须是2的整数次幂，例如1,2,4,8..*/
#define BLDC_PID_I       21      /*PID控制器的I*/
#define BLDC_PID_I_DIV   16384      /*PID控制器的I需要除以的数，这个参数必须是2的整数次幂，例如1,2,4,8..*/
#define BLDC_PID_D       7       /*PID控制器的D*/
#define BLDC_PID_D_DIV   256      /*PID控制器的D需要除以的数，这个参数必须是2的整数次幂，例如1,2,4,8..*/
#else
#define BLDC_PWM_GATE   5     /*不采用PID控制器时使用PWM自动调节时的阈值上限*/
#endif
/*以上为参数的设定-------------------------------------------->*/

/*以下为宏函数的设定--------------------------------------------<*/

#ifndef BLDC_METHOD_ADC
#define BLDC_ReadHallLessU() ((((GPIOA->IDR & GPIO_Pin_3)) ? Bit_SET : Bit_RESET)) /*读取U相过零信号，请尽量以寄存器形式操作*/
#define BLDC_ReadHallLessV() ((((GPIOA->IDR & GPIO_Pin_2)) ? Bit_SET : Bit_RESET)) /*读取V相过零信号，请尽量以寄存器形式操作*/
#define BLDC_ReadHallLessW() ((((GPIOA->IDR & GPIO_Pin_1)) ? Bit_SET : Bit_RESET)) /*读取W相过零信号，请尽量以寄存器形式操作*/
#else
#define BLDC_ReadHallLessU() ((int16_t)ADC1->JDR2) /*读取U相反电动势，请尽量以寄存器形式操作*/
#define BLDC_ReadHallLessV() ((int16_t)ADC1->JDR1) /*读取V相反电动势，请尽量以寄存器形式操作*/
#define BLDC_ReadHallLessW() ((int16_t)ADC1->JDR0) /*读取W相反电动势，请尽量以寄存器形式操作*/
#endif

/*以上宏函数的设定-------------------------------------------->*/

/*以下为自动配置的参数，别动*/
#define BLDC_PWM_START_DUTY       ((uint16_t)(BLDC_PWM_PERIOD_ARR * BLDC_PWM_START))
#define BLDC_PWM_START_STEP_DUTY  ((uint16_t)(BLDC_PWM_PERIOD_ARR * BLDC_PWM_OPEN_STEP))
#define BLDC_PWM_START_MIN_DUTY   ((uint16_t)(BLDC_PWM_PERIOD_ARR * BLDC_PWM_OPEN_DUTY_MIN))
#define BLDC_PWM_START_MAX_DUTY   ((uint16_t)(BLDC_PWM_PERIOD_ARR * BLDC_PWM_OPEN_DUTY_MAX))
#define BLDC_PWM_CLOSE_MIN_DUTY   ((uint16_t)(BLDC_PWM_PERIOD_ARR * BLDC_RUN_PWM_MIN))
#define BLDC_PWM_CLOSE_MAX_DUTY   ((uint16_t)(BLDC_PWM_PERIOD_ARR * BLDC_RUN_PWM_MAX))  
#define BLDC_SPEED_FACTOR   ((uint32_t)(10/BLDC_PWM_PERIOD))
/*以上为自动配置的参数，别动*/

/*以下为函数的设定--------------------------------------------<*/
extern void initBLDC(void);
extern void startBLDC(void);
extern void stopBLDC(void);

extern void BLDC_SwitchOnAllLowSides(void);
extern void BLDC_SwitchOffAllLowSides(void);

extern void BLDC_UH_VL(PWMHandler* pwm);
extern void BLDC_UH_WL(PWMHandler* pwm);
extern void BLDC_VH_WL(PWMHandler* pwm);
extern void BLDC_VH_UL(PWMHandler* pwm);
extern void BLDC_WH_UL(PWMHandler* pwm);
extern void BLDC_WH_VL(PWMHandler* pwm);

extern void BLDC_GetBusVoltage(SensorlessHandler* sensor);
extern void BLDC_GetBusCurrent(SensorlessHandler* sensor,PWMHandler* pwm);

extern uint32_t hardwareUnsignedDiv(uint32_t dend,uint32_t sor);

extern void BLDC_Debug(int8_t debugDat);
/*以上为函数的设定-------------------------------------------->*/
#endif

