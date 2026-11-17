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

static uint32_t s_counter_speedy_ticks = 0;
static uint32_t s_counter_slow_ticks   = 0;

/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_speedy_Uart1_entry(ULONG thread_input)
{
  static thread_data_t s_thread_data;
  while (TRUE)
  {
    s_counter_speedy_ticks = tx_time_get();
    tx_thread_sleep(2);
    if (tx_mutex_get(&g_sleep_mutex, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      tx_thread_sleep(5);
      tx_mutex_put(&g_sleep_mutex);
    }
    tx_thread_sleep(4);
    if (tx_mutex_get(&g_sleep_mutex, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      tx_thread_sleep(3);
      tx_mutex_put(&g_sleep_mutex);
    }
    s_thread_data.thread_id = THREAD_ID_SPEEDY;
    s_thread_data.ticks     = tx_time_get() - s_counter_speedy_ticks;
    tx_queue_send(&g_thread_queue, &s_thread_data, TX_WAIT_FOREVER);
  }
}

/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_slow_Uart1_entry(ULONG thread_input)
{
  static thread_data_t s_thread_data;
  while (TRUE)
  {
    s_counter_slow_ticks = tx_time_get();
    if (tx_mutex_get(&g_sleep_mutex, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      tx_thread_sleep(12);
      tx_mutex_put(&g_sleep_mutex);
    }
    tx_thread_sleep(8);
    if (tx_mutex_get(&g_sleep_mutex, TX_WAIT_FOREVER) == TX_SUCCESS)
    {
      tx_thread_sleep(11);
      tx_mutex_put(&g_sleep_mutex);
    }
    tx_thread_sleep(9);

    s_thread_data.thread_id = THREAD_ID_SLOW;
    s_thread_data.ticks     = tx_time_get() - s_counter_slow_ticks;
    tx_queue_send(&g_thread_queue, &s_thread_data, TX_WAIT_FOREVER);
  }
}

/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_Uart1_entry(ULONG thread_input)
{
  static char s_uart1_message[100];
  while (TRUE)
  {
    static thread_data_t s_thread_data;
    tx_queue_receive(&g_thread_queue, &s_thread_data, TX_WAIT_FOREVER);
    switch (s_thread_data.thread_id)
    {
      case THREAD_ID_SPEEDY:
        snprintf(s_uart1_message, sizeof(s_uart1_message), "Thread SPEEDY[ Ticks: %lu]\r\n",
                 (unsigned long)s_thread_data.ticks);
        break;
      case THREAD_ID_SLOW:
        snprintf(s_uart1_message, sizeof(s_uart1_message), "Thread SLOW[ Ticks: %lu]\r\n",
                 (unsigned long)s_thread_data.ticks);
        break;
      default:
        snprintf(s_uart1_message, sizeof(s_uart1_message), "Unknown Thread ID[ %lu Ticks: %lu]\r\n",
                 (unsigned long)s_thread_data.thread_id, (unsigned long)s_thread_data.ticks);
        break;
    }
    App_UART1_Transmit((char *)s_uart1_message, strlen((char *)s_uart1_message));
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

void App_UART1_Transmit(char *data, uint16_t size)
{
  if (HAL_UART_Transmit_IT(&huart1, (uint8_t *)data, size) == HAL_OK)
  {
    tx_semaphore_get(&g_uart1_done_Semaphore, TX_WAIT_FOREVER);
  }
  else
  {
    Error_Handler();
  }
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
    tx_semaphore_put(&g_uart1_done_Semaphore);
  }
}
