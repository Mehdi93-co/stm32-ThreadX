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

static uint8_t  s_counter_thread_speedy_activity = 0;
static uint8_t  s_counter_thread_slow_activity   = 0;
static uint32_t s_counter_speedy_ticks           = 0;
static uint32_t s_counter_slow_ticks             = 0;

static uint8_t tx_ticks_speedy[50];
static uint8_t tx_ticks_slow[50];

/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_speedy_Uart1_entry(ULONG thread_input)
{
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

    snprintf((char *)tx_ticks_speedy, sizeof(tx_ticks_speedy), "Thread [SPEEDY]: %lu ticks\r\n",
             (unsigned long)(tx_time_get() - s_counter_speedy_ticks));
    App_UART1_Transmit((char *)tx_ticks_speedy, strlen((char *)tx_ticks_speedy));
  }
}

/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_slow_Uart1_entry(ULONG thread_input)
{
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

    snprintf((char *)tx_ticks_slow, sizeof(tx_ticks_slow), "Thread [SLOW]: %lu ticks\r\n",
             (unsigned long)(tx_time_get() - s_counter_slow_ticks));
    App_UART1_Transmit((char *)tx_ticks_slow, strlen((char *)tx_ticks_slow));
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
  HAL_StatusTypeDef uart1_state = HAL_OK;

  if (tx_mutex_get(&g_uart1_mutex, TX_WAIT_FOREVER) == TX_SUCCESS)
  {
    if (HAL_UART_Transmit_IT(&huart1, (uint8_t *)data, size) == HAL_OK)
    {
      tx_semaphore_get(&g_uart1_done_Semaphore, TX_WAIT_FOREVER);
    }
    tx_mutex_put(&g_uart1_mutex);
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
