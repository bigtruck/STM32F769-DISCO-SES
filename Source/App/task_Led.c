#include <stdio.h>
#include <stdlib.h>

#include "stm32f7xx_hal.h"
#include "bsp.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"





void task_LEDBlink( void *parameter )
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /* Remove compiler warning about unused parameter. */
  ( void ) parameter;
  
  __GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  GPIO_InitStruct.Pin   = GPIO_PIN_13;
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init( GPIOJ, &GPIO_InitStruct );

  for( ;; )
  {
    GPIOJ->ODR ^= GPIO_PIN_13;
    vTaskDelay(500);
  }
}


