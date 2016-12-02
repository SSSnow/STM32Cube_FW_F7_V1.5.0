/**
  ******************************************************************************
  * @file    BSP/Src/stm32f7xx_it.c
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    23-September-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_it.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
extern SAI_HandleTypeDef haudio_out_sai;
extern DFSDM_Filter_HandleTypeDef       haudio_in_dfsdm_leftfilter;
extern DFSDM_Filter_HandleTypeDef       haudio_in_dfsdm_rightfilter;
extern DMA2D_HandleTypeDef hdma2d_eval;
extern LTDC_HandleTypeDef  hltdc_eval;
extern DSI_HandleTypeDef hdsi_eval;
extern SDRAM_HandleTypeDef sdramHandle;
extern SRAM_HandleTypeDef sramHandle;
extern SD_HandleTypeDef uSdHandle;
extern SD_HandleTypeDef uSdHandle2;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern uint8_t DemoIndex;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M7 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
  Toggle_Leds();
  /* Check periodically the buffer state and fill played buffer with new data
     following the state that has been updated by the BSP_AUDIO_OUT_TransferComplete_CallBack()
     and BSP_AUDIO_OUT_HalfTransfer_CallBack() */
  AUDIO_Process();  
}

/******************************************************************************/
/*                 STM32F7xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f7xx.s).                                               */
/******************************************************************************/
/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(MFX_IRQOUT_PIN);
}

/**
  * @brief  This function handles External lines 15 to 10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(TAMPER_BUTTON_PIN);
}


/**
  * @brief  This function handles SAI DMA interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream6_IRQHandler(void)
{
  if(SdmmcTest == 1)
  {
    HAL_DMA_IRQHandler(uSdHandle.hdmatx); 
  }
  else
  {
    HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
  }
}


/**
  * @brief  This function handles DMA2D Handler.
  * @param  None
  * @retval None
  */
void BSP_LCD_DMA2D_IRQHandler(void)
{
  HAL_DMA2D_IRQHandler(&hdma2d_eval);
}

/**
  * @brief  This function handles DFSDM Left DMAinterrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream0_IRQHandler(void)
{
  if(DemoSdramIndex == 1)
  {
  HAL_DMA_IRQHandler(sdramHandle.hdma); 
  }
  else if(SdmmcTest == 1)
  {
    HAL_DMA_IRQHandler(uSdHandle2.hdmarx); 
  }  
  else
  {
  HAL_DMA_IRQHandler(haudio_in_dfsdm_leftfilter.hdmaReg);
  }
}

/**
  * @brief  This function handles DFSDM Right DMAinterrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream5_IRQHandler(void)
{
  if(SdmmcTest == 1)
  {
    HAL_DMA_IRQHandler(uSdHandle2.hdmatx);
  }
  else
  {
    HAL_DMA_IRQHandler(haudio_in_dfsdm_rightfilter.hdmaReg);  
  } 
}

/**
  * @brief  Handles SDMMC1 DMA Rx transfer interrupt request.
  * @retval None
  */
void BSP_SDMMC1_DMA_Rx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(uSdHandle.hdmarx); 
}

/**
  * @brief  Handles SD1 card interrupt request.
  * @retval None
  */
void BSP_SDMMC1_IRQHandler(void)
{
  HAL_SD_IRQHandler(&uSdHandle);
}

/**
  * @brief  Handles SD2 card interrupt request.
  * @retval None
  */
void BSP_SDMMC2_IRQHandler(void)
{
  HAL_SD_IRQHandler(&uSdHandle2);
}

/**
  * @brief  Handles DSI interrupt request.
  * @note : Can be surcharged by application code implementation of the function.
  */
void BSP_LCD_DSI_IRQHandler(void)
{
  HAL_DSI_IRQHandler(&(hdsi_eval));
}

/**
  * @brief  Handles LTDC interrupt request.
  * @note : Can be surcharged by application code implementation of the function.
  */
void BSP_LCD_LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&(hltdc_eval));
}

/**
  * @brief  This function handles LTDC Error interrupt Handler.
  * @note : Can be surcharged by application code implementation of the function.
  */
void BSP_LCD_LTDC_ER_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&(hltdc_eval));
}

/**
  * @brief  Handles SRAM DMA transfer interrupt request.
  * @retval None
  */
void BSP_SRAM_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(sramHandle.hdma); 
}
   
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
