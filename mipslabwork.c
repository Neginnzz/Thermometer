/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog 

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */
#include <IOShieldTemp.h>
#include <IOShieldOled.h>
#include <Wire.h>
char textstring[] = "text, more text, and even more text!";

int format = 0;
int temperature;


void light (void){
    static int counter = 1;
    volatile int* pointer = (volatile int*) 0xbf886110;
    *pointer = counter;
    counter++;
    return;
}
/* Interrupt Service Routine */
void user_isr( void )
 {  
	static int timeoutcounter = 0;


	// this is the interrupts for counting
	//int buttonstatus = getbtns();
	
	if(IFS(0)& 0x100){
	   
		IFSCLR(0) = 0x100;
		timeoutcounter++;
		if(timeoutcounter == 10){
			timeoutcounter = 0;
			
			
			//rewrite time2string to handle temperature
			time2string( textstring, mytime );
		
		display_string( 3, textstring );
	display_update();
	tick( &mytime );
	
	
}

int get_temperature(void){
	int tempC;
	tempC = IOShieldTemp.getTemp();
	return tempC;
}
int temp_F(temp){
	temp = IOshieldTemp.convCtoF(temp)
	return temp;
}

int temp_K(temp){
	temp += 273;
	return temp;
}
void timer(void){
	T2CON =0x0070;
	PR2 = 31250;
	TMR2 = 0;
	IECSET(0) = 0x100;
	IPCSET(2)  =0x5;
	T2CONSET= 0x8000;
	
	return;
}
/* Lab-specific initialization goes here */
void labinit( void )
{
    volatile int* pointer = (volatile int*) 0xbf886110;
    int mask = 0xFF;
    mask = ~mask; //~ betyder 2:nd komplement 
    *pointer = *pointer & mask;
    TRISDSET = 0xFE0;
	 
	 
	timer();
	//temperature
	Serial.begin(9600);
	IOShieldTemp.config(IOSHIELDTEMP_ONESHOT | IOSHIELDTEMP_RES11);
	
	enable_interrupt();
	
	
	
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
	//check syntax for getTemp and switch!!!
	temperature = getTemp();
	switch (temperature){
	case 1
	temperature = temp_F(temperature);
	break;
	case 2
	temperature = temp_K(temperature);
	break;
	}
	
return;
		
  } 
 
  //display_debug(&test);  used to print adress+ value of var on chipkit
  

