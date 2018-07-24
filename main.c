/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "stm8s_it.h"    /* SDCC patch: required by SDCC for interrupts */
#include "main.h"
#include "SSD1306.h"

uint16_t welding = 0;
uint16_t welding_timer = 0;
uint16_t current_weding_time = 0;
uint16_t delay = 0;
uint16_t delay_timer = 0;

void main(void)
{
  CLK_DeInit();

  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);

  GPIO_Config();

  TIM1_Config();
  TIM2_Config();

  LCD_Init();
  LCD_Time_Init();
  LCD_Update();

  /* Initialize the Interrupt sensitivity */
  EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);
  EXTI_SetTLISensitivity(EXTI_TLISENSITIVITY_FALL_ONLY);

  enableInterrupts();

  uint16_t encoder_time = 0;
  while (1)
  {
    Delay(0x1FFF);
    encoder_time = TIM1_GetCounter()/4+1;
    LCD_Set_Time(encoder_time);
    current_weding_time = encoder_time*10;
    LCD_Update();
  }
}

static void GPIO_Config(void)
{
  GPIO_Init(GPIOC, ((GPIO_Pin_TypeDef)(GPIO_PIN_6 | GPIO_PIN_7)), GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)(GPIO_PIN_1), GPIO_MODE_IN_FL_IT);
  GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)(GPIO_PIN_6), GPIO_MODE_OUT_PP_LOW_FAST);
}

static void TIM2_Config(void)
{
  TIM2_TimeBaseInit(TIM2_PRESCALER_128, ARR_Val);

  TIM2_PrescalerConfig(TIM2_PRESCALER_128, TIM2_PSCRELOADMODE_IMMEDIATE);

  // TIM2_OC1Init(TIM2_OCMODE_INACTIVE, TIM2_OUTPUTSTATE_ENABLE, CCR1_Val, TIM2_OCPOLARITY_HIGH); 
  // TIM2_OC1PreloadConfig(ENABLE);

  TIM2_ARRPreloadConfig(ENABLE);
  
  /* TIM IT enable */
  TIM2_ITConfig(TIM2_IT_UPDATE, ENABLE);
  
  /* TIM2 enable counter */
  TIM2_Cmd(ENABLE);
}

static void TIM1_Config(void)
{
  TIM1_DeInit();
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, 395, 1);
  TIM1_EncoderInterfaceConfig(TIM1_ENCODERMODE_TI12, TIM1_ICPOLARITY_RISING, TIM1_ICPOLARITY_RISING);
  TIM1_Cmd(ENABLE);
}

void Start_Welding(void)
{
  if (welding != 1 && delay != 1) {
    welding = 1;
    delay = 1;
    GPIO_WriteHigh(GPIOD, GPIO_PIN_6); //Start welding
  }
}

void Update_timer(void)
{
  if (welding == 1) {
    welding_timer++;
  }
  else if (delay == 1)
  {
    delay_timer++;
  }
  if (welding_timer >= current_weding_time)
  {
    GPIO_WriteLow(GPIOD, GPIO_PIN_6); //Stop welding
    welding = 0;
    welding_timer = 0;
  }
  if (delay_timer >= Delay_Val)
  {
    delay = 0;
    delay_timer = 0;
  }
}

void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}
