#include "timer.h"
#include "led.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "oled.h"

float pitch,roll,yaw; 		//欧拉角
short aacx,aacy,aacz;		//加速度传感器原始数据
short gyrox,gyroy,gyroz;	//陀螺仪原始数据
short temp;					//温度
u8 start;
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&0X0001) //溢出中断
	{
		start = 1;
	}			
	TIM2->SR&=~(1<<0); //清除中断标志位
}
//通用定时器 2 中断初始化
//这里时钟选择为 APB1 的 2 倍，而 APB1 为 36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器 2!
void TIM2_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<0; //TIM2 时钟使能
	TIM2->ARR=arr; //设定计数器自动重装值//刚好 1ms
	TIM2->PSC=psc; //预分频器 7200,得到 10Khz 的计数时钟
	TIM2->DIER|=1<<0; //允许更新中断
//	TIM2->CR1|=0x01; //使能定时器 2

	MY_NVIC_Init(1,0,TIM2_IRQn,2);//抢占 1，子优先级 3，组 2
}


//PWM 输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u16 arr,u16 psc)
{
		//此部分需手动修改 IO 口设置
		RCC->APB1ENR|=1<<11;      //TIM1 时钟使能
		RCC->APB2ENR|=1<<2; //使能 PORTA 时钟
	
		GPIOA->CRH&=0XFFFFFFF0; //PA8 输出
		GPIOA->CRH|=0X0000000B; //复用功能输出
		GPIOA->CRH&=0XFFFFFF0F; //PA9 输出
		GPIOA->CRH|=0X000000B0; //复用功能输出
		GPIOA->CRH&=0XFFFFF0FF; //PA10 输出
		GPIOA->CRH|=0X00000B00; //复用功能输出
		GPIOA->CRH&=0XFFFF0FFF; //PA11 输出
		GPIOA->CRH|=0X0000B000; //复用功能输出
	
	
		RCC->APB2ENR|=1<<0; //开启辅助时钟
	
		AFIO->MAPR&=0XFFFFFF3F; //清除 MAPR 的[7:6]
//		AFIO->MAPR|=0<<6; //没有重映像,TIM1_CH1N->PB13
		TIM1->ARR=arr; //设定计数器自动重装值
		TIM1->PSC=psc; //预分频器不分频
		
		TIM1->CCMR1|=7<<4; //CH1 PWM2 模式
		TIM1->CCMR1|=7<<12; //CH2 PWM2 模式
		TIM1->CCMR2|=7<<4; //CH3 PWM2 模式
		TIM1->CCMR2|=7<<12; //CH4 PWM2 模式
		
		TIM1->CCMR1|=1<<3; //CH1 预装载使能	
		TIM1->CCMR1|=1<<11; //CH2 预装载使能
		TIM1->CCMR2|=1<<3; //CH3 预装载使能	
		TIM1->CCMR2|=1<<11; //CH4 预装载使能
		
		TIM1->CCER|=1<<0; //OC1 输出使能
		TIM1->CCER|=1<<4; //OC2 输出使能
		TIM1->CCER|=1<<8; //OC3 输出使能
		TIM1->CCER|=1<<12; //OC4 输出使能
		
		TIM1->CR1=0x0080; //ARPE 使能
		
		TIM1->CR1|=0x01; //使能定时器 1	
}

//PWM 输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(u16 arr,u16 psc)
{
		//此部分需手动修改 IO 口设置
		RCC->APB1ENR|=1<<1;      //TIM3 时钟使能
		RCC->APB2ENR|=1<<3; //使能 PORTB 时钟
	
		GPIOB->CRL&=0XFFFFFFF0; //PB0 输出
		GPIOB->CRL|=0X0000000B; //复用功能输出
		GPIOB->CRL&=0XFFFFFF0F; //PB1 输出
		GPIOB->CRL|=0X000000B0; //复用功能输出
		GPIOA->CRL&=0X0FFFFFFF; //PA7 输出
		GPIOA->CRL|=0XB0000000; //复用功能输出

	
		RCC->APB2ENR|=1<<0; //开启辅助时钟

		AFIO->MAPR&=0XFFFFF3FF; //清除 MAPR 的[11:10]  没有重映像
		TIM3->ARR=arr; //设定计数器自动重装值
		TIM3->PSC=psc; //预分频器不分频
		

		TIM3->CCMR1|=7<<12; //CH2 PWM2 模式
		TIM3->CCMR2|=7<<4; //CH3 PWM2 模式
		TIM3->CCMR2|=7<<12; //CH4 PWM2 模式
		
		TIM3->CCMR1|=1<<11; //CH2 预装载使能
		TIM3->CCMR2|=1<<3; 	//CH3 预装载使能	
		TIM3->CCMR2|=1<<11; //CH4 预装载使能
		

		TIM3->CCER|=1<<4; //OC2 输出使能
		TIM3->CCER|=1<<8; //OC3 输出使能
		TIM3->CCER|=1<<12; //OC4 输出使能
		
		TIM3->CR1=0x0080; //ARPE 使能
		
		TIM3->CR1|=1 << 0; //使能定时器 1	
}
