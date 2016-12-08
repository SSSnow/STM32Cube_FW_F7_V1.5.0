/**
  ******************************************************************************
  * @file    Src/main.c
  * @author  kyChu
  * @version V0.0.1
  * @date    07-December-2016
  * @brief   none
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
# define APP_LOAD_ADDRESS               0x0800C000

# define BOARD_FLASH_SIZE               (2 * 1024 * 1024)
# define BOOTLOADER_RESERVATION_SIZE	(16 * 1024)
# define APP_RESERVATION_SIZE		(2 * 16 * 1024) /* 2 16 Kib Sectors */
# define APP_SIZE_MAX			(BOARD_FLASH_SIZE - (BOOTLOADER_RESERVATION_SIZE + APP_RESERVATION_SIZE))
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Times = 5;
static GPIO_InitTypeDef  GPIO_InitStruct;

/* Private function prototypes -----------------------------------------------*/
void jump_to_app(void);
static void do_jump(uint32_t stacktop, uint32_t entrypoint);

//void SystemClock_Config(void);
static void CPU_CACHE_Enable(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();

  /* STM32F7xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 216 MHz */
//  SystemClock_Config();

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

/* -2- Configure IO in output push-pull mode to drive external LEDs */
  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

  GPIO_InitStruct.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  while(Times --)
  {
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
	HAL_Delay(200);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
	HAL_Delay(200);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
	HAL_Delay(200);
  }

  jump_to_app();

  /* Infinite loop */
  Error_Handler();

  for(;;);
}

void jump_to_app(void)
{
  const uint32_t *app_base = (const uint32_t *)APP_LOAD_ADDRESS;

  /*
   * We refuse to program the first word of the app until the upload is marked
   * complete by the host.  So if it's not 0xffffffff, we should try booting it.
   */
	if (app_base[0] == 0xffffffff) {
		return;
	}

	/*
	 * The second word of the app is the entrypoint; it must point within the
	 * flash area (or we have a bad flash).
	 */
	if (app_base[1] < APP_LOAD_ADDRESS) {
		return;
	}

	if (app_base[1] >= (APP_LOAD_ADDRESS + APP_SIZE_MAX)) {
		return;
	}
	/* switch exception handlers to the application */
//	SCB->VTOR = APP_LOAD_ADDRESS;
	/* extract the stack and entrypoint from the app vector table and go */
	do_jump(app_base[0], app_base[1]);
}
//typedef void (*pFunc)(void);
static void do_jump(uint32_t stacktop, uint32_t entrypoint)
{
//pFunc funct = (pFunc)entrypoint;
	asm volatile(
		"msr msp, %0	\n"
		"bx	%1	\n"
		: : "r"(stacktop), "r"(entrypoint) :);
//funct();
	// just to keep noreturn happy
//	for (;;) ;
Error_Handler();
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 16000000
  *            PLL_M                          = 8
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            PLL_R                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
//void SystemClock_Config(void)
//{
//  RCC_ClkInitTypeDef RCC_ClkInitStruct;
//  RCC_OscInitTypeDef RCC_OscInitStruct;
//  
//  /* Enable HSE Oscillator and activate PLL with HSE as source */
//  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
//  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
//  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//  RCC_OscInitStruct.PLL.PLLM = 16;
//  RCC_OscInitStruct.PLL.PLLN = 432;  
//  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//  RCC_OscInitStruct.PLL.PLLQ = 9;
//  RCC_OscInitStruct.PLL.PLLR = 7;
//  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//  {
//    while(1) {};
//  }
//  
//  /* Activate the OverDrive to reach the 216 Mhz Frequency */
//  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
//  {
//    while(1) {};
//  }
//  
//  
//  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
//     clocks dividers */
//  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
//  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
//  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
//  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
//  {
//    while(1) {};
//  }
//}
/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
while(1)
{
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
//	HAL_Delay(200);
	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
//	HAL_Delay(200);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
//	HAL_Delay(50);
}
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
