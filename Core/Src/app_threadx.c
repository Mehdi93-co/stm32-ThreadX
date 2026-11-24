/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    app_threadx.c
 * @author  MCD Application Team
 * @brief   ThreadX applicative file
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "defines.h"
#include "app.h"
#include "stm32f746xx.h"
#include "stm32f7xx_hal_tim.h"
#include "tx_api.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t trace_buffer[TRACEX_BUFFER_SIZE];

static uint8_t   ld1_thread_stack[THREAD_STACK_SIZE];
static TX_THREAD ld1_thread_ptr;

TX_TIMER g_ld1_timer;

TX_SEMAPHORE g_ld1_Semaphore;

extern TIM_HandleTypeDef htim2;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
 * @brief  Application ThreadX Initialization.
 * @param memory_ptr: memory pointer
 * @retval int
 */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  //(void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  /* Trace Buffer ---------------------------------------------------------*/
  tx_trace_enable(&trace_buffer, TRACEX_BUFFER_SIZE, 30);

  /* Create the semaphore --------------------------------------------------*/
  ret = tx_semaphore_create(&g_ld1_Semaphore, "LD1 Semaphore", 0);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Create the timer -----------------------------------------------------*/
  ret = tx_timer_create(&g_ld1_timer, "LD1 Timer", ld1_timer_callback, 0, INITIAL_TICKS,
                        RESCHEDULE_TICKS, TX_AUTO_ACTIVATE);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }
  /* Create the threads ---------------------------------------------------*/
  ret = tx_thread_create(&ld1_thread_ptr, "LD1 Thread", ld1_thread_entry, 0, ld1_thread_stack,
                         THREAD_STACK_SIZE, LD1_THREAD1_PRIORITY, LD1_THREAD1_PRIAMPTION,
                         TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

/**
 * @brief  MX_ThreadX_Init
 * @param  None
 * @retval None
 */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; /* Trace-Einheit einschalten   */
  DWT->LAR    = 0xC5ACCE55;                       /* DWT entsperren (Cortex-M7)  */
  DWT->CYCCNT = 0;                                /* Zähler auf 0                */
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;            /* Zykluszähler starten        */
  HAL_TIM_Base_Start(&htim2);
  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
