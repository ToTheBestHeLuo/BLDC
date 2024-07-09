/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2023-09-18 09:12:34
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2023-10-10 09:04:46
 * @FilePath: \MDK-ARMd:\灵动\MM32SPIN0230_Lib_Samples\LibSamples_MM32SPIN0230_v0.9.2\Samples\LibSamples\MotorControl\BLDC_SensorlessWithADC\BLDC\mcConfig\mcConfig.c
 * @Description: 
 * 
 * Copyright (c) 2023 by ${git_name_email}, All Rights Reserved. 
 */
#include "../include/mcConfig.h"
/*
*   在下面区域添加需要的头文件.............<
*/

#include "hal_tim.h"
#include "hal_hwdiv.h"
#include "hal_gpio.h"
#include "hal_usart.h"

/*
*   在上面区域添加需要的头文件.............>
*/


/*  在配置相应的外设请先注意-----------------------------------------------------：
*   本文件使用的BLDC驱动方式为：上桥PWM控制，下桥直接GPIO控制，请据此配置相应的IO端口。
*   此外确保：仅当CCR（PWM占空比给定值）大于或等于ARR（PWM周期）时，相应上桥臂为开通信号。
*/

/**
 * @description: 初始化BLDC运行期间需要用到的所有片上外设，主要包括定时器、Systick、PWM对应的GPIO、ADC。
 * @available param list: none
 * @return {*}
 */
void initBLDC(void)
{
    //中断配置
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = ADC_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    NVIC_SetPriority(ADC_IRQn,0);

    //硬件除法器配置
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_HWDIV, ENABLE);
    HWDIV_UsignConfig(HWDIV_UnSigned);

    //HALL接口配置(采用ADC实现)
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    //电流采集IO设置
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    //ShutDown引脚配置
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    //CH1n,CH2n,CH3n通道独立配置
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_6 |GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    // //配置3路ADC采样
    // //使用PA1,PA2,PA3作为 HALL 反电动势采样，PA4,PA5作为电流采样
    ADC_InitTypeDef         ADC_InitStruct;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_ADC1, ENABLE);
    ADC_StructInit(&ADC_InitStruct);
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_10b;
    ADC_InitStruct.ADC_Prescaler  = ADC_Prescaler_2;
    ADC_InitStruct.ADC_Mode       = ADC_Mode_Scan;
    ADC_InitStruct.ADC_DataAlign  = ADC_DataAlign_Left;
    ADC_Init(ADC1, &ADC_InitStruct);
    // 注入通道转换配置
    ADC_SampleTimeConfig(ADC1, ADC_Channel_3, ADC_SampleTime_6_5);
    ADC_SampleTimeConfig(ADC1, ADC_Channel_4, ADC_SampleTime_6_5);
    ADC_InjectedChannelNumCfg(ADC1, 2 - 1);
    ADC_InjectedChannelSelect(ADC1, ADC_InjectedChannel_0, ADC_Channel_3);
    ADC_InjectedChannelSelect(ADC1, ADC_InjectedChannel_1, ADC_Channel_4);
    ADC_InjectedChannelOffsetCfg(ADC1,ADC_InjectedChannel_0,1551);
    ADC_InjectedChannelOffsetCfg(ADC1,ADC_InjectedChannel_1,1551);
    ADC_InjectedExternalTrigSourceConfig(ADC1,ADC_InjectedExtTrig_T1_TRGO,ADC_InjectedExtTrig_Edge_Down,ADC_InjectedExtTrig_Shift_0);
    ADC_InjectedExternalTrigConvCmd(ADC1, ENABLE);
    ADC_InjectedConvCmd(ADC1, ENABLE);
    ADC_ITConfig(ADC1,ADC_IT_INJEOS,ENABLE);
    ADC_Cmd(ADC1,ENABLE);

    //CH1,CH2,CH3通道配置复用IO配置
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_0);  /* TIM1_CH3  */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);  /* TIM1_CH2  */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_0);  /* TIM1_CH1  */
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_High;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
  
    //定时器配置
    TIM_OCInitTypeDef       TIM_OCInitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM1, ENABLE);
	
    TIM_SelectOutputTrigger(TIM1,TIM_TRIGSource_Update);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);
    TIM_TimeBaseStruct.TIM_Prescaler         = 0;
    TIM_TimeBaseStruct.TIM_CounterMode       = TIM_CounterMode_CenterAligned1;
    TIM_TimeBaseStruct.TIM_Period            = 1500 - 1;
    TIM_TimeBaseStruct.TIM_ClockDivision     = TIM_CKD_Div1;
    TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct);

    TIM_ARRPreloadConfig(TIM1, DISABLE);

    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStruct.TIM_Pulse        = 0;
    TIM_OCInitStruct.TIM_OCPolarity   = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_OCNPolarity  = TIM_OCNPolarity_High;
    TIM_OCInitStruct.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OC1Init(TIM1, &TIM_OCInitStruct);

    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OC2Init(TIM1, &TIM_OCInitStruct);

    TIM_OCInitStruct.TIM_Pulse = 0;
    TIM_OC3Init(TIM1, &TIM_OCInitStruct);
		
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);

    // TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM1, ENABLE);
    //
    BLDC_CmdInit();
}
/**
 * @description: 启动BLDC，也就是启动相应PWM输出
 * @available param list: none
 * @return {*}
 */
void startBLDC()
{
    GPIO_SetBits(GPIOA,GPIO_Pin_0);
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Enable);
    TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Enable);
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
}
/**
 * @description: 停止BLDC，也就是关断相应PWM输出
 * @available param list: none
 * @return {*}
 */
void stopBLDC(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_0);
    TIM_CCxCmd(TIM1, TIM_Channel_1, TIM_CCx_Disable);
    TIM_CCxCmd(TIM1, TIM_Channel_2, TIM_CCx_Disable);
    TIM_CCxCmd(TIM1, TIM_Channel_3, TIM_CCx_Disable);
    GPIO_ResetBits(GPIOB,GPIO_Pin_4);
    GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);
    TIM_CtrlPWMOutputs(TIM1, DISABLE);
}
/**
 * @description: 开启所有下管
 * @available param list: none
 * @return {*}
 */
void BLDC_SwitchOnAllLowSides(void)
{
    GPIOB->BSRR = GPIO_Pin_4;
    GPIOB->BSRR = GPIO_Pin_6;
    GPIOB->BSRR = GPIO_Pin_8;  
}
/**
 * @description: 关断所有下管
 * @available param list: none
 * @return {*}
 */
void BLDC_SwitchOffAllLowSides(void)
{
    GPIOB->BRR = GPIO_Pin_4;
    GPIOB->BRR = GPIO_Pin_6;
    GPIOB->BRR = GPIO_Pin_8; 
}
/**
 * @description: 打开U相上桥，V相下桥，并且设置新的占空比，最好直接使用寄存器操作
 * @param {PWMHandler*} pwm，这里需要使用其中PWMDuty作为新的占空比
 * @available param list: none
 * @return {*}
 */
void BLDC_UH_VL(PWMHandler* pwm)
{
    /*参考示例：*/
    // TIM_SetCompare1(TIM1,pwm->PWMDuty);
    // TIM_SetCompare2(TIM1,0);
    // TIM_SetCompare3(TIM1,0);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_4);
    // GPIO_SetBits(GPIOB,GPIO_Pin_6);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_8);

    TIM1->CCR1 = pwm->PWMDuty;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0; 

    GPIOB->BRR = GPIO_Pin_4;
    GPIOB->BSRR = GPIO_Pin_6;
    GPIOB->BRR = GPIO_Pin_8;

    pwm->whichOn = eMOS_V_ON;
}
/**
 * @description: 打开U相上桥，W相下桥，并且设置新的占空比，最好直接使用寄存器操作
 * @param {PWMHandler*} pwm，这里需要使用其中PWMDuty作为新的占空比
 * @available param list: none
 * @return {*}
 */
void BLDC_UH_WL(PWMHandler* pwm)
{
    /*参考示例：*/
    // TIM_SetCompare1(TIM1,pwm->PWMDuty);
    // TIM_SetCompare2(TIM1,0);
    // TIM_SetCompare3(TIM1,0);
    // GPIO_SetBits(GPIOB,GPIO_Pin_4);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_8);

    TIM1->CCR1 = pwm->PWMDuty;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0; 

    GPIOB->BSRR = GPIO_Pin_4;
    GPIOB->BRR = GPIO_Pin_6;
    GPIOB->BRR = GPIO_Pin_8; 

    pwm->whichOn = eMOS_W_ON;
}
/**
 * @description: 打开V相上桥，W相下桥，并且设置新的占空比，最好直接使用寄存器操作
 * @param {PWMHandler*} pwm，这里需要使用其中PWMDuty作为新的占空比
 * @available param list: none
 * @return {*}
 */
void BLDC_VH_WL(PWMHandler* pwm)
{
    /*参考示例：*/
    // TIM_SetCompare1(TIM1,0);
    // TIM_SetCompare2(TIM1,pwm->PWMDuty);
    // TIM_SetCompare3(TIM1,0);
    // GPIO_SetBits(GPIOB,GPIO_Pin_4);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_8);

    TIM1->CCR1 = 0;
    TIM1->CCR2 = pwm->PWMDuty;
    TIM1->CCR3 = 0; 

    GPIOB->BSRR = GPIO_Pin_4;
    GPIOB->BRR = GPIO_Pin_6;
    GPIOB->BRR = GPIO_Pin_8; 

    pwm->whichOn = eMOS_W_ON;
}
/**
 * @description: 打开V相上桥，U相下桥，并且设置新的占空比，最好直接使用寄存器操作
 * @param {PWMHandler*} pwm，这里需要使用其中PWMDuty作为新的占空比
 * @available param list: none
 * @return {*}
 */
void BLDC_VH_UL(PWMHandler* pwm)
{
    /*参考示例：*/
    // TIM_SetCompare1(TIM1,0);
    // TIM_SetCompare2(TIM1,pwm->PWMDuty);
    // TIM_SetCompare3(TIM1,0);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_4);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    // GPIO_SetBits(GPIOB,GPIO_Pin_8);

    TIM1->CCR1 = 0;
    TIM1->CCR2 = pwm->PWMDuty;
    TIM1->CCR3 = 0; 

    GPIOB->BRR = GPIO_Pin_4;
    GPIOB->BRR = GPIO_Pin_6;
    GPIOB->BSRR = GPIO_Pin_8; 

    pwm->whichOn = eMos_U_ON;
}
/**
 * @description: 打开W相上桥，U相下桥，并且设置新的占空比，最好直接使用寄存器操作
 * @param {PWMHandler*} pwm，这里需要使用其中PWMDuty作为新的占空比
 * @available param list: none
 * @return {*}
 */
void BLDC_WH_UL(PWMHandler* pwm)
{
    /*参考示例：*/
    // TIM_SetCompare1(TIM1,0);
    // TIM_SetCompare2(TIM1,0);
    // TIM_SetCompare3(TIM1,pwm->PWMDuty);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_4);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    // GPIO_SetBits(GPIOB,GPIO_Pin_8);

    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = pwm->PWMDuty; 

    GPIOB->BRR = GPIO_Pin_4;
    GPIOB->BRR = GPIO_Pin_6;
    GPIOB->BSRR = GPIO_Pin_8; 

    pwm->whichOn = eMos_U_ON;
}
/**
 * @description: 打开W相上桥，V相下桥，最好直接使用寄存器操作
 * @param {PWMHandler*} pwm，这里需要使用其中PWMDuty作为新的占空比
 * @available param list: none
 * @return {*}
 */
void BLDC_WH_VL(PWMHandler* pwm)
{
    /*参考示例：*/
    // TIM_SetCompare1(TIM1,0);
    // TIM_SetCompare2(TIM1,0);
    // TIM_SetCompare3(TIM1,pwm->PWMDuty);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_4);
    // GPIO_SetBits(GPIOB,GPIO_Pin_6);
    // GPIO_ResetBits(GPIOB,GPIO_Pin_8);

    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = pwm->PWMDuty; 

    GPIOB->BRR = GPIO_Pin_4;
    GPIOB->BSRR = GPIO_Pin_6;
    GPIOB->BRR = GPIO_Pin_8; 

    pwm->whichOn = eMOS_V_ON;
}
/**
 * @description: 当除数不为一个常量或者为2的整数次幂时会调用下面函数以执行无符号32位整数除法，最好直接使用寄存器操作
 * @param {uint32_t} dend 被除数
 * @param {uint32_t} sor 除数
 * @available param list: none
 * @return {uint32_t} 商，不包含余数
 */
uint32_t hardwareUnsignedDiv(uint32_t dend,uint32_t sor)
{
    SET_DVDR(dend);
    SET_DVSR(sor);
    return GET_QUOTR();
}
/**
 * @description: 调试用Debug函数，最好直接使用寄存器操作
 * @param {int8_t} debugDat 需要显示给上位机的debug数据
 * @available param list: none
 * @return {*}
 */
void BLDC_Debug(int8_t debugDat)
{
    USART_SendData(USART1,debugDat);
    while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
}
/**
 * @description: 获取母线电压，注意sensor->busVoltage是uint32_t的类型
 * @param {SensorlessHandler*} sensor
 * @available param list: none
 * @return {*}
 */
void BLDC_GetBusVoltage(SensorlessHandler* sensor)
{

}
/**
 * @description: 获取母线电流，注意sensor->busCurrent是uint32_t的类型，注意需要自己实现相应的滤波处理
 * @param {SensorlessHandler*} sensor
 * @param {PWMHandler*} pwm
 * @available param list: none
 * @return {*}
 */
void BLDC_GetBusCurrent(SensorlessHandler* sensor,PWMHandler* pwm)
{
    static int32_t total = 0;
    static int8_t cnt = 0;

    if(pwm->whichOn == eMos_U_ON)
    {
        int32_t cur = (int16_t)ADC1->JDR0;
        if(cur < 0) 
            cur = -cur;
        
        total += cur;
        if(cnt++ == 31)
        {
            cnt = 0;
            total = total / 32;
            sensor->busCurrent = ((((61) * sensor->busCurrent) + (64 - 61) * cur) / 64);
            total = 0;
        }        
    }
    else if(pwm->whichOn == eMOS_V_ON)
    {
        int32_t cur = (int16_t)ADC1->JDR1;
        if(cur < 0) 
            cur = -cur;
        
        total += cur;
        if(cnt++ == 31)
        {
            cnt = 0;
            total = total / 32;
            sensor->busCurrent = ((((61) * sensor->busCurrent) + (64 - 61) * cur) / 64);
            total = 0;
        }        
    }
}
