/*-------------------------------------------------*/
/*            超纬电子STM32系列开发板              */
/*-------------------------------------------------*/
/*                                                 */
/*            实现扫描按键功能的源文件             */
/*                                                 */
/*-------------------------------------------------*/

#include "stm32f10x.h"  //包含需要的头文件
#include "delay.h"      //包含需要的头文件
#include "key.h"      //包含需要的头文件

/*-------------------------------------------------*/
/*函数名：初始化KEY按键函数                        */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/
void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;                       //定义一个设置IO的结构体
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);    //使能PA端口时钟

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //复用功能时钟   (这句重点) 
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE); //SWJ完全失能(JTAG+SW-DP) 	
	
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);//完全禁用SWD及JTAG 
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     //禁用JTAG
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_7;                 //准备设置PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;   		   //上拉输入，默认状态是高电平   GPIO_Mode_AF_OD   GPIO_Mode_IPU
	GPIO_Init(GPIOB, &GPIO_InitStructure);            		   //设置PA0

} 





///*-------------------------------------------------*/
///*函数名：按键扫描函数                             */
///*参  数：无                                       */
///*返回值：有无按键按下，有的话是哪个按键按下       */
///*-------------------------------------------------*/
//char KEY_Scan(void)
//{	 	  
//	if(KEY1_IN_STA==1){                  //判断控制KEY1的IO输入电平，是否是高电平，如果是进入if	
//		Delay_Ms(5);                     //延时5ms，消除抖动的误判
//		if(KEY1_IN_STA==1){              //消除抖动的影响后，再次判断控制KEY1的IO输入电平，是否是高电平，如果是进入if，说明按键按下了	
//			while(1){                    //等待按键抬起
//				if(KEY1_IN_STA==0){      //判断控制KEY1的IO输入电平，是否是低电平，如果是进入if	
//					Delay_Ms(5);         //延时5ms，消除抖动的误判
//					if(KEY1_IN_STA==0){  //消除抖动的影响后，再次判断控制KEY1的IO输入电平，是否是低电平，如果是进入if，说明按键抬起了
//						return 1;        //返回1，表示KEY1按下了
//					}
//				}
//			}
//		}
//	}else if(KEY2_IN_STA==0){            //判断控制KEY2的IO输入电平，是否是低电平，如果是进入if	
//		Delay_Ms(5);                     //延时5ms，消除抖动的误判
//		if(KEY2_IN_STA==0){              //消除抖动的影响后，再次判断控制KEY2的IO输入电平，是否是低电平，如果是进入if，说明按键按下了	
//			while(1){                    //等待按键抬起
//				if(KEY2_IN_STA==1){      //判断控制KEY2的IO输入电平，是否是高电平，如果是进入if
//					Delay_Ms(5);         //延时5ms，消除抖动的误判
//					if(KEY2_IN_STA==1){  //消除抖动的影响后，再次判断控制KEY2的IO输入电平，是否是高电平，如果是进入if，说明按键抬起了
//						return 2;        //返回2，表示KEY2按下了
//					}
//				}
//			}
//		}
//	}	
//	return 0;                            //如果没有按键按下，返回0
//}


//void KEY_add(u32 tempxx, int aa)
//{	 	  
//	if(KEY4_IN_STA==0){                  //判断控制KEY4的IO输入电平，是否是高电平，如果是进入if	
//		Delay_Ms(100);                     //延时5ms，消除抖动的误判
//		if(KEY4_IN_STA==0){              //消除抖动的影响后，再次判断控制KEY4的IO输入电平，是否是高电平，如果是进入if，说明按键按下了	
//						tempxx = tempxx+aa;						
//		}
//	}
//}



//void KEY_cutdowm(u32 tempxx, int aa)	
//{  
//	if(KEY3_IN_STA==0){                  //判断控制KEY3的IO输入电平，是否是高电平，如果是进入if	
//		Delay_Ms(100);                     //延时5ms，消除抖动的误判
//		if(KEY3_IN_STA==0){              //消除抖动的影响后，再次判断控制KEY3的IO输入电平，是否是高电平，如果是进入if，说明按键按下了	
//						tempxx = tempxx+aa;					
//		}
//	}
//}






/*-------------------------------------------------*/
/*函数名：按键外部中断初始化程序.                  */
/*参  数：无                                       */
/*返回值：无                                       */
/*-------------------------------------------------*/	   
void KEY_Exti_Init(void)
{
	
	NVIC_InitTypeDef   NVIC_InitStructure; //定义一个设置IO的结构体
	EXTI_InitTypeDef   EXTI_InitStructure; //定义一个设置中断的结构体 

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	          //使能GPIO复用功能时钟
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);    //配置KEY1-PA0中断线
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource1);    //配置KEY2-PA1中断线	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);    //配置KEY1-PA0中断线
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);    //配置KEY2-PA1中断线		
	
	
	
  	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	                  //准备设置外部0中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //上升沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
  	EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断0线
	
  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;	                  //准备设置KEY2-PA1外部1中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //下降沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
  	EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断1线

  	EXTI_InitStructure.EXTI_Line = EXTI_Line10;	                  //准备设置外部0中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;        //上升沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
  	EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断0线
	
  	EXTI_InitStructure.EXTI_Line = EXTI_Line15;	                  //准备设置KEY2-PA1外部1中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //下降沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
  	EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断1线








	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			  //准备设置外部中断0
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		      //子优先级0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			      //使能外部中断0通道
  	NVIC_Init(&NVIC_InitStructure);                               //设置外部中断0

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			  //准备设置外部中断1
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		      //子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //使能外部中断1通道
  	NVIC_Init(&NVIC_InitStructure);                               //设置外部中断1	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			  //准备设置外部中断0
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		      //子优先级0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			      //使能外部中断0通道
  	NVIC_Init(&NVIC_InitStructure);                               //设置外部中断0

    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			  //准备设置外部中断1
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		      //子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //使能外部中断1通道
  	NVIC_Init(&NVIC_InitStructure);                               //设置外部中断1		
	
	
	
	
/*	
	
	
	NVIC_InitTypeDef   NVIC_InitStructure; //定义一个设置IO的结构体
	EXTI_InitTypeDef   EXTI_InitStructure; //定义一个设置中断的结构体 

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	          //使能GPIO复用功能时钟
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);    //配置KEY1-PA0中断线
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);    //配置KEY2-PA1中断线

	
	
	
	
	
  	EXTI_InitStructure.EXTI_Line = EXTI_Line0;	                  //准备设置外部0中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //上升沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
  	EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断0线
	
  	EXTI_InitStructure.EXTI_Line = EXTI_Line1;	                  //准备设置KEY2-PA1外部1中断线
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	          //外部中断
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;       //下降沿触发中断
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;                     //使能
  	EXTI_Init(&EXTI_InitStructure);	 	                          //设置外部中断1线

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			  //准备设置外部中断0
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		      //子优先级0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			      //使能外部中断0通道
  	NVIC_Init(&NVIC_InitStructure);                               //设置外部中断0

    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			  //准备设置外部中断1
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;     //抢占优先级3
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		      //子优先级1
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				  //使能外部中断1通道
  	NVIC_Init(&NVIC_InitStructure);                               //设置外部中断1
		
		
		
		*/
}




















