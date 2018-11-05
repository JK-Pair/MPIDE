#include <24FJ48GA002.h>
#include "BL_Support.h"
#include <math.h>
#use delay (internal = 8 MHz, clock = 32MHz)
#PIN_SELECT U1RX = PIN_B12 //PIN_B14 //
#PIN_SELECT U1TX = PIN_B13 //PIN_B15 //
#use rs232 (UART1, BAUD = 115200, XMIT = PIN_B13, RCV = PIN_B12)

#define TIME_PERIOD 2000
#define tolerance 24

#define servoI   PIN_B14
#define	servoII  PIN_B15

#define	lim_SW_X PIN_B4 //10
#define lim_SW_Y PIN_B4
#define lim_SW_Z PIN_B5

#define motorXI  PIN_B0 
#define motorXII PIN_B1
#define ena_X    PIN_B2 
//Encoder X is INT0 PIN_B7

#define	motorYI  PIN_A2
#define motorYII PIN_A4
#define ena_Y    PIN_B9
#define	encodeY  PIN_B6

#define	motorZI  PIN_A0 //B1
#define motorZII PIN_A1 //B10
#define ena_Z    PIN_B8
#define	encodeZ  PIN_B3
#define position 1 //receive from python for direction of Motor. if 1 = unti-clockwise

#PIN_SELECT INT1 = encodeY //EncoderY
#PIN_SELECT INT2 = encodeZ //EncoderZ
#PIN_SELECT OC1 =  ena_X  
#PIN_SELECT OC2 =  ena_Y
#PIN_SELECT OC3 =  ena_Z
#PIN_SELECT OC4 =  servoI
#PIN_SELECT OC5 =  servoII

int countX = 0;
int countY = 0;
int countZ = 0; 
int stateEncode_X = 0;
int stateEncode_Y = 0;
int stateEncode_Z = 0;
int stateSWII = 0;
int state = 0;

void control_inputX(int input, int direc){
	if(direc == 0){
		if(input >= 743 ){
			set_pwm_duty(1, 2000);
			output_bit(motorXI,1); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorXII,0);
		}else if(input > 0 ){
			set_pwm_duty(1, (int)((input/743.0f)*2000));
			output_bit(motorXI,1); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorXII,0);
		}else{
			set_pwm_duty(1, 0);
			output_bit(motorXI,0);
			output_bit(motorXII,0);
			}
	}	
	else if(direc == 1){
		if(input >= 743 ){
			set_pwm_duty(1, 2000);
			output_bit(motorXI,0); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorXII,1);
		}else if(input > 0 ){
			set_pwm_duty(1, (int)((input/743.0f)*2000));
			output_bit(motorXI,0); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorXII,1);
		}else{
			set_pwm_duty(1, 0);
			output_bit(motorXI,0);
			output_bit(motorXII,0);
		}
	}		
}


void control_inputY(int input, int direc){
	
if(direc == 0){
		if(input >= 743 ){
			set_pwm_duty(2, 2000);
			output_bit(motorYI,1); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorYII,0);
		}else if(input > 0 ){
			set_pwm_duty(2, (int)((input/743.0f)*2000));
			output_bit(motorYI,1); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorYII,0);
		}else{
			set_pwm_duty(2, 0);
			output_bit(motorYI,0);
			output_bit(motorYII,0);
			}
	}	
	else if(direc == 1){
		if(input >= 743 ){
			set_pwm_duty(2, 2000);
			output_bit(motorYI,0); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorYII,1);
		}else if(input > 0 ){
			set_pwm_duty(2, (int)((input/743.0f)*2000));
			output_bit(motorYI,0); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorYII,1);
		}else{
			set_pwm_duty(2, 0);
			output_bit(motorYI,0);
			output_bit(motorYII,0);
			
		}		
	}
}

void control_inputZ(int input, int direc){
	if(direc == 0){
		if(input >= 743 ){
			set_pwm_duty(3, 2000);
			output_bit(motorZI,1); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorZII,0);
		}else if(input > 3 ){
			set_pwm_duty(1, (int)((input/743.0f)*2000));
			output_bit(motorZI,1); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorZII,0);
		}else{
			set_pwm_duty(3, 0);				
			output_bit(motorZI,0);
			output_bit(motorZII,0);
				}
	}	
	else if(direc == 1){
		if(input >= 743 ){
			set_pwm_duty(3, 2000);
			output_bit(motorZI,0); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorZII,1);
		}else if(input > 0 ){
			set_pwm_duty(3, (int)((input/743.0f)*2000));
			output_bit(motorZI,0); //if 0, IN2->AXI@2 = clockwise 
			output_bit(motorZII,1);
		}else{
			set_pwm_duty(3, 0);
			output_bit(motorZI,0);
			output_bit(motorZII,0);
				
			}		
		}
	}

void move_posX(int pulse_x, int direc){
	int check = 1;
	while(check == 1){
		int error = pulse_x - countX;
		if(abs(error) <= tolerance){
			control_inputX(0,direc);
			check = 0;	
		}else{
		//	if(stateSWII == 0){
			control_inputX(error, direc); 
		//	}check = 0;
		}
	}
}

void move_posY(int pulse_y, int direc){
	int check = 1;
	while(check == 1){
		int error = pulse_y - countY;
		if(abs(error) <= tolerance){
			control_inputY(0, direc);
			check = 0;	
		}else{
			if(stateSWII == 2){
			control_inputY(error, direc); 
			}
		}
	}
}

void move_posZ(int pulse_z, int direc){
	int check = 1;
	while(check == 1){
	int error = pulse_z - countZ;
		if(abs(error) <= tolerance){
			control_inputZ(0, direc);
			check = 0;	
		}else{
			if(stateSWII == 1){
				control_inputZ(error, direc);
			}check = 0;	 
		} 
	}
}

void set_Zero(void){
	int loop = 1;
	while(loop == 1){
		if(stateSWII == 0){
			if(input(lim_SW_X) == 1){
				control_inputX(0,0);
				stateSWII++;
			}else if(input(lim_SW_X) == 0){
				move_posX(5000,0);
			}
		}else if(stateSWII == 1){  //X-axial
			if(input(lim_SW_Z) == 1){
				control_inputZ(0,1);
				stateSWII++;
				loop = 0;
			}else {
			move_posZ(7000,1);
			}
		}
	}
}

void move_XYZ(int z){
	if(state == 0){
		move_posZ(z,0);
	}
	/*else if(state == 1){
		move_posY(y);
		state++;
	}
	else if(state == 2){
		move_posX(x);
		state++;
	}*/
}

#INT_EXT0
void INT_EXT_INPUT0 (void)
{	
	if(stateEncode_X == 0){
		countX++;
	}else{countX--;}
}
#INT_EXT1
void INT_EXT_INPUT1 (void)
{	
	if(stateEncode_Y == 0){
		countY++;
	}else{countY--;}
}
#INT_EXT2
void INT_EXT_INPUT2 (void)
{	
	if(stateEncode_Z == 0){
		countZ++;
	}else{countZ--;}
}
void Init_Interrupts()
{
	disable_interrupts(GLOBAL);

	enable_interrupts( INT_EXT0 );
	ext_int_edge( 0, L_TO_H ) ;

	enable_interrupts( INT_EXT1 );
	ext_int_edge( 1, L_TO_H ) ;

	enable_interrupts( INT_EXT2 );
	ext_int_edge( 2, L_TO_H ) ;

	setup_timer3(TMR_INTERNAL | TMR_DIV_BY_8 ,TIME_PERIOD);
	
	enable_interrupts(GLOBAL);

}

int main(){
	/*char stateSW = 0;
	while((int)stateSW == 0){
		if(input(PIN_B5) == 0){
			stateSW = 1;
		}
	}*/
	Init_Interrupts();

	setup_compare(1, COMPARE_PWM | COMPARE_TIMER3);
	setup_compare(2, COMPARE_PWM | COMPARE_TIMER3);
	setup_compare(3, COMPARE_PWM | COMPARE_TIMER3);

	while(TRUE){
			//set_Zero(); // X is Y, Z is x
			move_posX(3000,1); //if = 1 ,y is left 	: if X_Axial = 0 is forward and if = 1 z_Axial is down
			printf("X : %d\n",countY);
	}return 0;
}
