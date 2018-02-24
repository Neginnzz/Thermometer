#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"




int getsw (void){
	int switchstatus = PORTD >> 8;
	int mask = 0xF;
    switchstatus = switchstatus & mask;
	return switchstatus;
}

	int getbtns(void){
	int buttonstatus = PORTD >> 5;
	int mask = 0x7;
	buttonstatus = buttonstatus & mask;
	return buttonstatus;
}

int buttontime(int buttons, int switches, int time) {
	int mask = 0xf;
	if(buttons & 0x4){
		time = (switches << 12) | (time & 0x0FFF);
	} 
    
	if(buttons & 0x2){
		time = (switches << 8) | (time & 0xF0FF);
	}
    
    if(buttons & 0x1){
		time = (switches << 4) | (time &0xFF0F);
	}
	
	return time;
   
} 