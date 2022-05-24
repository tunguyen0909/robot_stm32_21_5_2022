/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.h
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
 ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define sampling_Time 10
#define inv_sampling_Time 100

 volatile float Kp1 = 5, Ki1 = 0, Kd1 = 1;
 volatile float Kp2 = 5, Ki2 = 0, Kd2 = 1;

 volatile int count = 0;

 volatile int pluse1 = 0;
 volatile int pluse_pre1 = 0;
 volatile int rSpeed1 = 0, Err1 = 0, pre_Err1 = 0;

 volatile float pPart1 = 0, dPart1=0, iPart1 = 0;
 volatile int Output1 = 0;
 volatile int Speed_des1_rps = 0;

 volatile int pluse2 = 0;
 volatile int pluse_pre2 = 0;
 volatile int rSpeed2 = 0, Err2 = 0, pre_Err2 = 0;

 volatile float pPart2 = 0, dPart2=0, iPart2 = 0;
 volatile int Output2 = 0;
 volatile int Speed_des2_rps = 0;


 volatile int PWM1 = 0;
 volatile int PWM2 = 0;

 extern volatile double W1;
 extern volatile double W2;

 extern volatile double Vx;
 extern volatile double Vy;
 extern volatile double Wz;

 extern volatile double w1;
 extern volatile double w2;

 extern volatile double vx;
 extern volatile double vy;
 extern volatile double wz;
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void EXTI1_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);
void DMA1_Stream6_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void TIM1_BRK_TIM9_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void USART1_IRQHandler(void);
void USART2_IRQHandler(void);
void USART3_IRQHandler(void);
void TIM5_IRQHandler(void);
/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
