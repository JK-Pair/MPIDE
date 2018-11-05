#include <24FJ48GA002.h>
#device ADC=10
#include "BL_Support.h"
#include <math.h>

#use delay (internal = 8 MHz, clock = 32MHz)
#PIN_SELECT U1RX = PIN_B12 //PIN_B14 //
#PIN_SELECT U1TX = PIN_B13 //PIN_B15 //
#use rs232 (UART1, BAUD = 9600, XMIT = PIN_B13, RCV = PIN_B12)
#define tolerance 5
int adc_data;
float e =0.0, u = 0.0;
int kp = 0.4;
#PIN_SELECT OC1 = PIN_B2
#PIN_SELECT OC2 = PIN_B3


void control_input(int input){
	if(input >= 100 ){
		set_pwm_duty(1, 200);
		set_pwm_duty(2, 0);
	}else if(input > 0 ){
		set_pwm_duty(1, (int)(((100-input)/100.0f)*200));
		set_pwm_duty(2, 0);
	}else if(input <= -100){
		set_pwm_duty(1, 0);
		set_pwm_duty(2, 200);
	}else if(input < 0){
		set_pwm_duty(1, 0);
		set_pwm_duty(2, (int)(((input+100)/100.0f)*200));
	}else{
		set_pwm_duty(1, 0);
		set_pwm_duty(2, 0);
	}delay_ms(10);	
		
}

void init_adc(){
	setup_adc_ports(sAN3, VSS_VDD);
	setup_adc(ADC_CLOCK);
	set_adc_channel(3);
	delay_us(10);
}


void main(){
	disable_interrupts(GLOBAL);
	setup_timer2(TMR_INTERNAL | TMR_DIV_BY_8, 200);	
	setup_timer3(TMR_INTERNAL | TMR_DIV_BY_8, 200);
	enable_interrupts(GLOBAL);

	setup_compare(1, COMPARE_PWM | COMPARE_TIMER2);
	setup_compare(2, COMPARE_PWM | COMPARE_TIMER3);
	init_adc();
	adc_data = 0;
	while(TRUE){
		adc_data = read_adc();
		e = 512 - adc_data;
		s = s + e;
		if(abs(e) > tolerance){
			u = (kp*e);
		}else{
			u = 0;
		}
		control_input(u);
		p = e;
		printf("AN3: %d\r\n", read_adc());

	}
}