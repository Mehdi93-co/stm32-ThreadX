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

#ifndef __DEFINES_H
#define __DEFINES_H

#if defined(__cplusplus)
extern "C"
{
#endif /* defined(__cplusplus) */

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
#define TRUE  1
#define FALSE 0

#define ON  1
#define OFF 0

#define HIGH 1
#define LOW  0

#define LED_BLINK_DELAY                   10
#define UART1_TX_PERIODIC_INTERRUPT_COUNT 10

/* Thread Define -------------------------------------------------------------*/
#define THREAD_STACK_SIZE  1024
#define TRACEX_BUFFER_SIZE (32 * 1024) /* 32 KB TraceX buffer size */

/* Exported types ------------------------------------------------------------*/
/* USER BUTTON State */
typedef enum
{
  RELEASED = 0,
  PRESSED  = 1
} BTN_State_t;

/* Compile Mode for Exercise */
#define SEMAPHORE_MODE       FALSE
#define LD1_BLINK_MODE_DELAY FALSE
#define LD1_BLINK_MODE_TICK  FALSE
#define LD1_BTN_MODE         FALSE
#define UART_MODE            TRUE
// define LD1_SLEEP_MODE       FALSE

/* Exported functions -------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* defined(__cplusplus) */
#endif /* __DEFINES_H */
