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

static uint8_t   thread_producer_stack[THREAD_STACK_SIZE];
static uint8_t   thread_consumer1_stack[THREAD_STACK_SIZE];
static uint8_t   thread_consumer2_stack[THREAD_STACK_SIZE];
static uint8_t   thread_consumer3_stack[THREAD_STACK_SIZE];
static TX_THREAD thread_producer_ptr;
static TX_THREAD thread_consumer1_ptr;
static TX_THREAD thread_consumer2_ptr;
static TX_THREAD thread_consumer3_ptr;

static thread_data_t g_thread_queue_data_consumer1[CONSUMER1_QUEUE_DEPTH];
static thread_data_t g_thread_queue_data_consumer2[CONSUMER2_QUEUE_DEPTH];
static thread_data_t g_thread_queue_data_consumer3[CONSUMER3_QUEUE_DEPTH];

TX_QUEUE g_thread_queue_consumer1;
TX_QUEUE g_thread_queue_consumer2;
TX_QUEUE g_thread_queue_consumer3;

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
  UINT          ret       = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL *)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  //(void)byte_pool;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* USER CODE BEGIN App_ThreadX_Init */
  /* Trace Buffer ---------------------------------------------------------*/
  tx_trace_enable(&trace_buffer, TRACEX_BUFFER_SIZE, 30);

  /* Queue ------------------------------------------------------------*/

  ret = tx_queue_create(&g_thread_queue_consumer1, "Thread Queue Consumer1", THREAD_DATA_SIZE,
                        g_thread_queue_data_consumer1, sizeof(g_thread_queue_data_consumer1));
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  ret = tx_queue_create(&g_thread_queue_consumer2, "Thread Queue Consumer2", THREAD_DATA_SIZE,
                        g_thread_queue_data_consumer2, sizeof(g_thread_queue_data_consumer2));
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  ret = tx_queue_create(&g_thread_queue_consumer3, "Thread Queue Consumer3", THREAD_DATA_SIZE,
                        g_thread_queue_data_consumer3, sizeof(g_thread_queue_data_consumer3));
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Threads -----------------------------------------------------------*/
  ret = tx_thread_create(&thread_producer_ptr, "Thread Producer", thread_producer_entry, 0,
                         thread_producer_stack, THREAD_STACK_SIZE, PRODUCER_THREAD_PRIORITY,
                         PRODUCER_THREAD_PRIAMPTION, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }
  ret = tx_thread_create(&thread_consumer1_ptr, "Thread Consumer1", thread_consumer1_entry, 0,
                         thread_consumer1_stack, THREAD_STACK_SIZE, CONSUMER1_THREAD_PRIORITY,
                         CONSUMER1_THREAD_PRIAMPTION, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }
  ret = tx_thread_create(&thread_consumer2_ptr, "Thread Consumer2", thread_consumer2_entry, 0,
                         thread_consumer2_stack, THREAD_STACK_SIZE, CONSUMER2_THREAD_PRIORITY,
                         CONSUMER2_THREAD_PRIAMPTION, TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }
  ret = tx_thread_create(&thread_consumer3_ptr, "Thread Consumer3", thread_consumer3_entry, 0,
                         thread_consumer3_stack, THREAD_STACK_SIZE, CONSUMER3_THREAD_PRIORITY,
                         CONSUMER3_THREAD_PRIAMPTION, TX_NO_TIME_SLICE, TX_AUTO_START);
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
