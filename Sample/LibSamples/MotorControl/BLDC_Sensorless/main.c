/***********************************************************************************************************************
    @file    main.c
    @author  FAE Team
    @date    15-Mar-2023
    @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
  **********************************************************************************************************************
    @attention

    <h2><center>&copy; Copyright(c) <2023> <MindMotion></center></h2>

      Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
    following conditions are met:
    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
       the following disclaimer in the documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or
       promote products derived from this software without specific prior written permission.

      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *********************************************************************************************************************/

/* Define to prevent recursive inclusion */
#define _MAIN_C_

/* Files include */
#include "platform.h"
#include "gpio_led_toggle.h"
#include "main.h"

#include "mcConfig.h"
#include "mcCMD.h"
/**
  * @addtogroup MM32SPIN0230_LibSamples
  * @{
  */

/**
  * @addtogroup GPIO
  * @{
  */

/**
  * @addtogroup GPIO_LED_Toggle
  * @{
  */

/* Private typedef ****************************************************************************************************/

/* Private define *****************************************************************************************************/

/* Private macro ******************************************************************************************************/

/* Private variables **************************************************************************************************/

/* Private functions **************************************************************************************************/

/***********************************************************************************************************************
  * @brief  This function is main entrance
  * @note   main
  * @param  none
  * @retval none
  *********************************************************************************************************************/
int main(void)
{
  PLATFORM_Init();

  GPIO_Configure();
	
  initBLDC();
  BLDC_CMD_SetDirection(false);
  BLDC_CMD_WaitExc();

  int8_t cnt0 = 0;
  int8_t cnt1 = 0;
  int8_t cnt2 = 0;
  bool sysFlag = false;
	
  uint16_t speed = 16900u;

  while(1)
  {
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3) == RESET)
    {
      if(cnt0++ > 50u)
      {
        cnt0 = 0;
        GPIO_IO_Toggle(GPIOB, GPIO_Pin_1);
        GPIO_IO_Toggle(GPIOB, GPIO_Pin_0);
        GPIO_IO_Toggle(GPIOA, GPIO_Pin_7);
        GPIO_IO_Toggle(GPIOA, GPIO_Pin_6);
        BLDC_Delay(500);
        sysFlag = !sysFlag;
        if(sysFlag)
        {
          BLDC_CMD_SetSpeed(speed);
          BLDC_CMD_WaitExc();
          BLDC_CMD_Start();
          BLDC_CMD_WaitExc();
        }
        else
        {
          BLDC_CMD_Stop();
        }
      }
    }
    else
    {
      if(cnt0 > 0u)
      {
        cnt0--;
      }
    }

    if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == RESET)
    {
      if(cnt1++ > 50u)
      {
        cnt1 = 0;
        speed += 200u;
        if(speed > 17000u)
        {
          speed = 5000u;
        }
        BLDC_CMD_SetSpeed(speed);
        BLDC_Delay(50);
      }
    }
    else
    {
      if(cnt1 > 0u)
      {
        cnt1--;
      }
    }

    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_2) == RESET)
    {
      if(cnt2++ > 50u)
      {
        cnt1 = 0;
        BLDC_CMD_Reverse();
        BLDC_Delay(1500);
      }
    }
    else
    {
      if(cnt2 > 0u)
      {
        cnt2--;
      }
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/********************************************** (C) Copyright MindMotion **********************************************/

