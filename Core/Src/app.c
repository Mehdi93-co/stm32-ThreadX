/**
 ******************************************************************************
 * @file    app.c
 * @brief   Application logic: LED blink, user button with debounce
 * @author  Mehdi Elkorch
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 Mehdi Elkorch.
 * SPDX-License-Identifier: MIT
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app.h"
#include "stm32f746xx.h"
#include "tx_api.h"
#include <stdint.h>

/**
 * @brief  Application code for User Button with Debounce
 * @retval None
 */
VOID thread_Btn_entry(ULONG thread_input)
{
  while (TRUE)
  {
    tx_semaphore_get(&g_user_btn_Semaphore, TX_WAIT_FOREVER);
    App_Blink_Process();
  }
}

/**
 * @brief  Application code for blinking LED LD1
 * @retval None
 */
void App_Blink_Process(void)
{
  HAL_GPIO_TogglePin(GPIOI, LED_LD1_Pin);
}

/**
 * @brief  Callback function for EXTI interrupts
 * @param  htim: Specifies the TIM handle
 * @retval None
 */
void App_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM7)
  {
    HAL_TIM_Base_Stop_IT(&htim7);
    if (HAL_GPIO_ReadPin(USER_BTN_GPIO_Port, USER_BTN_Pin) == GPIO_PIN_SET)
    {
      tx_semaphore_put(&g_user_btn_Semaphore);
    }

    __HAL_GPIO_EXTI_CLEAR_FLAG(USER_BTN_Pin);
    EXTI->IMR |= USER_BTN_Pin;
  }
}

/**
 * @brief Callback function EXTI interrupts
 * @param GPIO_Pin: Specifies the GPIO handle
 * @retval None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == USER_BTN_Pin)
  {
    EXTI->IMR &= ~USER_BTN_Pin;
    __HAL_TIM_SET_COUNTER(&htim7, 0);
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim7);
  }
}
