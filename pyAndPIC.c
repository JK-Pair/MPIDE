#include <24FJ48GA002.h>
#include "BL_Support.h"
#include <math.h>
#use delay (internal = 8 MHz, clock = 32MHz)
#PIN_SELECT U1RX = PIN_B12 //PIN_B14 //
#PIN_SELECT U1TX = PIN_B13 //PIN_B15 //
#use rs232 (UART1, BAUD = 115200, XMIT = PIN_B13, RCV = PIN_B12)

#define INPUT_VOLTAGE 12
#define TIME_PERIOD 20000 //10 HZ

#PIN_SELECT INT1 = PIN_B7 //EncoderA

#PIN_SELECT OC1 = PIN_B2
#PIN_SELECT OC2 = PIN_B3

//B5 SW 

int count = 0;
float time =0.0;
float u;

float ChirpSine(float time){
	float signal = sin(0.5 * (time * time)) * INPUT_VOLTAGE;
	return signal;
}

float convertVoltToPwm(float volt){
	return (volt * 100.0f) / INPUT_VOLTAGE;
}

void driveMotor(int direction, float pwm){
	if(direction == 1){
		set_pwm_duty(1, pwm);
		set_pwm_duty(2, 0);
	}else{
		set_pwm_duty(1, 0);
		set_pwm_duty(2, pwm);
	}		
} 


#INT_EXT1
void INT_EXT_INPUT1 ( void )
{
	if(input(PIN_B6) == 0){
		count --;
	}else{
		count ++;}	
	
}
#INT_TIMER3
void Timer3_ISR(void){
	time += 0.01;
	u = ChirpSine(time);
	boolean direction = (u > 0.0) ? 1:0;
	float pwm = convertVoltToPwm(u);
	driveMotor(direction, pwm);
	printf("%d , %d , %d\r\n", (int)(time * 1000), (int)(u * 1000), count);
}

void Init_Interrupts()
{
	disable_interrupts(GLOBAL);

	enable_interrupts( INT_EXT1 );
	ext_int_edge( 1, L_TO_H ) ;

	setup_timer3(TMR_INTERNAL | TMR_DIV_BY_8 ,TIME_PERIOD);
	enable_interrupts(INT_TIMER3);
	
	enable_interrupts(GLOBAL);

}


int main(){
	
	char stateSW = 0;
	while((int)stateSW == 0){
		if(input(PIN_B5) == 0){
			stateSW = 1;
		}
}
	Init_Interrupts();
	
	setup_compare(1, COMPARE_PWM | COMPARE_TIMER3);
	setup_compare(2, COMPARE_PWM | COMPARE_TIMER3);
	
	while(TRUE){
	}return 0;
}
