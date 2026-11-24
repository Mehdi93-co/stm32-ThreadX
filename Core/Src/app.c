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

/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_1_entry(ULONG thread_input)
{
  static uint32_t s_counter_thread1 = 0;
  char           *s_thread1_msg;
  UINT            s_status;
  while (TRUE)
  {
    tx_thread_sleep(THREAD1_SLEEP_TICKS);
    s_counter_thread1++;
    s_status =
        tx_byte_allocate(&g_msg_pool, (VOID **)&s_thread1_msg, MSG_BLOCK_SIZE, TX_WAIT_FOREVER);
    if (s_status != TX_SUCCESS)
    {
      Error_Handler();
    }
    snprintf(s_thread1_msg, MSG_BLOCK_SIZE,
             "Thread 1: executed %lu times, current system time: %lu\r\n",
             (unsigned long)s_counter_thread1, (unsigned long)tx_time_get());

    ;
    tx_queue_send(&g_thread1_queue, &s_thread1_msg, TX_WAIT_FOREVER);
    s_status = tx_event_flags_set(&g_uart1_event_group, THREAD1_FLAGS, TX_OR);
    if (s_status != TX_SUCCESS)
    {
      Error_Handler();
    }
  }
}

/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_2_entry(ULONG thread_input)
{
  static uint32_t s_counter_thread2 = 0;
  char           *s_thread2_msg;
  UINT            s_status;
  while (TRUE)
  {
    tx_thread_sleep(THREAD2_SLEEP_TICKS);
    s_counter_thread2++;
    s_status =
        tx_byte_allocate(&g_msg_pool, (VOID **)&s_thread2_msg, MSG_BLOCK_SIZE, TX_WAIT_FOREVER);
    if (s_status != TX_SUCCESS)
    {
      Error_Handler();
    }
    snprintf(s_thread2_msg, MSG_BLOCK_SIZE,
             "Thread 2: executed %lu times, current system time: %lu\r\n",
             (unsigned long)s_counter_thread2, (unsigned long)tx_time_get());

    ;
    tx_queue_send(&g_thread2_queue, &s_thread2_msg, TX_WAIT_FOREVER);
    s_status = tx_event_flags_set(&g_uart1_event_group, THREAD2_FLAGS, TX_OR);
    if (s_status != TX_SUCCESS)
    {
      Error_Handler();
    }
  }
}

/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_Uart1_entry(ULONG thread_input)
{
  static ULONG s_actual_event_flags;
  while (TRUE)
  {
    if (tx_event_flags_get(&g_uart1_event_group, THREADS_FLAG_MASK, TX_OR_CLEAR,
                           &s_actual_event_flags, TX_WAIT_FOREVER) != TX_SUCCESS)
    {
      Error_Handler();
    }
    if (s_actual_event_flags & THREAD1_FLAGS)
    {
      uart_drain_queue(&g_thread1_queue);
    }
    if (s_actual_event_flags & THREAD2_FLAGS)
    {
      uart_drain_queue(&g_thread2_queue);
    }
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

void uart_drain_queue(TX_QUEUE *queue)
{
  char *s_uart1_message;
  {
    while (tx_queue_receive(queue, &s_uart1_message, TX_NO_WAIT) == TX_SUCCESS)
      ;
    App_UART1_Transmit((char *)s_uart1_message, strlen((char *)s_uart1_message));
    tx_byte_release(s_uart1_message);
  }
}
