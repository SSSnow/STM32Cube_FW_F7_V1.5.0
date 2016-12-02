/**
  ******************************************************************************
  * @file    BSP/Src/audio_loopback.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    23-September-2016  
  * @brief   This example code shows how to use the audio feature in the
  *          stm32f769i_discovery driver
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
#include <stdio.h>
#include "string.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
typedef enum
{
  BUFFER_OFFSET_NONE = 0,
  BUFFER_OFFSET_HALF = 1,
  BUFFER_OFFSET_FULL = 2,
}BUFFER_StateTypeDef;

#define AUDIO_BLOCK_SIZE   ((uint32_t)512)
#define AUDIO_BUFFER_IN    AUDIO_REC_START_ADDR     /* In SDRAM */
#define AUDIO_BUFFER_OUT   (AUDIO_REC_START_ADDR + (AUDIO_BLOCK_SIZE * 2)) /* In SDRAM */

#define SCRATCH_BUFF_SIZE  1024

#if defined ( __ICCARM__ ) // !< IAR Compiler
#pragma location=0x2000E000
int32_t Scratch [SCRATCH_BUFF_SIZE];

#elif defined ( __CC_ARM ) //!< Keil Compiler
int32_t Scratch [SCRATCH_BUFF_SIZE] __attribute__((at(0x2000E000)));

#elif defined ( __GNUC__ ) // !< GNU Compiler
int32_t Scratch [SCRATCH_BUFF_SIZE] __attribute__((section(".scratch_section")));
#endif
 
#define RECORD_BUFFER_SIZE  4096

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t  audio_rec_buffer_state;
uint8_t ChannelNumber = 0;
/* Buffer containing the PCM samples coming from the microphone */
int16_t RecordBuffer[RECORD_BUFFER_SIZE];

/* Buffer used to stream the recorded PCM samples towards the audio codec. */
int16_t PlaybackBuffer[RECORD_BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void AudioLoopback_SetHint(void);
static void CopyBuffer(int16_t *pbuffer1, int16_t *pbuffer2, uint16_t BufferSize);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Audio Play demo
  * @param  None
  * @retval None
  */
void AudioLoopback_demo (void)
{
  uint32_t audio_loop_back_init = RESET ; 
  
  AudioLoopback_SetHint();

  /* Initialize Audio Recorder with 4 channels to be used */
  if (BSP_AUDIO_IN_Init(BSP_AUDIO_FREQUENCY_44K, DEFAULT_AUDIO_IN_BIT_RESOLUTION, 2*DEFAULT_AUDIO_IN_CHANNEL_NBR) == AUDIO_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT OK  ", CENTER_MODE);
  }
  else
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 95, (uint8_t *)"  AUDIO RECORD INIT FAIL", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)" Try to reset board ", CENTER_MODE);
  }

  /* Display the state on the screen */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 80, (uint8_t *)"Microphones sound streamed to headphones", CENTER_MODE);

  /* Allocate scratch buffers */
    BSP_AUDIO_IN_AllocScratch (Scratch, SCRATCH_BUFF_SIZE);
    
  /* Start Recording */
  BSP_AUDIO_IN_Record((uint16_t*)&RecordBuffer[0], RECORD_BUFFER_SIZE);

  ChannelNumber = BSP_AUDIO_IN_GetChannelNumber();
  audio_rec_buffer_state = BUFFER_OFFSET_NONE; 
  
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);  
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAt(0, 130, (uint8_t*)"RECORD/PLAYING ONGOING", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 145, (uint8_t*)"at Sample rate 44 Khz", CENTER_MODE);  
  BSP_LCD_DisplayStringAt(0, 175, (uint8_t *)"Press User push-button", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 190, (uint8_t *)"to stop recording", CENTER_MODE);

  while (1)
  {
    /* 1st or 2nd half of the record buffer ready for being copied 
    to the playbakc buffer */
    if(audio_rec_buffer_state != BUFFER_OFFSET_NONE)
    {
      /* Copy half of the record buffer to the playback buffer */
      if(audio_rec_buffer_state == BUFFER_OFFSET_HALF)
      {
        CopyBuffer(&PlaybackBuffer[0], &RecordBuffer[0], RECORD_BUFFER_SIZE/2);
        
        if (audio_loop_back_init == RESET)
        {
          /* Initialize the audio device*/ 
          if (BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_HEADPHONE, 
                                 70, 
                                 BSP_AUDIO_FREQUENCY_44K) != AUDIO_OK)
          {
            Error_Handler();
          }
          if(ChannelNumber > 2)
          {
            BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_0123);
          }
          else
          {
            BSP_AUDIO_OUT_SetAudioFrameSlot(CODEC_AUDIOFRAME_SLOT_02);
          }
          /* Play the recorded buffer */
          BSP_AUDIO_OUT_Play((uint16_t *) &PlaybackBuffer[0], 2*RECORD_BUFFER_SIZE);
          
          /* Audio device is initialized only once */
          audio_loop_back_init = SET; 
        }
      }
      else /* if(audio_rec_buffer_state == BUFFER_OFFSET_FULL)*/
      {
        CopyBuffer(&PlaybackBuffer[RECORD_BUFFER_SIZE/2], &RecordBuffer[RECORD_BUFFER_SIZE/2], RECORD_BUFFER_SIZE/2);
        
      }
      
      /* Wait for next data */
      audio_rec_buffer_state = BUFFER_OFFSET_NONE;  
    }
    
    if (CheckForUserInput() > 0)
    {
      /* Stop Player before close Test */
      BSP_AUDIO_OUT_Stop(CODEC_PDWN_HW);
      BSP_AUDIO_OUT_DeInit();
      BSP_AUDIO_IN_Stop();
      BSP_AUDIO_IN_DeInit(); 
      return;
    }
  }
}

/**
  * @brief  Copy content of pbuffer2 to pbuffer1
  * @param  None
  * @retval None
  */
static void CopyBuffer(int16_t *pbuffer1, int16_t *pbuffer2, uint16_t BufferSize)
{
  uint32_t i = 0;
  for(i = 0; i < BufferSize; i++)
  {
    pbuffer1[i] = pbuffer2[i];
  }
}

/**
  * @brief  Display Audio Record demo hint
  * @param  None
  * @retval None
  */
static void AudioLoopback_SetHint(void)
{
  /* Clear the LCD */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set Audio Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 90);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"AUDIO LOOPBACK EXAMPLE", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Press User button for next menu", CENTER_MODE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DrawRect(10, 100, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 110);
  BSP_LCD_DrawRect(11, 101, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 112);

}

/**
  * @brief Manages the DMA Transfer complete interrupt.
  * @param None
  * @retval None
  */
void BSP_AUDIO_IN_TransferComplete_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_FULL;
}

/**
  * @brief  Manages the DMA Half Transfer complete interrupt.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_HalfTransfer_CallBack(void)
{
  audio_rec_buffer_state = BUFFER_OFFSET_HALF;
}

/**
  * @brief  Audio IN Error callback function.
  * @param  None
  * @retval None
  */
void BSP_AUDIO_IN_Error_CallBack(void)
{
  /* This function is called when an Interrupt due to transfer error on or peripheral
     error occurs. */
  /* Display message on the LCD screen */
  BSP_LCD_SetBackColor(LCD_COLOR_RED);
  BSP_LCD_DisplayStringAt(0, LINE(14), (uint8_t *)"       DMA  ERROR     ", CENTER_MODE);

  /* Stop the program with an infinite loop */
  while (BSP_PB_GetState(BUTTON_WAKEUP) != RESET)
  {
    return;
  }
  /* could also generate a system reset to recover from the error */
  /* .... */
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
