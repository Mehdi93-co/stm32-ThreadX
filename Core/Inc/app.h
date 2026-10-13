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

#include "tx_api.h"
#if defined(__cplusplus)
extern "C"
{
#endif /* defined(__cplusplus) */

/* Define to prevent recursive inclusion -------------------------------------*/
#include "main.h"
#include "app_threadx.h"
#include "defines.h"
#include <stdint.h>

extern TIM_HandleTypeDef    htim7;
extern TIM_HandleTypeDef    htim10;
extern UART_HandleTypeDef   huart1;
extern TX_SEMAPHORE         g_user_btn_Semaphore;
extern TX_SEMAPHORE         g_uart1_Semaphore;
extern TX_SEMAPHORE         g_user_ld1_Semaphore;
extern TX_MUTEX             g_uart1_Mutex;
extern volatile BTN_State_t g_user_btn_State;

VOID thread_Led_entry(ULONG thread_input);
VOID thread_Btn_entry(ULONG thread_input);
VOID thread_Uart1_entry(ULONG thread_input);

/* Exported functions ------------------------------------------------------- */
void App_Blink_Process(void);
void App_BTN_Process(void);
void App_UART1_Process(void);

/* Callback function for EXTI interrupts ------------------------------------ */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
void App_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void App_UART1_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */
#endif /* __APP_H */
