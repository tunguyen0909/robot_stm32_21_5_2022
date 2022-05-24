/*
 * ir.c
 *
 *  Created on: May 20, 2022
 *      Author: Lenovo
 */


#include <stdio.h>
#include <stdlib.h>
#include "ir.h"

#define __BELOW_LIMIT_IR_LEFT_T__ 4500
#define __ABOVE_LIMIT_IR_LEFT_T__ 5500

#define __BELOW_LIMIT_IR_RIGHT_T__ 9500
#define __ABOVE_LIMIT_IR_RIGHT_T__ 10500

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim3;

extern double g_vel_angular_right;
extern double g_vel_angular_left;

extern void convert_vel_linear_and_angular_to_vel_linear_2_wheel(double Vx, double Wz);

volatile uint8_t Is_First_Captured_1 = 0;
volatile uint32_t Difference_1 = 0;
volatile uint32_t IC_Val1 = 0;
volatile uint32_t IC_Val2 = 0;
volatile uint32_t i1 = 0;
volatile uint8_t Is_First_Captured_2 = 0;
volatile uint32_t Difference_2 = 0;
volatile uint32_t IC_Val3 = 0;
volatile uint32_t IC_Val4 = 0;
volatile uint32_t i2 = 0;
char data_ir1[100];
char data_ir2[100];

int cnt1 = 0;
int cnt2 = 0;
int cnt3 = 0;
int cnt4 = 0;

uint8_t thong_ke_ir1 = 0;
uint8_t thong_ke_ir2 = 0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){

	if(htim->Instance == htim2.Instance){
		if (Is_First_Captured_1 == 0) // if the first value is not captured
		{
			IC_Val1 = __HAL_TIM_GetCounter(&htim2);
			Is_First_Captured_1 = 1;  // set the first captured as true
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
		}

		else if (Is_First_Captured_1 == 1)   // if the first is already captured
		{
			IC_Val2 = __HAL_TIM_GetCounter(&htim2);
			Difference_1 = IC_Val2-IC_Val1;

			if(Difference_1 > __BELOW_LIMIT_IR_LEFT_T__ && Difference_1 < __ABOVE_LIMIT_IR_LEFT_T__){
				data_ir1[i1++] = 1;
			}else if(Difference_1 >= __BELOW_LIMIT_IR_RIGHT_T__ && Difference_1 <= __ABOVE_LIMIT_IR_RIGHT_T__){
				data_ir1[i1++] = 2;
			}else if(Difference_1 >= 14500 && Difference_1 <= 15500){
				data_ir1[i1++] = 3;
			}

			if(i1 == 10){
				i1 = 0;
				thong_ke_ir1 = 1;
			}


			Is_First_Captured_1 = 0; // set it back to false
			__HAL_TIM_SetCounter(&htim2,0);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim2, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);

		}
	}

	else if(htim->Instance == htim5.Instance){
		if (Is_First_Captured_2 == 0) // if the first value is not captured
		{
			IC_Val3 = __HAL_TIM_GetCounter(&htim5);
			Is_First_Captured_2 = 1;  // set the first captured as true
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
		}
		else if (Is_First_Captured_2 == 1)   // if the first is already captured
		{
			IC_Val4 = __HAL_TIM_GetCounter(&htim5);
			Difference_2 = IC_Val4-IC_Val3;

			if(Difference_2 > __BELOW_LIMIT_IR_LEFT_T__ && Difference_2 < __ABOVE_LIMIT_IR_LEFT_T__){
				data_ir2[i2++] = 1;
			}else if(Difference_2 >= __BELOW_LIMIT_IR_RIGHT_T__ && Difference_2 <= __ABOVE_LIMIT_IR_RIGHT_T__){
				data_ir2[i2++] = 2;
			}

			if(i2 == 10){
				i2 = 0;
				thong_ke_ir2 = 1;
			}


			Is_First_Captured_2 = 0; // set it back to false
			__HAL_TIM_SetCounter(&htim5,0);
			__HAL_TIM_SET_CAPTUREPOLARITY(&htim5, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}
	}

}

void check_ir_signal(void){
	if(thong_ke_ir1 == 1){
		check_ir1(data_ir1, 10);
		thong_ke_ir1 = 0;
	}

}

uint8_t center1 = 0;
uint8_t center2 = 0;
uint8_t left1 = 0;
uint8_t left2 = 0;
uint8_t right1 = 0;
uint8_t right2 = 0;

uint8_t flag = 0;

void check_ir1(char *p,int len){
    for(int i = 0; i<len;i++){
        if(*(p+i) == 1){
            cnt1++;
        }
        else if(*(p+i) == 2){
            cnt2++;
        }
    }

    //printf("%d %d\n",cnt1,cnt2);
    if(cnt3 == 0){
        if(cnt1 > cnt2){
        	g_vel_angular_right = -0.5;
        	g_vel_angular_left = 0.5;
                //printf("left1\n");
        }else if(cnt2 > cnt1){
        	g_vel_angular_right = 0.5;
        	g_vel_angular_left = -0.5;
                //printf("right1\n");
        }else if(cnt1 == cnt2){
        	g_vel_angular_right = 0.5;
        	g_vel_angular_left = 0.5;
        		//printf("center\n");
        }
    }else if(cnt3 != 0){
    	g_vel_angular_right = 0.5;
    	g_vel_angular_left = 0.5;
    }


    cnt1 = 0;
    cnt2 = 0;
    cnt3 = 0;

}

void check_ir2(char *p,int len){
    for(int i = 0; i<len;i++){
        if(*(p+i) == 1){
            cnt3++;
        }
        else if(*(p+i) == 2){
            cnt4++;
        }
    }

    if(cnt3 > cnt4){
        if((cnt3-cnt4) < 5){

            //printf("center2\n");
        }else if((cnt3 - cnt4) > 5){

            //printf("left2\n");
        }
    }else if(cnt4 > cnt3){
        if((cnt4-cnt3) < 5){

            //printf("center2\n");
        }else if((cnt4 - cnt3) > 5){

            //printf("right2\n");
        }
    }

    cnt3 = 0;
    cnt4 = 0;

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance == htim3.Instance){
		if(thong_ke_ir1 == 0){
			g_vel_angular_left = 0;
			g_vel_angular_right = 0;
		}
	}

}


void read_ir(void){

}
