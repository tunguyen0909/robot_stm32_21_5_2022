/*
 * mainpp.h
 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */

#ifndef MAINPP_H_
#define MAINPP_H_

#ifdef __cplusplus
 extern "C" {
#endif

#define haiPI 6.283185307
extern volatile int Speed_des1_rps;
extern volatile int Speed_des2_rps;
extern volatile int Output1;
extern volatile int Output2;
extern volatile int rSpeed1;
extern volatile int rSpeed2;
extern volatile int count;
extern volatile int PWM1,PWM2;

volatile double Vx;
volatile double Vy;
volatile double Wz;

volatile double W1;
volatile double W2;

volatile double w1;
volatile double w2;

volatile double vx;
volatile double vy;
volatile double wz;

void setup(void);
void loop(void);
int chuyenChartoInt(char c);
void DC1_thuan(int speed);
void DC1_nghich(int speed);
void DC2_thuan(int speed);
void DC2_nghich(int speed);
void lui();
void tien();
void Xoay_Trai();
void Xoay_Phai();
void dung();
void SystemClock_Config(void);
void convert_vel_linear_2_wheel_to_vel_linear_and_angular(double g_vel_angular_right, double g_vel_angular_left);
void convert_vel_linear_and_angular_to_vel_linear_2_wheel(float Vx, float Wz);

#ifdef __cplusplus
}
#endif


#endif /* MAINPP_H_ */
