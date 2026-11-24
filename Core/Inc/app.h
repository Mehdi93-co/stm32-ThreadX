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

#define MESSAGE_NUM 3

extern TIM_HandleTypeDef  htim7;
extern TIM_HandleTypeDef  htim10;
extern UART_HandleTypeDef huart1;

extern TX_QUEUE g_thread_queue_consumer1;
extern TX_QUEUE g_thread_queue_consumer2;
extern TX_QUEUE g_thread_queue_consumer3;

/* Threads Priamption and Priority--------------------------------------------*/
typedef enum
{
  CONSUMER1_QUEUE_DEPTH = 3,
  CONSUMER2_QUEUE_DEPTH = 3,
  CONSUMER3_QUEUE_DEPTH = 3
} CONSUMER1_QUEUE_DEPTH_t;

typedef enum
{
  PRODUCER_THREAD_PRIORITY   = 4,
  PRODUCER_THREAD_PRIAMPTION = 1
} PRODUCER_THREAD_PRIO_t;

typedef enum
{
  CONSUMER1_THREAD_PRIORITY   = 1,
  CONSUMER1_THREAD_PRIAMPTION = 1
} CONSUMER1_THREAD_PRIO_t;

typedef enum
{
  CONSUMER2_THREAD_PRIORITY   = 2,
  CONSUMER2_THREAD_PRIAMPTION = 2
} CONSUMER2_THREAD_PRIO_t;

typedef enum
{
  CONSUMER3_THREAD_PRIORITY   = 3,
  CONSUMER3_THREAD_PRIAMPTION = 3
} CONSUMER3_THREAD_PRIO_t;

typedef enum
{
  THREAD_ID_SPEEDY = 0,
  THREAD_ID_SLOW   = 1,
} THREAD_ID_t;

typedef struct
{
  uint32_t thread_id;
  uint32_t counter;
} thread_data_t;

#define THREAD_DATA_SIZE (sizeof(thread_data_t) / sizeof(ULONG))
#define UART_QUEUE_DEPTH 5

/* Threads entry function ----- ----------------------------------------------*/
VOID thread_Uart1_entry(ULONG thread_input);

/* threads entry functions for priority management */
VOID thread_producer_entry(ULONG thread_input);
VOID thread_consumer1_entry(ULONG thread_input);
VOID thread_consumer2_entry(ULONG thread_input);
VOID thread_consumer3_entry(ULONG thread_input);

/* Exported functions ------------------------------------------------------- */

/* Callback function for EXTI interrupts ------------------------------------ */
void App_UART1_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */
#endif /* __APP_H */
