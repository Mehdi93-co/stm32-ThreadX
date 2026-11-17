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

/* Threads Priamption and Priority--------------------------------------------*/
typedef enum
{
  UART_THREAD_SPEEDY_PRIORITY   = 5,
  UART_THREAD_SPEEDY_PRIAMPTION = 5
} UART_THREAD_SPEEDY_PRIO_t;

typedef enum
{
  UART_THREAD_SLOW_PRIORITY   = 15,
  UART_THREAD_SLOW_PRIAMPTION = 15
} UART_THREAD_SLOW_PRIO_t;

typedef enum
{
  UART_THREAD_PRIORITY   = 16,
  UART_THREAD_PRIAMPTION = 16
} UART_THREAD_PRIO_t;

typedef enum
{
  THREAD_ID_SPEEDY = 0,
  THREAD_ID_SLOW   = 1,
} THREAD_ID_t;

typedef struct
{
  uint32_t thread_id;
  uint32_t ticks;
} thread_data_t;

#define THREAD_DATA_SIZE (sizeof(thread_data_t) / sizeof(ULONG))
#define UART_QUEUE_DEPTH 5

/* Threads entry function ----- ----------------------------------------------*/
VOID thread_speedy_Uart1_entry(ULONG thread_input);
VOID thread_slow_Uart1_entry(ULONG thread_input);
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
