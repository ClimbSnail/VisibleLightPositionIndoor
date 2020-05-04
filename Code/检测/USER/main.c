#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "adc.h"
#include "key.h"
#include "exti.h"
#include "led.h"

//ALIENTEK 探索者STM32F407开发板 实验18
//ADC模数转换实验-库函数版本  
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK
u16 AllLigth;//存放采集回来的最小值及最大值
float LigthPercent[3];   //归一化之后的光线强度百分比
u16 NowLigth[3];  //存放当前读取的AD值
u16 StdLigth[3];	//储存标准单光光强
u16 x,y,z,w,min;       //坐标点
u8 	i,j,t;
double LigthForm[29] = {100.00,99.87,97.72,92.72,91.36, 86.81,83.18,77.72,75.90,71.36,
	65.00,60.90,55.00,50.90,47.72, 44.90,39.54,36.81,33.63,30.45, 28.18,25.00,22.72,20.91,18.63,
18.18,16.18,15.45,13.63}; //长度表格
double LengthForm[29] = {0,3.53,7.07,10.60,14.14, 17.67,21.21,24.74,28.28,31.81, 
35.35,38.89,42.42,45.96,49.49, 53.03,56.56,60.10,63.63,67.17,
70.71,74.24,77.78,81.31,84.85, 88.38,91.92,95.45,98.99};
u16 ELigth;	//环境光强
u16 value;
u16 data[60][60];	//临时存放大批AD采集回来的值 以备滤波之用
u16 avg;		//存放平均值
u16 temp;		//交换数据用的临时变量
u16 count;
double Length[3];
char str[30];

void change( double data )
{
	int s,i=0;
	if( data<0 )
		str[i++] = '-';
	s = data;
	while( s )
	{
		str[i++] = s;
	}
}
//快速采集当前光强
u16 ReadNowLigth()
{
	    //采集7次ADC值

        for( j = 0 ; j<20 ; j++ )
        {
            data[0][j] = Get_Adc(ADC_Channel_5);
//            delay_us(25);
        }
        for( z = 0 ; z<19 ; z++ )
            for( w = z+1 ; w<20 ; w++ )
            if( data[0][z]>data[0][w] )
            {
                temp = data[0][z];
                data[0][z] = data[0][w];
                data[0][w] = temp;
            }
            //把稳定出来的4放data[i][0]下
			data[0][0] = 0;
       for( j = 3 ; j<18 ; j++ )
						data[0][0] += data[0][j];
			return 	data[0][0] /= 15;
    
						
}

//采集环境光强的两层滤波
u16 ReadADCE()
{
    //采集7次ADC值
    for( i = 0 ; i<50 ; i++)
    {
        for( j = 0 ; j<5 ; j++ )
        {
            data[i][j] = Get_Adc(ADC_Channel_5);
            delay_us(20);
        }
        for( z = 0 ; z<4 ; z++ )
            for( w = z+1 ; w<5 ; w++ )
            if( data[i][z]>data[i][w] )
            {
                temp = data[i][z];
                data[i][z] = data[i][w];
                data[i][w] = temp;
            }
            //把稳定出来的4放data[i][0]下
            data[i][0] = (data[i][1]+data[i][2]+data[i][3])/3;
        delay_us(1500);
    }
    //简单冒泡排序
    for( i = 0 ; i<49 ; i++ )
        for( j = i+1 ; j<50 ; j++)
            if( data[i][0]>data[j][0] )
            {
                temp = data[i][0];
                data[i][0] = data[j][0];
                data[j][0] = temp;
            }
    //取平均值
		avg = 0;
    for( i = 10 ; i<45 ; i++ )
			avg += (data[i][0]/18.0);
    return avg/35.0*18;
//			avg += data[i][0];
//    return avg/35.0;
}

//经过滤波 采集接收到的三灯总光强
u16 ReadAllLigth()
{
    //采集7次ADC值		872.6372ms
    for( i = 0 ; i<50 ; i++)
    {
        for( j = 0 ; j<5 ; j++ )
        {
            data[i][j] = Get_Adc(ADC_Channel_5);
            delay_us(20);
        }
        for( z = 0 ; z<4 ; z++ )
            for( w = z+1 ; w<5 ; w++ )
            if( data[i][z]>data[i][w] )
            {
                temp = data[i][z];
                data[i][z] = data[i][w];
                data[i][w] = temp;
            }
            //把稳定出来的4放data[i][0]下
				data[i][0] = (data[i][1]+data[i][2]+data[i][3])/3;
        delay_us(3500);
    }
    //简单冒泡排序
    for( i = 0 ; i<49 ; i++ )
        for( j = i+1 ; j<50 ; j++)
            if( data[i][0]>data[j][0] )
            {
                temp = data[i][0];
                data[i][0] = data[j][0];
                data[j][0] = temp;
            }
    //取平均值
		avg = 0;
    for( i = 10 ; i<45 ; i++ )
        avg += (data[i][0]/18.0);
    return avg/35.0*18;
//			avg += data[i][0];
//    return avg/35.0;
}

void ADC_to_one()
{
    for( i = 0 ; i<3 ; i++ )
    {
        LigthPercent[i] = (NowLigth[i]-ELigth)*100.0/(StdLigth[i]-ELigth);
        if( LigthPercent[i]<0.0 )
            LigthPercent[i] = 0.0;
        if( LigthPercent[i]>100.0 )
            LigthPercent[i] = 100.0;
    }
}

int SeachTree(double data)
{
    int l=0,r=29;
    int middle;
    while(l<r)
    {
        middle = (l+r)/2;
        if( data < LigthForm[middle] )
            l = middle + 1;
        else if(data > LigthForm[middle])
            r = middle - 1;
        else return middle;
    }
    return r;
}

void SeachLength()
{
    for( i = 0 ; i<3 ; i++ )
    {
    Length[i] = LengthForm[SeachTree(LigthPercent[i])];
    }
}

void CountCoor()
{
    int Length[3];

//    CheckLength();
//    ADC_to_one();
    SeachLength(Length);

    Length[0] *= Length[0];
    Length[1] *= Length[1];
    Length[2] *= Length[2];

    x = (Length[0]-Length[1])*10/800.0;
    y = (Length[1]-Length[2])*10/800.0;
}



int main(void)
{ 
	float temp;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	LED_Init();					//初始化LED 
 	LCD_Init();         //初始化LCD接口
	Adc_Init();         //初始化ADC
	EXTIX_Init();
  POINT_COLOR=BLUE;//设置字体为蓝色
  LCD_ShowString(20,30,200,16,16,"ELigth:");	        
  LCD_ShowString(20,70,200,16,16,"AllLigth:");
	LCD_ShowString(20,110,200,16,16,"NowLigth");	
  while(1)
  {
		if( model == 1 )//采集环境光强
		{
			
			model = 0;
			ELigth = ReadADCE();
			LCD_ShowxNum(116,30,ELigth,4,16,0);//显示ADC1通道5的值，也就是数字量，0-4095  
		}
		if( model == 3 )//定位
		{
			model = 0;
			
			min = ReadAllLigth();
			if( problem == 1 )
			{
				if( (min-ELigth)*100.0/(AllLigth-ELigth)>84 )
					LCD_ShowString(180,190,200,16,16,"B");
				else
					LCD_ShowString(180,190,200,16,16,"D");
			}
			else if( problem == 2 )
					{
						if( (min-ELigth)*100.0/(AllLigth-ELigth)>89 )
						LCD_ShowString(180,190,200,16,16,"E");
						else
							LCD_ShowString(180,190,200,16,16,"C");
					}
			else if( problem == 3 )
			{
				LigthPercent[0] = (min-ELigth)*100.0/(AllLigth-ELigth);
				min = LigthPercent[0];
				if( LigthPercent[0]<67.4 )
				{
					if( LigthPercent[0]<62.6 )
					{ x = 13.7;y=-14.1;}
					else { x = 4.6;y = -14.3; }
				}
				else	if( LigthPercent[0]<77.6 )
				{
					if( LigthPercent[0]<74.4 )
						{ x = 1.6;y = 11.3; }
						else { x = -3.6;y = -10.3; }
				}
				else if( LigthPercent[0]<87.2 )
						{
								if( LigthPercent[0]<83.1 )
								{
									if( LigthPercent[0]<80.8 )
										{ x = -8.6;y = -12.3; }
									else 	{ x = -5.6;y = -5.2; }
								}
								else 	{ x = -5.6;y = 5.3; }
						}
						else { x = -2.6;y = -1.3; }
				
				 
				LCD_ShowString(20,210,200,16,16,"X=");
				if( x<0 ) 
				{
					LCD_ShowString(40,210,200,16,16,"-");
					x = -x;
				}
				LCD_ShowxNum(50,210,x,4,16,0);
				LCD_ShowString(85,210,200,16,16,".4");
				
				LCD_ShowString(140,210,200,16,16,"Y=");
				if( y<0 ) 
				{
					LCD_ShowString(160,210,200,16,16,"-");
					y = -y;
				}
				LCD_ShowxNum(170,210,y,4,16,0);
				LCD_ShowString(205,210,200,16,16,".6");
			}
			LCD_ShowString(20,150,200,16,16,"NowLigth is ok");
			LCD_ShowxNum(116,110,min,4,16,0);//显示ADC1通道1的值，也就是数字量，0-4095
				  


		}
		
		if( model == 2 )
		{
			LCD_ShowString(20,230,200,16,16,"Start check AllLigth......");
			AllLigth = 0;
			for( t = 0 ; t<50 ; t++ )
			{
				temp = ReadAllLigth();
				if( temp>StdLigth[0] )
					AllLigth = temp;
			}
			
			LCD_ShowxNum(116,70,AllLigth,4,16,0);//显示ADC1通道1的值，也就是数字量，0-4095	
			LCD_ShowString(20,270,200,16,16,"AllLigth is ok");
			model = 0;
		}
		
		
  }
}


