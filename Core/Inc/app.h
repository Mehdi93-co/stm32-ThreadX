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

/* Threads entry function ----- ----------------------------------------------*/
VOID thread_speedy_Uart1_entry(ULONG thread_input);
VOID thread_slow_Uart1_entry(ULONG thread_input);

/* Exported functions ------------------------------------------------------- */
void App_UART1_speedy_Process(void);
void App_UART1_slow_Process(void);
void App_UART1_Transmit(char *data, uint16_t size);

/* Callback function for EXTI interrupts ------------------------------------ */
void App_UART1_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void App_UART1_TransmitCpltCallback(UART_HandleTypeDef *huart);
#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */
#endif /* __APP_H */
