/**
  ******************************************************************************
  * @file    GPIO_Toggle\main.c
  * @author  MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief   This file contains the main function for GPIO Toggle example.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_it.h"    /* SDCC patch: required by SDCC for interrupts */
#include "main.h"
#include "SSD1306.h"

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{
  GPIO_Config();


  TIM1_DeInit();
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 1000, 1);
  TIM1_EncoderInterfaceConfig(TIM1_ENCODERMODE_TI12, TIM1_ICPOLARITY_RISING, TIM1_ICPOLARITY_RISING);
  TIM1_Cmd(ENABLE);

  TIM2_Config();

  LCD_Init();
  LCD_Update();
  // enableInterrupts();


  while (1)
  {
    Delay(0x0FFF);
    TIM2_SetCompare1(TIM1_GetCounter());
  }

}

static void GPIO_Config(void)
{
  /* Set pin PG.5 & PG.6 & PG.7 to high state */
  GPIO_Init(GPIOC, ((GPIO_Pin_TypeDef)(GPIO_PIN_6 | GPIO_PIN_7)), GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)(GPIO_PIN_2|GPIO_PIN_4), GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_WriteLow(GPIOD, (GPIO_Pin_TypeDef)(GPIO_PIN_2|GPIO_PIN_4));
}

/**
  * @brief  Configure Output Compare Active Mode for TIM2 Channel1, Channel2 and 
  *         channel3  
  * @param  None
  * @retval None
  */
static void TIM2_Config(void)
{
  TIM2_TimeBaseInit(TIM2_PRESCALER_512, 1020);

  TIM2_PrescalerConfig(TIM2_PRESCALER_512, TIM2_PSCRELOADMODE_IMMEDIATE);

  TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, CCR1_Val, TIM2_OCPOLARITY_HIGH); 
  TIM2_OC1PreloadConfig(ENABLE);


  TIM2_OC2Init(TIM2_OCMODE_ACTIVE, TIM2_OUTPUTSTATE_ENABLE, CCR2_Val, TIM2_OCPOLARITY_HIGH); 
  TIM2_OC2PreloadConfig(ENABLE);

  TIM2_ARRPreloadConfig(ENABLE);
  
  /* TIM IT enable */
  TIM2_ITConfig(TIM2_IT_CC1, ENABLE);
  TIM2_ITConfig(TIM2_IT_CC2, ENABLE);
  
  /* TIM2 enable counter */
  TIM2_Cmd(ENABLE);
}

void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
