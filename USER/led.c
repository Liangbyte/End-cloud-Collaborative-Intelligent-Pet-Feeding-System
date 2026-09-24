/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*              实现LED功能的源文件                */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "led.h"        //包含需要的头文件
#include "delay.h"
#include "sys.h" 


/*-------------------------------------------------*/
/*函数名：初始化LED函数                            */
/*参  数：无                                       */
/*返回值：无 
LED1:PA0
LED2:PC15
LED3:PC14
LED4:PC13

*/
/*-------------------------------------------------*/
void LED_Init(void)
{    	 
	GPIO_InitTypeDef GPIO_InitStructure;                     //定义一个设置GPIO的变量
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);   //使能GPIOB端口时钟

	
//	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15|GPIO_Pin_14;   //设置PB0 PB1
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO速率50Mhz
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		 //推免输出方式
//	GPIO_Init(GPIOC, &GPIO_InitStructure);            		 //设置PB0 PB1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;   //设置PB0 PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		 //推免输出方式
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		 //设置PB0 PB1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;   //设置PB0 PB1
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;        //IO速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		 //推免输出方式
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		 //设置PB0 PB1	
	
	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				     //DHT11
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出 		 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	LED_AllOff();                                            //所有LED熄灭
}


void WiFi_ResetIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;                      //定义一个设置IO端口参数的结构体
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE);   //使能PA端口时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                 //准备设置PA4
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //速率50Mhz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   		  //推免输出方式
	GPIO_Init(GPIOA, &GPIO_InitStructure);            		  //设置PA4
	RESET_IO(1);                                              //复位IO拉高电平
	
}


/*-------------------------------------------------*/
/*函数名：点亮所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOn(void)
{
	LED1_ON;
	LED2_ON;
	LED3_ON;
	LED4_ON;
}

/*-------------------------------------------------*/
/*函数名：熄灭所有LED                              */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void LED_AllOff(void)
{
	LED1_OFF;
	LED2_OFF;
	LED3_OFF;
	LED4_OFF;
}

/*************************************************

ADC功能操作IO

*************************************************/
/*-------------------------------------------------*/
/*函数名：初始化ADC                                */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/													   
void Adc_Init(void)
{    
	GPIO_InitTypeDef      GPIO_InitStructure;                             //定义一个设置GPIO的变量
	ADC_InitTypeDef       ADC_InitStructure;                              //定义一个设置ADC的变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	              //使能GPIOA通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );	              //使能GPIOB通道时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	              //使能ADC1通道时钟
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                                      //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;        //准备设置PA5 6 7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		                   //模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);                                 //设置PA5 6 7
    	
	ADC_DeInit(ADC1);                                                      //复位ADC1,将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	                   //ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	                       //模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	                   //模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	   //转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	               //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	                               //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);                                    //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   
 
	ADC_Cmd(ADC1, ENABLE);	                                               //使能指定的ADC1	
	ADC_ResetCalibration(ADC1);	                                           //使能复位校准  	 
	while(ADC_GetResetCalibrationStatus(ADC1));                     	   //等待复位校准结束	
	ADC_StartCalibration(ADC1);	                                           //开启AD校准
	while(ADC_GetCalibrationStatus(ADC1));	                               //等待校准结束
}

/*-------------------------------------------------*/
/*函数名：获得ADC结果                              */
/*参  数：ch: 通道数                               */
/*返回值：ADC的结果                                */
/*-------------------------------------------------*/	
int Get_Adc(int ch)   
{	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                        //使能指定的ADC1的软件转换启动功能	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));                     //等待转换结束
	return ADC_GetConversionValue(ADC1);	                            //返回最近一次ADC1规则组的转换结果
}
/*-------------------------------------------------*/
/*函数名：平均多次ADC结果，提高精度                */
/*参  数：ch: 通道数                               */
/*参  数：times: 平均次数                          */
/*返回值：平均后的结果                             */
/*-------------------------------------------------*/	
int Get_Adc_Average(int ch,int times)
{
	int temp_val=0;
	char t;
	
	for(t=0;t<times;t++)             //循环读取times次
	{
		temp_val+=Get_Adc(ch);       //计算总值
		delay_ms(5);                //延时
	}
	return temp_val/times;           //返回平均值
} 
	 


//==========================舵机=====================================

void SG90_Init(void){ //舵机接口初始化
	GPIO_InitTypeDef  GPIO_InitStructure; 	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);       
    GPIO_InitStructure.GPIO_Pin = SE_OUT; //选择端口号（0~15或all）                        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //选择IO接口工作方式       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置IO接口速度（2/10/50MHz）    
	GPIO_Init(SE_PORT, &GPIO_InitStructure);
	GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(0)); //接口输出
}

void SG90_angle(u8 a){ //舵机角度控制设置（参数值0～180）对应角度0～180度
	u8 b=100;//角度校正偏移量
	GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(1)); //接口输出
	delay_us(500+a*10+b); //延时
	GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(0)); //接口输出
	delay_us(19500-a*10-b); //延时
	GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(1)); //接口输出
	delay_us(500+a*10+b); //延时
	GPIO_WriteBit(SE_PORT,SE_OUT,(BitAction)(0)); //接口输出
	delay_us(19500-a*10-b); //延时	
	
	
} 

