#include <stdint.h>   
#include <pic32mx.h>  
#include "mipslab.h"

double Temperatureshift(int memadr) {
	while(getbtns() == 0){
		getbtns();
	}
	int buttonstatus = getbtns();
	int *currentlocation = getlocation(buttonstatus);
	
	
	
	
	
	
	
	
int getlocation(int btnstats){
	
	
	
	
	
int getbtns(void){
	int buttonstatus = PORTD >> 5;
	int mask = 0x7;
	buttonstatus = buttonstatus & mask;
	return buttonstatus;
}

/*
int getsw (void){
	int switchstatus = PORTD >> 8;
	int mask = 0xF;
    switchstatus = switchstatus & mask;
	return switchstatus;
}*/