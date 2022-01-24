/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/main.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    30-September-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"

/** @addtogroup STM8L10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* define the GPIO port and pins connected to Leds mounted on STM8L101-EVAL board */


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t flag1 = 0;
uint8_t flag2 = 0;
uint8_t flag3 = 0;
uint8_t flag4 = 0;

uint32_t timertick;

uint8_t buttonstate1;
uint8_t buttoncounter1;
uint8_t buttonstate2;
uint8_t buttoncounter2;
uint8_t buttonstate3;
uint8_t buttoncounter3;
uint8_t buttonstate4;
uint8_t buttoncounter4;

uint8_t READSTATE1, READSTATE2, READSTATE3, READSTATE4;

uint32_t start_add1 = 0x9000;
uint32_t start_add2 = 0x9001;
uint32_t start_add3 = 0x9002;
uint32_t start_add4 = 0x9003;

uint8_t add = 0x01;


/* Private function prototypes -----------------------------------------------*/
void Delay (uint16_t nCount);
void outputs(void);
void buttoncount1(void);
void buttoncount2(void);
void buttoncount3(void);
void buttoncount4(void);

void flashwrite1(void);
void flashwrite2(void);
void flashwrite3(void);
void flashwrite4(void);

static void TIM4_Config(void);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
void main(void)
{

  /* GPIO INPUT CONFIG */
    GPIO_Init(GPIOC, GPIO_Pin_4, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIOA, GPIO_Pin_2, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIOA, GPIO_Pin_3, GPIO_Mode_In_PU_No_IT);
  /* GPIO OUTPUT CONFIG */
    GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOB, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOB, GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Fast);
  /*FLASH CONFIG*/
    FLASH_DeInit();
    
  //Button Input Assingments 
    buttonstate1 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
    buttonstate2 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    buttonstate3 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
    buttonstate4 = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3);
    
    TIM4_Config();
    
  while (1)
  {
    
    buttoncount1();
    buttoncount2();
    buttoncount3();
    buttoncount4();
    READSTATE1 = FLASH_ReadByte(start_add1);
    READSTATE2 = FLASH_ReadByte(start_add2);
    READSTATE3 = FLASH_ReadByte(start_add3);
    READSTATE4 = FLASH_ReadByte(start_add4);
    outputs();
    
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
/* Flash writing functions */
void flashwrite1(void)
{
    FLASH_Unlock(FLASH_MemType_Program);
    FLASH_EraseByte(start_add1);
    FLASH_ProgramByte(start_add1, buttoncounter1);
    FLASH_Lock(FLASH_MemType_Program);
        
}
void flashwrite2(void)
{
    FLASH_Unlock(FLASH_MemType_Program);
    FLASH_EraseByte(start_add2);
    FLASH_ProgramByte(start_add2, buttoncounter2);
    FLASH_Lock(FLASH_MemType_Program);
}
void flashwrite3(void)
{
    FLASH_Unlock(FLASH_MemType_Program);
    FLASH_EraseByte(start_add3);
    FLASH_ProgramByte(start_add3, buttoncounter3);
    FLASH_Lock(FLASH_MemType_Program);
}
void flashwrite4(void)
{
    FLASH_Unlock(FLASH_MemType_Program);
    FLASH_EraseByte(start_add4);
    FLASH_ProgramByte(start_add4, buttoncounter4);
    FLASH_Lock(FLASH_MemType_Program);
}
/* Output controls with buttons after reading state "1 or 0" from flash memory */
void outputs(void)
{
  if(READSTATE1 == 1)            //     INP1 -> PC4     OUTP1 -> PB1
    {
      
      flag1 = 1;
      GPIO_SetBits(GPIOB,GPIO_Pin_1);   //Output occurs
      
    }
    else
    {
      flag1 = 0;
      GPIO_ResetBits(GPIOB,GPIO_Pin_1);
    }
    if(READSTATE2 == 1)             //      INP2 -> PA0     OUTP2 -> PB2
    {
      flag2 = 1;
      GPIO_SetBits(GPIOB, GPIO_Pin_2);    //Output occurs
    }
    else
    {
      flag2 = 0;
      GPIO_ResetBits(GPIOB,GPIO_Pin_2);
    }
    
    if(READSTATE3 == 1)             //      INP3 -> PA2     OUTP3 -> PB3
    {
      flag3 = 1;
      GPIO_SetBits(GPIOB, GPIO_Pin_3);     //Output occurs
    }
    
    else
    {
      flag3 = 0;
      GPIO_ResetBits(GPIOB,GPIO_Pin_3);
    }
    
    if(READSTATE4 == 1)             //      INP4 -> PA3     OUTP4 -> PB4
    {
      flag4 = 1;
      GPIO_SetBits(GPIOB, GPIO_Pin_4);  //Output occurs
     
    }
    else
    {
      flag4 = 0;
      GPIO_ResetBits(GPIOB,GPIO_Pin_4);
    }
}
/* Output states via buttons */
void buttoncount1(void)
{
  if(buttonstate1 == 1 && buttoncounter1 == 0)
  {
    buttoncounter1 = 1;
    flashwrite1();
  }
  else if (buttonstate1 == 1 && buttoncounter1 == 1)
  {
    buttoncounter1 = 0 ;
    flashwrite1();
  }
}
void buttoncount2(void)
{
  if(buttonstate2 == 1 && buttoncounter2 == 0)
  {
    buttoncounter2= 1;
    flashwrite2();
  }
  else if (buttonstate2 == 1 && buttoncounter2 == 1)
  {
    buttoncounter2 = 0 ;
    flashwrite2();
  }
}
void buttoncount3(void)
{
  if(buttonstate3 == 1 && buttoncounter3 == 0)
  {
    buttoncounter3= 1;
    flashwrite3();
  }
  else if (buttonstate3 == 1 && buttoncounter3 == 1)
  {
    buttoncounter3 = 0 ;
    flashwrite3();
  }
}

void buttoncount4(void)
{
  if(buttonstate4 == 1 && buttoncounter4 == 0)
  {
    buttoncounter4= 1;
    flashwrite4();
  }
  else if (buttonstate4 == 1 && buttoncounter4 == 1)
  {
    buttoncounter4 = 0 ;
    flashwrite4();
  }
}

static void TIM4_Config(void)
{
  /* Enable TIM4 CLK */
  CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
  
  TIM4_DeInit();
  
  /* Time base configuration */ 
  TIM4_TimeBaseInit(TIM4_Prescaler_128, 124);
  TIM4_ITConfig(TIM4_IT_Update, ENABLE);
  
  enableInterrupts();

  /* Initialize I/Os in Output Mode */
  GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);

  /* Enable TIM4 */
  TIM4_Cmd(ENABLE);
}


/* Library Delay Function */

void Delay(__IO uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {
    nCount--;
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {}
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
