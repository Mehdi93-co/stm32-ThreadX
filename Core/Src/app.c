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
#include "stm32f7xx_hal_def.h"
#include "stm32f7xx_hal_uart.h"
#include "tx_api.h"
#include <stdint.h>
#include <stdio.h>

void ld1_thread_entry(ULONG thread_input)
{
  UINT ret = TX_SUCCESS;
  while (1)
  {
    ret = tx_semaphore_get(&g_ld1_Semaphore, TX_WAIT_FOREVER);
    if (ret != TX_SUCCESS)
    {
      tx_thread_sleep(LED_BLINK_DELAY);
    }
    HAL_GPIO_TogglePin(LED_LD1_GPIO_Port, LED_LD1_Pin);
  }
}

void ld1_timer_callback(ULONG timer_input)
{
  tx_semaphore_put(&g_ld1_Semaphore);
}

/**
 * @brief  Callback function for UART interrupts
 * @param  huart: pointer to UART handle
 * @retval None
 */
void App_UART1_TransmitCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
  }
}
