#include "sys.h"
#include "delay.h"
#include "led.h"
#include "timer.h"
#include "oled.h"
#include "usart.h"	
#include "key.h"
int main(void)
{
	Stm32_Clock_Init(9); 
	delay_init(72); 
	led_init();
//	TIM3_PWM_Init(1999,0);    //36KHZ
//	LCD_Init();
//	KEY_Init();
	EXTIX_Init();
	TIM2_Int_Init(9999,7199);
//	LCD_Print(100,5,"6E");
//	uart_init(72,115200);
	start = 0;
  LED4 = 1;
	LED1=0;
	LED2=0;
	LED3=0;
//	delay_us(1000);
	while(1)
	{
//		if( start == 1 )
//		{
//			start = 0;//标志位清零
//			
//			LED1 = 1;	//只亮LED1一颗灯1ms
//			LED2 = 0;
//			LED3 = 0;
//			delay_us(2000);
////			delay_ms(1000);
//			
//			LED2 = 1;		//灯全亮200ms
//			LED3 = 1;
//			delay_ms(300);
//			
//			LED1 = 0;		//只亮LED2一颗灯1ms
//			LED3 = 0;
//			delay_us(2000);
////			delay_ms(1000);			
//						
//			LED1 = 1;		//灯全亮200ms
//			LED3 = 1;
//			delay_ms(300);
//			
//			LED1 = 0;		//只亮LED3一颗灯1ms
//			LED2 = 0;		
//			delay_us(2000);
////			delay_ms(1000);			
//			
//			LED1 = 1;
//			LED2 = 1;
//		}

		if( start == 1 )
		{
			start = 0;//标志位清零
			
			LED1 = 1;	//只亮LED1一颗灯1ms
			LED2 = 0;
			LED3 = 0;
			delay_us(1500);
//			delay_ms(1000);
			
			LED1 = 0;		//只亮LED2一颗灯1ms
			LED2 = 1;
//			LED3 = 0;
			delay_us(1500);
//			delay_ms(1000);			
						
			
//			LED1 = 0;		//只亮LED3一颗灯1ms
			LED2 = 0;	
			LED3 = 1;
			delay_us(1500);
//			delay_ms(1000);			
			
			LED1 = 1;
			LED2 = 1;
		}
	}

}
	

	
