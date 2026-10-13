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

static uint8_t s_counter_interrupts = 0;

static const uint8_t tx_data[]       = "Hello from UART1\r\n";
static const uint8_t tx_data_large[] = "This is a 40 long Sting just to callaps the UART\r\n";

/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_Uart1_entry(ULONG thread_input)
{
  while (TRUE)
  {
    tx_semaphore_get(&g_uart1_Semaphore, TX_WAIT_FOREVER);
    App_UART1_Process();
  }
}

/**
 * @brief  Application code for LD1 and BTN
 * @retval None
 */
void App_BTN_Process(void)
{
  HAL_GPIO_TogglePin(LED_LD1_GPIO_Port, LED_LD1_Pin);
}

/**
 * @brief  Application code for UART1
 * @retval None
 */
void App_UART1_Process(void)
{
#if (TX_LARGE_BUFFER)
  HAL_UART_Transmit(&huart1, tx_data_large, sizeof(tx_data_large) - 1, HAL_MAX_DELAY);
#else
  HAL_UART_Transmit(&huart1, tx_data, sizeof(tx_data) - 1, HAL_MAX_DELAY);
#endif
}

/**
 * @brief  Callback function for EXTI interrupts
 * @param  htim: Specifies the TIM handle
 * @retval None
 */
void App_UART1_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM10)
  {
    s_counter_interrupts++;
    if (s_counter_interrupts >= UART1_TX_PERIODIC_INTERRUPT_COUNT)
    {
      s_counter_interrupts = 0;
      tx_semaphore_put(&g_uart1_Semaphore);
    }
    // tx_semaphore_put(&g_user_ld1_Semaphore);
  }
}
