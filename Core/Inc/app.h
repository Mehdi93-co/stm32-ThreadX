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

extern TX_SEMAPHORE g_ld1_Semaphore;

typedef enum
{
  RESCHEDULE_TICKS = 10,
  INITIAL_TICKS    = 1
} LD1_TIMER_TICKS_t;

/* Threads Priamption and Priority--------------------------------------------*/
typedef enum
{
  LD1_THREAD1_PRIORITY   = 5,
  LD1_THREAD1_PRIAMPTION = 5
} LD1_THREAD1_PRIO_t;

/* Threads entry function ----- ----------------------------------------------*/
void ld1_thread_entry(ULONG thread_input);

/* Exported functions ------------------------------------------------------- */
void ld1_timer_callback(ULONG timer_input);
/* Callback function for EXTI interrupts ------------------------------------ */
void App_UART1_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void App_UART1_TransmitCpltCallback(UART_HandleTypeDef *huart);
#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */
#endif /* __APP_H */
