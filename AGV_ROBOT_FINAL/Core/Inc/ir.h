/*
 * ir.h
 *
 *  Created on: May 20, 2022
 *      Author: Lenovo
 */

#ifndef INC_IR_H_
#define INC_IR_H_
#include "stm32f4xx_hal.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim);
void check_ir1(char *p,int len);
void check_ir2(char *p,int len);
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void check_ir_signal(void);


#endif /* INC_IR_H_ */
