#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED2 PBout(0) 
#define LED3 PBout(1) 
#define LED1 PAout(7) 
#define LED4 PBout(12)
#define LED5 PCout(13)


#define MOTER1_A PAout(6) //B1
#define MOTER1_B PAout(7) 

#define MOTER2_A PBout(7) //B4
#define MOTER2_B PBout(6)

#define MOTER3_A PAout(5) //B0
#define MOTER3_B PAout(4) 

#define MOTER4_A PBout(8)  //B5
#define MOTER4_B PBout(9)

void led_init(void);
void moter_init(void);


#endif


