/*
 * main.cpp
 *
 *  Created on: 2018/01/17
 *      Author: yoneken
 */

#include "mainpp.h"
#include <ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <std_msgs/String.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int8.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/MagneticField.h>
#include <sensor_msgs/Imu.h>
#include <ros/time.h>
#include <math.h>
#include <geometry_msgs/Vector3.h>

#define __IR_WITH_ROS__
#define IR_dock_deebot

uint8_t cmd = 0;
char data_rx[10];
char data_tx[10];
uint8_t tx_flag = 0;
uint8_t idx = 0;

#define LENGTH_BETWEEN_2_WHEEL 0.372 	// m unit
#define WHEEL_RADIUS 0.0475				// m unit

double g_vel_angular_left = 0.0;			//r/s unit
double g_vel_angular_right = 0.0;			//r/s unit

int8_t docking_status = 0;

uint8_t pData;
char dataTX[62];
float num[10];
float a;
int i =0;
int j= 0;

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart1;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim6;

extern int vanToc1;
char button1[] = "button_1";
char button2[] = "button_2";
char button3[] = "button_3";
double vxRE,vyRE,wzRE;
int countTick = HAL_GetTick();

void process_data(char data[]);

void messageCb( const geometry_msgs::Twist& cmd_msg);
void messageCb_dock( const std_msgs::Int8& toggle_msg);
void convert_vel_linear_and_angular_to_vel_linear_2_wheel(double Vx, double Wz);
void convert_vel_linear_2_wheel_to_vel_linear_and_angular(double angular_right, double angular_left);
void usDelay(uint32_t uSec);

ros::NodeHandle nh;

//Publisher IMU
sensor_msgs::Imu imu;
//IMU
//ros::Publisher pub_magnetic("imu/mag",&magneticField);
ros::Publisher pub_imu("imu/data", &imu);
//Subscriber and Publisher Velocity
geometry_msgs::Twist raw_vel_msg;
ros::Publisher raw_vel_pub("raw_vel",&raw_vel_msg);
ros::Subscriber<geometry_msgs::Twist> sub_vel("cmd_vel", messageCb);
//Subcribe dock station
ros::Subscriber<std_msgs::Int8> sub_pos("check_dock_pos", messageCb_dock);

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->flush();
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
  nh.getHardware()->reset_rbuf();
  if(huart->Instance == huart1.Instance)
  	{
  		if(pData != 'A')
  		{
  			if(pData != 'B')
  			{
  				if((pData >= 46 && pData <= 57) || pData == 32 || pData == 45)
  				{
  					dataTX[i] = pData;
  					i++;
  				}
  			}
  		}

  		if(pData == 'B')
  		{
  			i = 0;
  			char *ptr;
  			ptr = strtok(dataTX," ");
  			while(ptr != NULL)
  			{
  				num[j] = atof(ptr);
  				ptr = strtok(NULL," ");
  				j++;
  				if(j == 10)
  				{
  					j = 0;
  					break;
  				}
  			}
  		}
  		HAL_UART_Receive_IT(&huart1, &pData, 1);
  	}

}

int inx_data_process = 0;
double data_process[2];

void process_data(char data[]){
	char * pch;
	pch = strtok (data_tx,",");
	while (pch != NULL)
	{
		data_process[inx_data_process++] = atof(pch);
		pch = strtok (NULL, ",");
	}
	inx_data_process = 0;
}

void setup(void)
{
  nh.initNode();
  nh.subscribe(sub_vel);
  nh.subscribe(sub_pos);
  HAL_UART_Receive_IT(&huart1, &pData, 1);
  nh.advertise(raw_vel_pub);
  nh.advertise(pub_imu);
}


void loop(void)
{
	if(g_vel_angular_right > 0 && g_vel_angular_left > 0){
		tien();
	}
	else if(g_vel_angular_right < 0 && g_vel_angular_left < 0){
		lui();
	}
	else if(g_vel_angular_right > 0 && g_vel_angular_left < 0){
		Xoay_Trai();
	}
	else if(g_vel_angular_right < 0 && g_vel_angular_left > 0){
		Xoay_Phai();
	}
	else{
		dung();
	}

	#ifdef __IR_WITH_ROS__
		if(HAL_GetTick() - countTick > 100){
			//publish velocity
			convert_vel_linear_2_wheel_to_vel_linear_and_angular(w1, w2);
			raw_vel_msg.linear.x = vx;
			raw_vel_msg.angular.z = wz;
			raw_vel_pub.publish(&raw_vel_msg);
			//publish imu/data
			imu.header.frame_id = "imu_link";
			imu.header.stamp = nh.now();
			imu.orientation.w = num[0];
			imu.orientation.x = num[1];
			imu.orientation.y = num[2];
			imu.orientation.z = num[3];
			imu.angular_velocity.x = num[7];
			imu.angular_velocity.y = num[8];
			imu.angular_velocity.z = num[9];
			imu.linear_acceleration.x = num[4];
			imu.linear_acceleration.y = num[5];
			imu.linear_acceleration.z = num[6];
			pub_imu.publish(&imu);
			countTick = HAL_GetTick();
		}

		nh.spinOnce();
	#endif
}

void messageCb_dock( const std_msgs::Int8& toggle_msg){
  docking_status = toggle_msg.data;
}

void messageCb( const geometry_msgs::Twist& cmd_msg)
{
	vxRE = cmd_msg.linear.x; //m/s
	wzRE = cmd_msg.angular.z; //rad/s
	convert_vel_linear_and_angular_to_vel_linear_2_wheel(vxRE, wzRE);
}

void convert_vel_linear_and_angular_to_vel_linear_2_wheel(double Vx, double Wz)
{
	g_vel_angular_right = (2*Vx + Wz*LENGTH_BETWEEN_2_WHEEL)/(2*WHEEL_RADIUS*haiPI);  //vòng/giây
	g_vel_angular_left = (2*Vx - Wz*LENGTH_BETWEEN_2_WHEEL)/(2*WHEEL_RADIUS*haiPI);   //vòng/giây
}

void convert_vel_linear_2_wheel_to_vel_linear_and_angular(double angular_right, double angular_left)
{
	vx = (WHEEL_RADIUS/2)*(angular_right + angular_left)*haiPI; //rad/s
	wz = (WHEEL_RADIUS/LENGTH_BETWEEN_2_WHEEL)*(angular_right - angular_left)*haiPI; //rad/s
}


