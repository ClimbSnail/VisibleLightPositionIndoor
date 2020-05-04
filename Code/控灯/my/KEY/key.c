#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "led.h"
#include "timer.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
static int flag=0; 
	
void EXTIX_Init(void)
{
	KEY_Init();
	Ex_NVIC_Config(GPIO_B,7,FTIR); //下降沿触发
	Ex_NVIC_Config(GPIO_B,6,FTIR); //下降沿触发
	Ex_NVIC_Config(GPIO_B,5,FTIR); //下降沿触发
	Ex_NVIC_Config(GPIO_B,4,FTIR); //下降沿触发
	
	MY_NVIC_Init(2,3,EXTI9_5_IRQn,2); //抢占 2，子优先级 1，组 2
	MY_NVIC_Init(2,0,EXTI4_IRQn,2); //抢占 2，子优先级 0，组 2
}

//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
	RCC->APB2ENR|=1<<3; //使能 PORTB 时钟
	RCC->APB2ENR|=1<<0; //使能 引脚复用 时钟
	
	GPIOB->CRL&=0X0FFFFFFF; //PB7 设置成输入，默认下拉
	GPIOB->CRL|=0X80000000;
	GPIOB->CRL&=0X0FFFFFFF; //PB6 设置成输入，默认下拉
	GPIOB->CRL|=0X80000000;
	GPIOB->CRL&=0X0FFFFFFF; //PB5 设置成输入，默认下拉
	GPIOB->CRL|=0X80000000;
	GPIOB->CRL&=0X0FFFFFFF; //PB4 设置成输入，默认下拉
	GPIOB->CRL|=0X80000000;
	GPIOB->ODR|= 15 << 4;  //   全部上拉
	

}


u8 KEY_Scan(void)
{	  
	if((KEY1==0||KEY2==0||KEY3==0||KEY4==1))
	{
		delay_ms(10);//去抖动 
		if(KEY1==0)return KEY1_PRES;
		else if(KEY2==0)return KEY2_PRES;
		else if(KEY3==0)return KEY3_PRES;
		else if(KEY4==1)return KEY4_PRES;
	} 	
 	return 0;// 无按键按下
}

void EXTI9_5_IRQHandler()
{
	
	delay_ms(50); //消抖
	if(KEY1==0)
	{		
		TIM2->CR1 &= ~0x01; //关闭定时器 2
		TIM2->CNT = 0;
		if( flag == 0 )
		{
		LED1 = 0;
		LED2 = 0;
		LED3 = 0;
		}
		else if( flag == 1 )
				{
					LED1 = 1;
					LED2 = 0;
					LED3 = 0;
				}
				else if( flag == 2 )
				{
					LED1 = 0;
					LED2 = 1;
					LED3 = 0;
				}
					else if( flag == 3 )
					{
						LED1 = 0;
						LED2 = 0;
						LED3 = 1;
					}
					else if( flag == 4 )
					{
						LED1 = 1;
						LED2 = 1;
						LED3 = 1;
					}
		start = 0;
		LED4 = 1;
		flag = (flag+1)%5;
		delay_ms(50); //消抖
		while(!KEY1);
					
	}
	else if(KEY2==0)
	{
		TIM2->CNT = 0;
		TIM2->CR1|=0x01; //使能定时器 2
		
		LED4 = 0;
		while(!KEY2);
		start = 1;
	}
	else if(KEY3==0)
	{	
		
		
		while(!KEY3);	

	}
	EXTI->PR=1<<7; //清除 LINE7 上的中断标志位
	EXTI->PR=1<<6; //清除 LINE6 上的中断标志位		
	EXTI->PR=1<<5; //清除 LINE5 上的中断标志位
}

void EXTI4_IRQHandler()
{
	delay_ms(20); //消抖
	if( KEY4 == 0 )
	{
		
		
		while(!KEY4);	
		EXTI->PR|= 1<<4;  //清除 LINE4 上的中断标志位
	}
	EXTI->PR|= 1<<4;  //清除 LINE4 上的中断标志位
}





