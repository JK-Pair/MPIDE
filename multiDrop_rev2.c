#include <24FJ48GA002.h>
#include "BL_Support.h"
#use delay (internal = 8 MHz, clock = 32MHz)
#PIN_SELECT U1RX = PIN_B12 //PIN_B14 //
#PIN_SELECT U1TX = PIN_B13 //PIN_B15 //
#use rs232 (UART1, BAUD = 115200, XMIT = PIN_B13, RCV = PIN_B12)


char arrayData[4] = {};
char arrayDataXI[2] = {};
char arrayDataXII[2] = {};
char arrayDataYI[2] = {};
char arrayDataYII[2] = {};

char SM_id = 1;
int getPackage = 0;

void SM_RxD(int c){
	if (SM_id <= 2){
		if (c ==  0xFF){
			SM_id++;
		}else{
			SM_id = 1;
		}
	}else if (SM_id <= 3){
		if (c == 0x02){
			SM_id++;
		}
	}else if (SM_id <= 4){
		if ( c == 0x08){
			SM_id++;
		}
	}else if (SM_id > 4 && SM_id <= 6){
		arrayDataXI[SM_id - 5] = c;
		SM_id++;
	}else if (SM_id > 6 && SM_id <= 8){
		arrayDataYI[SM_id - 7] = c;
		SM_id++;
	}else if (SM_id > 8 && SM_id <= 10){
		arrayData[SM_id - 9] = c;
		SM_id++;
	}else if (SM_id > 10 && SM_id <= 12){
		arrayDataXII[SM_id - 11] = c;
		SM_id++;
	}else if (SM_id > 12 && SM_id <= 14){
		arrayDataYII[SM_id - 13] = c;
		SM_id++;
	}else if(SM_id > 14 && SM_id <= 16){
		arrayData[SM_id - 13] = c;
		SM_id++;
	}else if(SM_id > 16){
		getPackage = 1;
		SM_id = 1;
	}
}

#INT_RDA               // receive data interrupt one time per one 
void UART1_Isr() {
    int c = getc();
    SM_RxD(c);
}
void main(){
	disable_interrupts(GLOBAL);
	
    clear_interrupt(INT_RDA);   // recommend style coding to confirm everything clear before use
    enable_interrupts(INT_RDA);
    printf("\nresult = %d\n", arrayData[3]);
	enable_interrupts(GLOBAL);
	while(TRUE){
		if (getPackage >= 1){
			getPackage = 0;
			int bagPosX;//, bagPosY, goPosX, goPosY ;
			memcpy(&bagPosX, arrayDataXI, sizeof(bagPosX));
			//printf("\nresult = %d\n", array[0]);
			printf("\nresult = %d\n", bagPosX);
			//printf("\nresult = %d\n", arrayData[0]);
			//printf("\nresult = %d\n", arrayData[1]);
			//printf("\nresult = %d\n", arrayData[2]);
			//printf("\nresult = %d\n", arrayData[3]);
		}
	}
}

