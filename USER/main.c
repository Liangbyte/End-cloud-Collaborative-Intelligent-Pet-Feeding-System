/*
key  PB0 PB1 PB12 PB11 	ok
beep PA8 								ok
水位 PA6					 			OK
光照 PA5 								OK
继电器 PA12 PA11  PB8 	OK
WIFI PA1 PA2 PA3 				OK
OLED PB5 PB6 						OK
舵机	PC13							OK

DHT11  PB11  					待调试
LIGHT  PA0						待调试
HX711称重  PB3 PB4		待调试
*/

#include "sys.h"  
#include "delay.h"
#include "oled_iic.h"
#include "stdio.h"
#include "wave.h"
#include "timer.h"
#include "ds18b20.h" 
#include "key.h"        //包含需要的头文件
#include "led.h"        //包含需要的头文件
#include "usart22.h"
#include "stdbool.h"
#include <stdio.h>  
#include <string.h>  
#include "usart.h"	
#include "DHT11.h"
#include "HX711.h"
#include "rtc.h"  //内部RTC时钟

//=======================================================================
#define WiFi_RX_BUF     USART2_RX_BUF       //串口1控制 WiFi
#define WiFi_RXBUFF_SIZE  USART2_REC_LEN  //串口1控制 WiFi
#define WiFi_RxCounter    USART2_RX_STA    //串口1控制 WiFi
char *str1;							//wifi
extern u8 SHUZI_RX_BUF[USART2_REC_LEN]; //阈值设定数组

int time1=0,time2=0,time3=0;
u8 set_mark1=0,automark=0;
short Temp1set=30,Humi1set=60,shuiweiset=30,weightset=100;
short temperature;   
char databuff[64];          //缓冲区
u8 set_mark=0;
extern u8 DHT11Data[4];//温湿度数据(温度高位,温度低位,湿度高位,湿度低位)
int Temp1,Humi1,light_pa5,shuiwei_pa6,shuiwei_pa7;   // ADC的值 光照 PA5 水位PA6  水位2 PA7
extern s32 Weight_Shiwu; //称重的数量
int i,sg90munber=0;	
int  adcdata[3];          //用于保存3个ADC通道的数据	  保存水位值	
u8 light_onoff = 1;
short auto_k1=1,auto_k2=1,auto_k3=1,auto_k4=1;
int onenet_time=0;


extern u16 ryear;//RTC时间设置
extern u8 rmon,rday,rhour,rmin,rsec,rweek;//RTC时间设置
u8 bya=1;  //第一次设置时间需要置1
_Bool showtime=0;
u8 rhourset1=13,rminset1=21,rhourset2=13,rminset2=22;
_Bool jialiangmark=1,jialiangmark2=1;
	_Bool shuibengmark=0,SG09Mark=0;		//手动控制加水加粮

void shoudongkongzhi();	//按键可以手动控制加水加粮



void get_shuju() //收集数据，上传云端
{
	shoudongkongzhi();	//按键可以手动控制加水加粮
	
		OLED_ShowCH(0,0,"温度:");		//测试显示中文
		OLED_ShowCH(102,0,"℃");		//测试显示中文
		OLED_ShowCH(0,2,"湿度:");		//显示中文：温度		
		OLED_ShowCH(101,2,"%");		//显示：℃	
		OLED_ShowCH(0,4,"余粮:");		//测试显示中文
		OLED_ShowCH(102,4,"G");		//测试显示中文
		OLED_ShowCH(0,6,"亮度:");		//显示中文：温度		128*64
//		OLED_ShowCH(60,6,"水位:");		//显示中文：温度	
	
		DHT11_Read_Data();		//温湿度计时变量 	
		Temp1=DHT11Data[2];
		Humi1=DHT11Data[0];
		OLED_ShowNum(50,0,Temp1,5,0);
		OLED_ShowNum(50,2,Humi1,5,0);					
		
		Get_Weight(); //获取重量

		OLED_ShowNum(50,4,Weight_Shiwu,6,0);	  //显示重量

//********************adc获取light  shuiwei**********************************************	
		for(i=0;i<3;i++){
	  adcdata[i] = (Get_Adc_Average(i+5,10));
		}
		light_pa5 = 100-adcdata[0]/41;  //最大4095   该公式控制亮度值为0~100
		shuiwei_pa6 =adcdata[1]/41;    //最大4095   该公式控制水位的值为0~100
		shuiwei_pa7 =adcdata[2];		//200时表示水位很满，80以下为缺水不
		
    if(light_onoff%2==1)TIM_SetCompare1(TIM2, light_pa5) ;	//自动调节PWM灯光
		else TIM_SetCompare1(TIM2, 120);   //关闭灯光
		OLED_ShowNum(40,6,light_pa5,2,0);
//		OLED_ShowNum(100,6,shuiwei_pa6,2,0);	
		
	if(PBin(7)==0){
		OLED_ShowCH(80-16,6,"进食中");		//测试显示中文
		shuiwei_pa6=1;
		
	}
	else
	{
		OLED_ShowCH(80-16,6,"        ");		//测试显示中文		
		shuiwei_pa6=0;
		
	}		
//********************数据上传云端**********************************************	
	if(onenet_time==10)
		{	
    sprintf(databuff,"{\"temperature\":%d,\"humidity\":%d,\"yuliang\":%d}",Temp1,Humi1,Weight_Shiwu);   //构建数据
		u2_printf(databuff);	
		delay_ms(100);	
		}		
	if(onenet_time++==20)
		{	
		onenet_time=0;			
		sprintf(databuff,"{\"light\":%d,\"shuiwei\":%d}",light_pa5,shuiwei_pa6);   //构建数据		
		u2_printf(databuff);	
		}
}

void set_wendu() //设置温度
{
	if(KEY2_IN_STA==0){                  //+5
		delay_ms(100);                     
		if(KEY2_IN_STA==0){              
				Temp1set = Temp1set+5;						
		}
	}
	if(KEY3_IN_STA==0){                  //-5
		delay_ms(100);                     
		if(KEY3_IN_STA==0){              
			Temp1set = Temp1set-5;
			if(Temp1set<=0)Temp1set=0;			
		}
	}
		OLED_ShowCH(0,0,"温度:");		//显示中文：温度	
		OLED_ShowCH(101,0,"℃");		//显示：℃
		OLED_ShowNum(50,0,Temp1set,5,0);
		OLED_ShowCH(0,3,"设置模式");		//显示中文：温度	
}

void set_shidu() //设置湿度
{
	if(KEY2_IN_STA==0){                  //+5
		delay_ms(100);                     
		if(KEY2_IN_STA==0){              
				Humi1set = Humi1set+5;						
		}
	}
	if(KEY3_IN_STA==0){                  //-5
		delay_ms(100);                     
		if(KEY3_IN_STA==0){              
			Humi1set = Humi1set-5;
			if(Humi1set<=0)Humi1set=0;			
		}
	}
		OLED_ShowCH(0,0,"湿度:");		//显示中文：温度	
		OLED_ShowCH(101,0,"%");		//显示：℃
		OLED_ShowNum(50,0,Humi1set,5,0);
		OLED_ShowCH(0,3,"设置模式");		//显示中文：温度	
}

void set_weight() //设置湿度
{
	if(KEY2_IN_STA==0){                  //+5
		delay_ms(100);                     
		if(KEY2_IN_STA==0){              
				weightset = weightset+50;						
		}
	}
	if(KEY3_IN_STA==0){                  //-5
		delay_ms(100);                     
		if(KEY3_IN_STA==0){              
			weightset = weightset-50;
			if(weightset<=0)weightset=0;			
		}
	}
		OLED_ShowCH(0,0,"余粮:");		//显示中文：温度	
		OLED_ShowCH(101,0,"G");		//显示：℃
		OLED_ShowNum(50,0,weightset,5,0);
		OLED_ShowCH(0,3,"设置模式");		//显示中文：温度	
}

void set_shuiwei() //设置湿度
{
	if(KEY2_IN_STA==0){                  //+5
		delay_ms(100);                     
		if(KEY2_IN_STA==0){              
				shuiweiset = shuiweiset++;						
		}
	}
	if(KEY3_IN_STA==0){                  //-5
		delay_ms(100);                     
		if(KEY3_IN_STA==0){              
			shuiweiset = shuiweiset--;
			if(shuiweiset<=0)shuiweiset=0;			
		}
	}
		OLED_ShowCH(0,0,"水位:");		//显示中文：温度	
		OLED_ShowCH(101,0,"%");		//显示：℃
		OLED_ShowNum(50,0,shuiweiset,5,0);
		OLED_ShowCH(0,3,"设置模式");		//显示中文：温度	
}



void set_hour1() //设置湿度
{
	showtime=1;
	if(KEY2_IN_STA==0){                  //+5
		delay_ms(100);                     
		if(KEY2_IN_STA==0){              
				rhourset1++;		
				if(rhourset1>24)rhourset1=0;
		}
	}
	if(KEY3_IN_STA==0){                  //-5
		delay_ms(100);                     
		if(KEY3_IN_STA==0){              
			if(rhourset1==0)rhourset1=0;
			else			rhourset1--;
		}
	}

		OLED_ShowNum(0,4,rhourset1,2,1);	//显示
		OLED_ShowCH(16,4,":");						
		OLED_ShowNum(24,4,rminset1,2,1);	//显示		
		delay_ms(100);   
		OLED_ShowCH(0,4,"  ");		//显示中文：温度				
		OLED_ShowCH(0,6,"rhour设置模式1");		//显示中文：温度	
}
void set_min1() //设置湿度
{
	showtime=1;
	if(KEY2_IN_STA==0){                  //+5
		delay_ms(100);                     
		if(KEY2_IN_STA==0){              
				rminset1++;		
				if(rminset1>60)rminset1=0;
		}
	}
	if(KEY3_IN_STA==0){                  //-5
		delay_ms(100);                     
		if(KEY3_IN_STA==0){              	
				rminset1--;
				if(rminset1==255)rminset1=60;
		}
	}
		OLED_ShowNum(0,4,rhourset1,2,1);	//显示
		OLED_ShowCH(16,4,":");						
		OLED_ShowNum(24,4,rminset1,2,1);	//显示		
		delay_ms(100);   
		OLED_ShowCH(24,4,"  ");		//显示中文：温度				
		OLED_ShowCH(0,6,"rmin设置模式1");		//显示中文：温度	
}

void set_hour2() //设置湿度
{
	showtime=1;
	if(KEY2_IN_STA==0){                  //+5
		delay_ms(100);                     
		if(KEY2_IN_STA==0){              
				rhourset2++;		
				if(rhourset2>24)rhourset2=0;
		}
	}
	if(KEY3_IN_STA==0){                  //-5
		delay_ms(100);                     
		if(KEY3_IN_STA==0){              
			if(rhourset2==0)rhourset2=0;
			else			rhourset2--;
		}
	}

		OLED_ShowNum(0,4,rhourset2,2,1);	//显示
		OLED_ShowCH(16,4,":");						
		OLED_ShowNum(24,4,rminset2,2,1);	//显示		
		delay_ms(100);   
		OLED_ShowCH(0,4,"  ");		//显示中文：温度				
		OLED_ShowCH(0,6,"rhour设置模式2");		//显示中文：温度	
}
void set_min2() //设置湿度
{
	showtime=1;
	if(KEY2_IN_STA==0){                  //+5
		delay_ms(100);                     
		if(KEY2_IN_STA==0){              
				rminset2++;		
				if(rminset2>60)rminset2=0;
		}
	}
	if(KEY3_IN_STA==0){                  //-5
		delay_ms(100);                     
		if(KEY3_IN_STA==0){              	
				rminset2--;
				if(rminset2==255)rminset2=60;
		}
	}
		OLED_ShowNum(0,4,rhourset2,2,1);	//显示
		OLED_ShowCH(16,4,":");						
		OLED_ShowNum(24,4,rminset2,2,1);	//显示		
		delay_ms(100);   
		OLED_ShowCH(24,4,"  ");		//显示中文：温度				
		OLED_ShowCH(0,6,"rmin设置模式2");		//显示中文：温度	
}



void auto_kongzhi()
{
	if(Temp1>Temp1set)RELAY1_ON;//温度控制
	else RELAY1_OFF;
	if(Humi1<Humi1set)RELAY2_ON;//湿度控制
	else RELAY2_OFF;
	
	if(rhourset1==rhour && rminset1==rmin+1)
	{
		jialiangmark=1;//软体开启定时加粮功能1
		
	}
	if(rhourset2==rhour && rminset2==rmin+1)
	{
		jialiangmark2=1;//软体开启定时加粮功能2
		
	}	
	
	if(rhourset1==rhour && rminset1==rmin && jialiangmark==1){  //定时加粮功能1
			OLED_Clear();	
		while(weightset>Weight_Shiwu){
		OLED_ShowCH(0,0,"定时1加粮中..");		//测试显示中文
		SG90_angle(90);//开粮食
		BEEP_ON;
		Get_Weight(); //获取重量
		OLED_ShowNum(50,4,Weight_Shiwu,6,0);	  //显示重量
			
		}
			OLED_Clear();	
		SG90_angle(0);//关粮食
		BEEP_OFF;
		jialiangmark=0;
		
	}
	if(rhourset2==rhour && rminset2==rmin && jialiangmark2==1){  //定时加粮功能2
			OLED_Clear();	
		while(weightset>Weight_Shiwu){
		OLED_ShowCH(0,0,"定时2加粮中..");		//测试显示中文
		SG90_angle(90);//开粮食
		BEEP_ON;
		Get_Weight(); //获取重量
		OLED_ShowNum(50,4,Weight_Shiwu,6,0);	  //显示重量
		}
			OLED_Clear();	
		SG90_angle(0);//关粮食
		BEEP_OFF;
		jialiangmark2=0;
		
	}	
	
	
	
//	if(Weight_Shiwu<100)  //加粮食
//	{
//		SG90_angle(90);
////		BEEP_ON;
//		}
//	else {
//		SG90_angle(0);
////		BEEP_OFF;
//	}
	
	if(shuiwei_pa6<10)RELAY3_ON;//水量控制
	else RELAY3_OFF;
}



void shoudongkongzhi()	//按键可以手动控制加水加粮
{
	

	
	if(KEY3_IN_STA==0){                  //设置阈值选项
	delay_ms(100);                     
	if(KEY3_IN_STA==0){              
			
		auto_k1=2;
		SG09Mark=!SG09Mark;
		}
	while(KEY3_IN_STA==0);
	}		
	
	if(KEY2_IN_STA==0){                  //设置阈值选项
	delay_ms(100);                     
	if(KEY2_IN_STA==0){              
			
		auto_k1=1; //开自动控制
		}
	while(KEY2_IN_STA==0);
	}		
	

	if(SG09Mark==1&&auto_k1==2)  //关闭自动控制  开粮
{
	SG90_angle(90);	
} 
	if(SG09Mark==0&&auto_k1==2)  //关闭自动控制  关粮
{
	SG90_angle(0);	
} 
	
}



void appyuzhixiugai(void)	//APP阈值设置
{
	u16 settemp=0;
	u8 hourset=0,minset=0;
	
	////==============物联网控制==================		
	if(WiFi_RX_BUF[0]!=0)		//数字设定值		收到A
		{
			for(i=0;i<=(sizeof(WiFi_RX_BUF)/sizeof(WiFi_RX_BUF[0]));i++)
			{
				SHUZI_RX_BUF[i]=WiFi_RX_BUF[i+1];
			}
			sscanf(SHUZI_RX_BUF,"%d",&settemp);
			
			OLED_ShowNum(90,6,settemp,4,1);	//显示	
			switch (WiFi_RX_BUF[0])
			{
				case 'O':
						hourset=settemp/100;
						minset=settemp%100;
						rhourset1=hourset;
						rminset1=minset;
						delay_ms(300);	

            break;	

				case 'P':
						hourset=settemp/100;
						minset=settemp%100;
						rhourset2=hourset;
						rminset2=minset;
						delay_ms(300);	
			
            break;

				case 'Q':
						weightset=settemp;
						delay_ms(300);	
			
            break;

				case 'R':
						Temp1set=settemp;
						delay_ms(300);	
			
            break;

				case 'S':
						Humi1set=settemp;
						delay_ms(300);	
			
            break;
				
				case 'N':
						hourset=settemp/100;
						minset=settemp%100;
						bya=RTC_Set(ryear,rmon,rday,hourset,minset,rsec); //将数据写入RTC计算器的程序	
						delay_ms(300);	
			
            break;		

				


        default: break;
						
			
			}
	}
	
////==============物联网控制清除数组数据==================	
		WiFi_RxCounter=0;                               //WiFi接收数据量变量清零  
		memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 	

	
}














int main(void)
{		
	u8 a=0;	
	
	u8 val = 0;
	Timer_SRD_Init(9998,7199);//time3  定时器初始化
//	TIM4_Int_Init(4999,20000);// time4 倒计时中断1s   10Khz的计数频率，计数到5000为500ms  	65536  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	
	delay_init();
	OLED_Init();		 	//OLED屏幕初始化		
	LED_Init();	                    //LED初始化  dht11初始化 BEEP初始化 WIFI
	KEY_Init();  	//按键初始化
	Adc_Init(); 				//水位初始化
	SG90_Init();					//舵机初始化  PC13
	DHT11_Init();//???DHT11	
	TIM2_PWM_Init(100,719);					//light PA0 的 PWM使用    720分频，72000_000/720 = 100Khz的计数频率，计数到100为1KHz
	uart_init(115200);	  // DEBUG
	uart2_init(115200);  //onenet串口初始化为115200	  //	uart2_init(9600);  //蓝牙9600
	
		
	u1_printf("U1 OK");
	u2_printf("U2 OK");
//	SG90_angle(90);
//	LED_AllOn();
//	BEEP_ON;
//	RELAY1_ON;	
//	RELAY2_ON;
//	RELAY3_ON;
	delay_ms(100);
	LED_AllOff();
	BEEP_OFF;
	RELAY1_OFF;	
	RELAY2_OFF;
	RELAY3_OFF;
	SG90_angle(0);		
	OLED_Clear();	
	//while(Wave_SRD_Strat(t)); //超声波初始化
	
	Init_HX711pin();			//HX711 称重模块初始化 一定要最后一个初始化
	delay_ms(100);	
	Get_Maopi();				//HX711  称毛皮重量
	delay_ms(100);	
	Get_Maopi();				//HX711  称毛皮重量	
	delay_ms(100);		
	
//		OLED_ShowCH(0,0,"温度:");		//测试显示中文
//		OLED_ShowCH(102,0,"℃");		//测试显示中文
//		OLED_ShowCH(0,2,"湿度:");		//显示中文：温度		
//		OLED_ShowCH(101,2,"%");		//显示：℃	
//		OLED_ShowCH(0,4,"余粮:");		//测试显示中文
//		OLED_ShowCH(102,4,"%");		//测试显示中文
//		OLED_ShowCH(0,6,"亮度:");		//显示中文：温度		128*64
//		OLED_ShowCH(60,6,"水位:");		//显示中文：温度			
//	
//	
	WiFi_RxCounter=0;                               //WiFi接收数据量变量清零  
	memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 	


while(1){
appyuzhixiugai();	//APP阈值设置	
	
	
	//==============RTC 实时时钟==================	
	if(bya==1)	{	//时间初始化
	ryear=2024;rmon=02;rday=20;rhour=13;rmin=20;rsec=0;rweek=5;
	bya=RTC_Set(ryear,rmon,rday,rhour,rmin,rsec); //将数据写入RTC计算器的程序	
}
	if(RTC_Get()==0  && bya==0){ //读出时间值，同时判断返回值是不是0，非0时读取的值是错误的。
		if(showtime==1){
		//======================时间显示========================================================
//		OLED_ShowNum(0,0,ryear,4,16);	//显示
//		OLED_ShowCH(32,0,"-");					
//		OLED_ShowNum(40,0,rmon,2,16);	//显示
//		OLED_ShowCH(56,0,"-");					
//		OLED_ShowNum(64,0,rday,2,16);	//显示	
//			
		OLED_ShowNum(0,2,rhour,2,16);	//显示
		OLED_ShowCH(16,2,":");					
		OLED_ShowNum(24,2,rmin,2,16);	//显示
		OLED_ShowCH(42,2,":");					
		OLED_ShowNum(50,2,rsec,2,16);	//显示
			
//		OLED_ShowNum(0,4,rhourset1,2,1);	//显示
//		OLED_ShowCH(16,4,":");						
//		OLED_ShowNum(24,4,rminset1,2,1);	//显示				
//			
//			
//		OLED_ShowNum(0,6,rhourset2,2,1);	//显示
//		OLED_ShowCH(16,6,":");						
//		OLED_ShowNum(24,6,rminset2,2,1);	//显示				
			
			
		}
		}
	
	
	



	if(KEY1_IN_STA==0){                  //设置阈值选项
	delay_ms(100);                     
	if(KEY1_IN_STA==0){              
			val++ ;	
			OLED_Clear();		
			showtime=0;		
//			jialiangmark=1;//按键开启定时加粮功能
		}
	}
	if(KEY4_IN_STA==0){                  //LED灯控制
	delay_ms(100);                     
	if(KEY4_IN_STA==0){              
		light_onoff++;
		}
	while(KEY4_IN_STA==0);
	}		
	

	

	
    switch (val%8)
    {
        case 0:
//						if(KEY2_IN_STA==0){                  //设置阈值选项
//							delay_ms(100);                     
//							if(KEY2_IN_STA==0){              
//									showtime=!showtime;	
//									OLED_Clear();			
//								}
//							}
						if(showtime==0){
								get_shuju();							
						}
            break;
        case 1:
            set_wendu(); //设置温度
            break;
        case 2:
						set_shidu(); //设置湿度
            break;
        case 3:
            set_weight();
            break;					
				
				
				
				
				
        case 4:
            set_hour1();
            break;				
        case 5:
            set_min1();
            break;					
        case 6:
            set_hour2();
            break;		
        case 7:
            set_min2();
            break;					
//        case 7:
//            set_min2();
//            break;	

				
        default: return 0;
    }
	if(auto_k1==1)	auto_kongzhi();			//自动控制函数
//	delay_ms(1);
	a++;
//////==============物联网控制==================	
//			WiFi_RxCounter=0;                               //WiFi接收数据量变量清零  
//			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);         //清空WiFi接收缓冲区 		
//	
}
}


