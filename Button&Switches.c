#include <stdint.h>   
#include <pic32mx.h>  
#include "mipslab.h"

double Temperatureshift(void) {
	double temp = get_temperature();
	while(getbtns() == 0){
		getbtns();
	}
	int buttonstatus = getbtns();
	if (buttonstatus == 8) {
		double Fahrenheit = temp_F(temp);
		return Fahrenheit;
	}
	if(buttonstatus == 4) {
		return temp;
	}
	if (buttonstatus == 2) {
		double Kelvin = temp_K(temp);
		return Kelvin;
	}
    if(buttonstatus == 1) {
		
	
	
	
int getbtns(void){
	int buttonstatus = PORTD >> 4; //Vill fånga upp btnstats för alla knappar
	int mask = 0xF; //Masken skall ej ignorera någon bit inom en nibble
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