/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              实现LED功能的头文件                */
/* 

LED1:PA0
LED2:PC15
LED3:PC14
LED4:PC13

*/
/*-------------------------------------------------*/
#include "sys.h" 

#ifndef __LED_H
#define __LED_H

#define LED1_OUT(x)   GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)x)  //设置PC4 的电平，可以点亮熄灭LED1
#define LED2_OUT(x)   GPIO_WriteBit(GPIOC, GPIO_Pin_15, (BitAction)x)  //设置PC5 的电平，可以点亮熄灭LED2
#define LED3_OUT(x)   GPIO_WriteBit(GPIOC, GPIO_Pin_14, (BitAction)x)  //设置PB0 的电平，可以点亮熄灭LED3
#define LED4_OUT(x)   GPIO_WriteBit(GPIOC, GPIO_Pin_13, (BitAction)x)  //设置PB1 的电平，可以点亮熄灭LED4


#define LED1_IN_STA   GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0)  //PC4 控制LED1,读取电平状态，可以判断LED1是点亮还是熄灭
#define LED2_IN_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_15)  //PC5 控制LED2,读取电平状态，可以判断LED2是点亮还是熄灭
#define LED3_IN_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_14)  //PB0 控制LED3,读取电平状态，可以判断LED3是点亮还是熄灭
#define LED4_IN_STA   GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)  //PB1 控制LED4,读取电平状态，可以判断LED4是点亮还是熄灭



#define RESET_IO(x)    GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction)x)  //PA4控制WiFi的复位


#define LED1_ON       GPIO_ResetBits(GPIOB, GPIO_Pin_9)         //共阳极，拉低PC4电平，点亮LED1
#define LED1_OFF      GPIO_SetBits(GPIOB, GPIO_Pin_9)           //共阳极，拉高PC4电平，熄灭LED1

#define LED2_ON       GPIO_ResetBits(GPIOC, GPIO_Pin_15)         //共阳极，拉低PC5电平，点亮LED2
#define LED2_OFF      GPIO_SetBits(GPIOC, GPIO_Pin_15)           //共阳极，拉高PC5电平，熄灭LED2

#define LED3_ON       GPIO_ResetBits(GPIOC, GPIO_Pin_14)        //共阳极，拉低PB0电平，点亮LED3
#define LED3_OFF      GPIO_SetBits(GPIOC, GPIO_Pin_14)          //共阳极，拉高PB0电平，熄灭LED3

#define LED4_ON       GPIO_ResetBits(GPIOA, GPIO_Pin_4)        //共阳极，拉低PB1电平，点亮LED4
#define LED4_OFF      GPIO_SetBits(GPIOA, GPIO_Pin_4)          //共阳极，拉高PB1电平，熄灭LED4

#define RELAY1_OFF       GPIO_ResetBits(GPIOA, GPIO_Pin_12)        //继电器1
#define RELAY1_ON      GPIO_SetBits(GPIOA, GPIO_Pin_12)          //

#define RELAY2_OFF       GPIO_ResetBits(GPIOA, GPIO_Pin_11)        //继电器2
#define RELAY2_ON      GPIO_SetBits(GPIOA, GPIO_Pin_11)          //

#define RELAY3_OFF       GPIO_ResetBits(GPIOB, GPIO_Pin_8)        //继电器3
#define RELAY3_ON      GPIO_SetBits(GPIOB, GPIO_Pin_8)   

#define BEEP_OFF    GPIO_ResetBits(GPIOA, GPIO_Pin_8)         //共阳极，拉低PB13电平，点亮LED1
#define BEEP_ON   GPIO_SetBits(GPIOA, GPIO_Pin_8)           //共阳极，拉高PC4电平，熄灭LED1


#define SE_PORT	GPIOC	//定义IO接口
#define SE_OUT	GPIO_Pin_13	//定义IO接口



void LED_Init(void);               //初始化	
void LED_AllOn(void);              //点亮所有LED
void LED_AllOff(void);             //熄灭所有LED
void WiFi_ResetIO_Init(void);    //WIFI RESET IO


void Adc_Init(void); 				  //初始化ADC
int Get_Adc(int ch) ; 				  //获得ADC结果 
int Get_Adc_Average(int ch,int times);//平均多次ADC结果，提高精度




void SG90_Init(void);//SG90舵机初始化
void SG90_angle(u8 a);//舵机角度设置

#endif
