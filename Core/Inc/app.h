/**
 ******************************************************************************
 * @file    app.h
 * @brief   Application interface: process functions and timer callback
 * @author  Mehdi Elkorch
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 Mehdi Elkorch.
 * All rights reserved.
 *
 ******************************************************************************
 */

#ifndef __APP_H
#define __APP_H

#if defined(__cplusplus)
extern "C"
{
#endif /* defined(__cplusplus) */

/* Define to prevent recursive inclusion -------------------------------------*/
#include "tx_api.h"
#include "main.h"
#include "app_threadx.h"
#include "defines.h"
#include <stdint.h>

extern TIM_HandleTypeDef  htim7;
extern TIM_HandleTypeDef  htim10;
extern UART_HandleTypeDef huart1;

extern TX_SEMAPHORE g_uart1_done_Semaphore;
extern TX_MUTEX     g_message_mutex;
extern TX_MUTEX     g_sleep_mutex;
extern TX_QUEUE     g_thread_queue;
extern TX_BYTE_POOL g_msg_pool;

typedef enum
{
  THREAD1_SLEEP_TICKS = 80,
  THREAD2_SLEEP_TICKS = 120,
} THREAD_SLEEP_TICKS_t;

/* Threads Priamption and Priority--------------------------------------------*/
typedef enum
{
  UART_THREAD1_PRIORITY   = 5,
  UART_THREAD1_PRIAMPTION = 5
} UART_THREAD1_PRIO_t;

typedef enum
{
  UART_THREAD2_PRIORITY   = 15,
  UART_THREAD2_PRIAMPTION = 15
} UART_THREAD2_PRIO_t;

typedef enum
{
  UART_THREAD_PRIORITY   = 16,
  UART_THREAD_PRIAMPTION = 16
} UART_THREAD_PRIO_t;

typedef enum
{
  THREAD1_ID = 1,
  THREAD2_ID = 2,
} THREAD_ID_t;

/* Threads entry function ----- ----------------------------------------------*/
VOID thread_1_entry(ULONG thread_input);
VOID thread_2_entry(ULONG thread_input);
VOID thread_Uart1_entry(ULONG thread_input);

/* Exported functions ------------------------------------------------------- */
void App_UART1_Transmit(char *data, uint16_t size);

/* Callback function for EXTI interrupts ------------------------------------ */
void App_UART1_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void App_UART1_TransmitCpltCallback(UART_HandleTypeDef *huart);
#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */
#endif /* __APP_H */
