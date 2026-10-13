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
#include "stm32f7xx_hal_uart.h"
#include "tx_api.h"
#include <stdint.h>

static uint8_t s_counter_interrupts = 0;

static const uint8_t tx_data[]       = "Hello from UART1\r\n";
static const uint8_t tx_data_large[] = "This is a 40 long Sting just to callaps the UART\r\n";

/**
 * @brief  Application code for LD1 Blink
 * @retval None
 */
VOID thread_Led_entry(ULONG thread_input)
{
  while (1)
  {
    App_Blink_Process();
  }
}

/**
 * @brief  Application code for User Button with Debounce
 * @retval None
 */
VOID thread_Btn_entry(ULONG thread_input)
{
  while (TRUE)
  {
    tx_semaphore_get(&g_user_btn_Semaphore, TX_WAIT_FOREVER);
    App_BTN_Process();
  }
}

/**
 * @brief  Application code for UART1
 * @retval None
 */
VOID thread_Uart1_entry(ULONG thread_input)
{
  while (TRUE)
  {
#if (SEMAPHORE_MODE)
    tx_semaphore_get(&g_uart1_Semaphore, TX_WAIT_FOREVER);
#else
    tx_mutex_get(&g_uart1_Mutex, TX_WAIT_FOREVER);
#endif
    App_UART1_Process();
  }
}

/**
 * @brief  Application code for blinking LED LD1
 * @retval None
 */
void App_Blink_Process(void)
{
  HAL_GPIO_TogglePin(GPIOI, LED_LD1_Pin);
#if (LD1_SLEEP_MODE)
  tx_thread_sleep(LED_BLINK_DELAY);
#endif
#if (SEMAPHORE_MODE)
  tx_semaphore_get(&g_user_ld1_Semaphore, TX_WAIT_FOREVER);
#else
  tx_mutex_get(&g_user_ld1_Mutex, TX_WAIT_FOREVER);
#endif
}

/**
 * @brief  Application code for LD1 and BTN
 * @retval None
 */
void App_BTN_Process(void)
{
  HAL_GPIO_TogglePin(LED_LD1_GPIO_Port, LED_LD1_Pin);
}

/**
 * @brief  Application code for UART1
 * @retval None
 */
void App_UART1_Process(void)
{
  // HAL_UART_Transmit(&huart1, tx_data, sizeof(tx_data) - 1, HAL_MAX_DELAY);
  // HAL_UART_Transmit(&huart1, tx_data_large, sizeof(tx_data_large) - 1, HAL_MAX_DELAY);
  HAL_UART_Transmit_IT(&huart1, tx_data_large, sizeof(tx_data_large) - 1);
}

void App_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1)
  {
    tx_mutex_put(&g_uart1_Mutex);
  }
}

/**
 * @brief  Callback function for EXTI interrupts
 * @param  htim: Specifies the TIM handle
 * @retval None
 */
void App_UART1_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM10)
  {
    s_counter_interrupts++;
    if (s_counter_interrupts >= UART1_TX_PERIODIC_INTERRUPT_COUNT)
    {
      s_counter_interrupts = 0;
#if (SEMAPHORE_MODE)
      // tx_semaphore_put(&g_uart1_Semaphore);
      //}
      tx_semaphore_put(&g_user_ld1_Semaphore);
#else
      tx_mutex_put(&g_uart1_Mutex);
#endif
    }
  }
}

/**
 * @brief  Callback function for EXTI interrupts
 * @param  htim: Specifies the TIM handle
 * @retval None
 */
void App_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM7)
  {
    HAL_TIM_Base_Stop_IT(&htim7);
    if (HAL_GPIO_ReadPin(USER_BTN_GPIO_Port, USER_BTN_Pin) == GPIO_PIN_SET)
    {
      tx_semaphore_put(&g_user_btn_Semaphore);
    }

    __HAL_GPIO_EXTI_CLEAR_FLAG(USER_BTN_Pin);
    EXTI->IMR |= USER_BTN_Pin;
  }
}

#if (LD1_BTN_MODE)
/* GPIO Interrupts */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == USER_BTN_Pin)
  {
    EXTI->IMR &= ~USER_BTN_Pin;
    __HAL_TIM_SET_COUNTER(&htim7, 0);
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
    HAL_TIM_Base_Start_IT(&htim7);
  }
}

#endif // LD1_BTN FALSE
