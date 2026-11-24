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

static uint32_t s_counter_producer  = 0;
static uint32_t s_counter_consumer1 = 0;
static uint32_t s_counter_consumer2 = 0;
static uint32_t s_counter_consumer3 = 0;

/**
 * @brief  Application code for producer thread
 * @retval None
 */
VOID thread_producer_entry(ULONG thread_input)
{
  while (TRUE)
  {
    s_counter_producer++;
    for (uint32_t i = 0; i < MESSAGE_NUM; i++)
    {
      thread_data_t data;
      data.thread_id = i;
      data.counter   = s_counter_producer;

      /* Send the data to the queue */
      UINT status = tx_queue_send(&g_thread_queue_consumer1, &data, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)
      {
        Error_Handler();
      }
      status = tx_queue_send(&g_thread_queue_consumer2, &data, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)
      {
        Error_Handler();
      }
      status = tx_queue_send(&g_thread_queue_consumer3, &data, TX_WAIT_FOREVER);
      if (status != TX_SUCCESS)
      {
        Error_Handler();
      }
    }
    tx_thread_relinquish();
    tx_trace_user_event_insert(TRACE_EVENT_PRODUCER_CYCLE, s_counter_producer, 0, 0, 0);
  }
}

/**
 * @brief  Application code for consumer thread 1
 * @retval None
 */
VOID thread_consumer1_entry(ULONG thread_input)
{
  thread_data_t s_thread_data;
  while (TRUE)
  {
    s_counter_consumer1++;
    tx_queue_receive(&g_thread_queue_consumer1, &s_thread_data, TX_WAIT_FOREVER);
  }
}

/**
 * @brief  Application code for consumer thread 1
 * @retval None
 */
VOID thread_consumer2_entry(ULONG thread_input)
{
  thread_data_t s_thread_data;
  while (TRUE)
  {
    s_counter_consumer2++;
    tx_queue_receive(&g_thread_queue_consumer2, &s_thread_data, TX_WAIT_FOREVER);
  }
}

/**
 * @brief  Application code for consumer thread 1
 * @retval None
 */
VOID thread_consumer3_entry(ULONG thread_input)
{
  thread_data_t s_thread_data;
  while (TRUE)
  {
    s_counter_consumer3++;
    tx_queue_receive(&g_thread_queue_consumer3, &s_thread_data, TX_WAIT_FOREVER);
  }
}
