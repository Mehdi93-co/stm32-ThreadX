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

static uint8_t   thread_1_stack[THREAD_STACK_SIZE];
static uint8_t   thread_2_stack[THREAD_STACK_SIZE];
static uint8_t   thread_Uart1_stack[THREAD_STACK_SIZE];
static TX_THREAD thread_1_ptr;
static TX_THREAD thread_2_ptr;
static TX_THREAD thread_Uart1_ptr;

TX_SEMAPHORE g_uart1_done_Semaphore;

TX_QUEUE     g_thread1_queue;
TX_QUEUE     g_thread2_queue;
ULONG        g_queue1_msg[MSG_MAX_IN_FLIGHT];
ULONG        g_queue2_msg[MSG_MAX_IN_FLIGHT];
TX_BYTE_POOL g_msg_pool;
static char  g_uart1_msg[MSG_POOL_SIZE];

TX_EVENT_FLAGS_GROUP g_uart1_event_group;

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
  /* Create a byte memory pool from which to allocate the thread stacks.  */
  ret = tx_byte_pool_create(&g_msg_pool, "App_ThreadX_Memory", g_uart1_msg, MSG_POOL_SIZE);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }
  /* Trace Buffer ---------------------------------------------------------*/
  tx_trace_enable(&trace_buffer, TRACEX_BUFFER_SIZE, 30);

  /* event flags group ----------------------------------------------------*/
  ret = tx_event_flags_create(&g_uart1_event_group, "UART1 Event Group");
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Semaphore ------------------------------------------------------------*/
  ret = tx_semaphore_create(&g_uart1_done_Semaphore, "UART1 Done Semaphore", 0);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* Queue ------------------------------------------------------------*/
  ret = tx_queue_create(&g_thread1_queue, "Thread 1 Queue", 1, g_queue1_msg, sizeof(g_queue1_msg));
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  ret = tx_queue_create(&g_thread2_queue, "Thread 2 Queue", 1, g_queue2_msg, sizeof(g_queue2_msg));
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* thread 1 --------------------------------------------------------*/
  ret = tx_thread_create(&thread_1_ptr, "Thread 1", thread_1_entry, 0, thread_1_stack,
                         THREAD_STACK_SIZE, UART_THREAD1_PRIORITY, UART_THREAD1_PRIAMPTION,
                         TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  /* thread 2 --------------------------------------------------------*/
  ret = tx_thread_create(&thread_2_ptr, "Thread 2", thread_2_entry, 0, thread_2_stack,
                         THREAD_STACK_SIZE, UART_THREAD2_PRIORITY, UART_THREAD2_PRIAMPTION,
                         TX_NO_TIME_SLICE, TX_AUTO_START);
  if (ret != TX_SUCCESS)
  {
    Error_Handler();
  }

  /*Thread_Uart1 -----------------------------------------------------*/
  ret = tx_thread_create(&thread_Uart1_ptr, "Thread UART1", thread_Uart1_entry, 0,
                         thread_Uart1_stack, THREAD_STACK_SIZE, UART_THREAD_PRIORITY,
                         UART_THREAD_PRIAMPTION, TX_NO_TIME_SLICE, TX_AUTO_START);
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
