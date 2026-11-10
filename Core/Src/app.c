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

static uint8_t  s_counter_interrupts = 0;
static uint32_t s_tx_semaphore_count = 0;

static const uint8_t tx_data_speedy[] = "Hello from UART1 ( speedy )\r\n";
static const uint8_t tx_data_slow[]   = "Hello from UART1 ( slow )\r\n";
/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_speedy_Uart1_entry(ULONG thread_input)
{
  while (TRUE)
  {
    App_UART1_speedy_Process();
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
    App_UART1_slow_Process();
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

/**
 * @brief  Application code for UART1 speedy
 * @retval None
 */
void App_UART1_speedy_Process(void)
{
  HAL_StatusTypeDef uart1_state = HAL_OK;
  uart1_state = HAL_UART_Transmit_IT(&huart1, tx_data_speedy, sizeof(tx_data_speedy) - 1);
  if (uart1_state != HAL_OK)
  {
    /* An error occurred during the transmission, handle the error */
    Error_Handler();
  }
}

/**
 * @brief  Application code for UART1 slow
 * @retval None
 */
void App_UART1_slow_Process(void)
{
  HAL_StatusTypeDef uart1_state = HAL_OK;
  uart1_state = HAL_UART_Transmit_IT(&huart1, tx_data_slow, sizeof(tx_data_slow) - 1);
  if (uart1_state != HAL_OK)
  {
    /* An error occurred during the transmission, handle the error */
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
  }
}
