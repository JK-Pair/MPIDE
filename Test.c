#include <24FJ48GA002.h>
#include <BL_Support.h>
#use delay (internal = 8 MHz, clock = 32 MHz)
#PIN_SELECT U1RX = PIN_B12 //PIN_B14//
#PIN_SELECT U1TX = PIN_B13 //PIN_B13//
#use rs232 (UART1, BAUD = 9600, xmit = PIN_B13, rcv = PIN_B14)

#define NUM_SWT 4
unsigned int swt_count[NUM_SWT]={30, 15, 10, 6};
boolean swt_flag[NUM_SWT] ={FALSE, FALSE, FALSE, FALSE};

typedef void (*SWTCallback)(void);
SWTCallback fp_ta[NUM_SWT] = {0, 0, 0, 0};

void SystemTick(void){
	unsigned int i;
	for(i=0;i<NUM_SWT; i++){
		if(swt_count[i] > 0){
			swt_count[i]--;
			if(swt_count[i] == 0) {
				swt_flag[i] = TRUE;
			}
		}
	}
}

#INT_TIMER1
void TIMER1_ISR(){
	SystemTick();
}

void SWTService(void){
	unsigned int i;
	for(i = 0;i<NUM_SWT;i++){
		if(swt_flag[i] == TRUE){
			swt_flag[i] = FALSE;
			if(fp_ta[i] != 0){
				(*fp_ta[i])();
			}
		}
	}
}

void LED0(void){
	output_toggle(PIN_A2);
}

void LED1(void){
	output_toggle(PIN_A4);
}

void LED2(void){
	output_toggle(PIN_B2);
}
void LED3(void){
	output_toggle(PIN_B3);
}

void Init_Timer1(){
	setup_timer1(TMR_INTERNAL | TMR_DIV_BY_8, 33333);
	enable_interrupts(INT_TIMER1);
}

void main(void){
	disable_interrupts(GLOBAL);
	Init_Timer1();
	enable_interrupts(GLOBAL);

	fp_ta[0] = &LED0;
	fp_ta[1] = &LED1;
	fp_ta[2] = &LED2;
	fp_ta[3] = &LED3;

	while(TRUE) {
		SWTService();	
	}
}