#include "timer.h"
//#include "sys.h"
//初始化定时器
void Timer_SRD_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_BaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_BaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_BaseInitStruct.TIM_Period=arr;
	TIM_BaseInitStruct.TIM_Prescaler=psc;
	TIM_BaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInit(TIM3,&TIM_BaseInitStruct);
	//中断优先级管理
	NVIC_InitStruct.NVIC_IRQChannel=TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	//开启更新中断，触发中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_ITConfig(TIM3,TIM_IT_Trigger,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);//使能中断
	
}
//中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
	
}


/**************************************************************************
PA0 做PWM输出 TIM2 CH1   https://blog.csdn.net/tongzuo1997/article/details/80963797
**************************************************************************/


void TIM2_PWM_Init(u16 arr,u16 psc)//PA0 做PWM输出
{  
	GPIO_InitTypeDef 		GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  	        TIM_OCInitStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	 //使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  	 //使能GPIO外设时钟
	
 
        //引脚功能配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 ; //TIM2_CH1 TIM2_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;          //复用推挽输出！！
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
        //初始化TIM2
	TIM_TimeBaseStructure.TIM_Period = arr;                     //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                   //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);             //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM2 Channel1、Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;           //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);                    //根据T指定的参数初始化外设TIM2 OC1
	//TIM_OC2Init(TIM2, &TIM_OCInitStructure);                    //根据T指定的参数初始化外设TIM2 OC2
 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
	//TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //使能TIM2在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2
}
/*主函数使用手法
int main(void)
{
	unsigned int pulse = 0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//中断控制器分组设置 
	Delay_Init();						//Systick初始化，用于普通的延时 
	DelayMs(200);	//等待电源稳定
	TIM2_PWM_Init(100,719);					//720分频，72000_000/720 = 100Khz的计数频率，计数到100为1KHz
	while(1)
	{		
		pulse = 0;
		for(i = 0; i < 100;i ++)
		{
			pulse += 1;
			TIM_SetCompare1(TIM2, pulse) ;
			TIM_SetCompare2(TIM2, pulse) ;
			DelayMs(50);
		}
		
		for(i = 0; i < 100;i ++)
		{
			pulse -= 1;
			TIM_SetCompare1(TIM2, pulse) ;
			TIM_SetCompare2(TIM2, pulse) ;
			DelayMs(50);
		}
	}
 
}

原文链接：https://blog.csdn.net/tongzuo1997/article/details/80963797
*/

//**********************************************************************************************************************











//============================time2=========================================================
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM4, ENABLE);  //使能TIMx					 
}
////定时器3中断服务程序
//void TIM4_IRQHandler(void)   //TIM4中断
//{
//	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
//		{
//		TIM_ClearITPendingBit(TIM4, TIM_IT_Update  );  //清除TIMx更新中断标志 
//		time1--;
//		}
//}


/*
使用
	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  


    TIM_Cmd(TIM7, ENABLE);                            //使能定时器计时
        UsartPrintf(USART1, "\r\nhello word!!n\r\n");
        TIM_Cmd(TIM7, DISABLE);                            //关闭定时器

        time = timer_info.timer7Out * 50000;            //测算时间-中断次数*中断周期
        time += TIM_GetCounter(TIM7);                    //获取当前中断前的剩余值
        TIM_SetCounter(TIM7, 0);                        //清零



*/













