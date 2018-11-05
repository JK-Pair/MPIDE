#include <24FJ48GA002.h>
#include "BL_Support.h"
#use delay (internal = 8 MHz, clock = 32MHz)
#PIN_SELECT U1RX = PIN_B12 //PIN_B14 //
#PIN_SELECT U1TX = PIN_B13 //PIN_B15 //
#use rs232 (UART1, BAUD = 9600, XMIT = PIN_B13, RCV = PIN_B12)

#define numSwt 4
unsigned int swtCount[numSwt]={30,15,10,6};
unsigned int refill[numSwt]={30,15,10,6};
boolean swtFlag[numSwt]={FALSE,FALSE,FALSE,FALSE};

typedef void (*SWTCallback)(void);
SWTCallback fp_ta[numSwt]={0,0,0,0};

void SystemTick(void){
	unsigned int i;
	for(i=0;i<numSwt;i++){
		if(swtCount[i]>0){
			swtCount[i]--;
			if(swtCount[i]==0){
				swtFlag[i]=TRUE;
				swtCount[i]=refill[i];
			}
		}
	}
}
#INT_TIMER1
void TIMER1_ISR(){
	SystemTick();
}
int a,b,c,d,res;
void SWTService(void){
	unsigned int i;
	for(i=0;i<numSwt;i++){
		if(swtFlag[i]==TRUE){
			swtFlag[i]=FALSE;
			if(fp_ta[i]!=0){
				(*fp_ta[i])();
			}
		}
	}
}
void task_led0(void){
	res=a+b*2+c*4+d*8;
	printf("status=%X\n",res);

	if (a==0){
		a=1;
	}
	else if (a==1){
		a=0;
	}
	output_bit(PIN_A2,a);	
}
void task_led1(void){
	if (b==0){
		b=1;
		}
	else if (b==1){
		b=0;
	}
	output_bit(PIN_A4,b);
}
void task_led2(void){
	if (c==0){
		c=1;
	}
	else if (c==1){
		c=0;
	}
	output_bit(PIN_B2,c);
}
void task_led3(void){
	if (d==0){
		d=1;
	}
	else if (d==1){
		d=0;
	}
	output_bit(PIN_B3,d);
}

void Init_Timer1(){
	setup_timer1(TMR_INTERNAL| TMR_DIV_BY_8 ,33333);
	enable_interrupts(INT_TIMER1);
}
void main(void){
	a=0;
	b=0;
	c=0;
	d=0;
	disable_interrupts(GLOBAL);
	Init_Timer1();	
	enable_interrupts(GLOBAL);

	fp_ta[0]= &task_led0;
	fp_ta[1]= &task_led1;
	fp_ta[2]= &task_led2;
	fp_ta[3]= &task_led3;
	while(TRUE){

		SWTService();
	}
}