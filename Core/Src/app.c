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

static uint8_t  s_counter_interrupts = 0;
static uint32_t s_tx_semaphore_count = 0;

#if (TX_LARGE_BUFFER)
static const uint8_t tx_data_large[] = "This is a long String sending via UART with interrupts\r\n";
#else
static const uint8_t tx_data[] = "Hello from UART1\r\n";
#endif
/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_Led_entry(ULONG thread_input)
{
  while (TRUE)
  {
    tx_semaphore_get(&g_user_ld1_Semaphore, TX_WAIT_FOREVER);
    App_Blink_Process();
  }
}
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
void App_Blink_Process(void)
{
  HAL_GPIO_TogglePin(LED_LD1_GPIO_Port, LED_LD1_Pin);
}

/**
 * @brief  Application code for UART1
 * @retval None
 */
void App_UART1_Process(void)
{
  HAL_StatusTypeDef uart1_state = HAL_OK;
#if (TX_LARGE_BUFFER)
  uart1_state = HAL_UART_Transmit_IT(&huart1, tx_data_large, sizeof(tx_data_large) - 1);
#else
  uart1_state = HAL_UART_Transmit_IT(&huart1, tx_data, sizeof(tx_data) - 1);
#endif
  if (uart1_state == HAL_BUSY)
  {
    /* UART1 is busy, handle the error */
  }
  else if (uart1_state != HAL_OK)
  {
    /* An error occurred during the transmission, handle the error */
    Error_Handler();
  }
  tx_semaphore_get(&g_uart1_done_Semaphore, TX_WAIT_FOREVER);
}

void App_UART1_TransmitCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    tx_semaphore_put(&g_uart1_done_Semaphore);
  }
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
    tx_semaphore_put(&g_user_ld1_Semaphore);
    s_counter_interrupts++;
    if (s_counter_interrupts >= UART1_TX_PERIODIC_INTERRUPT_COUNT)
    {
      s_counter_interrupts = 0;
      tx_semaphore_put(&g_uart1_Semaphore);
    }
  }
}
