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

int mytime = 0x5957;
int prime = 1234567;

char textstring[] = "text, more text, and even more text!";
int timeoutcounter = 0;



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
//IFSCLR(0) = 0x8000;
  //IFSCLR(0) = 0x100;
	//this is the interrupts for the switch
	if(IFS(0) & 0x8000){
		light();
		IFSCLR(0) = 0x8000;
	} 

	// this is the interrupts for counting
	//int buttonstatus = getbtns();
	
	if(IFS(0)& 0x100){
	   
		IFSCLR(0) = 0x100;
		timeoutcounter++;
		if(timeoutcounter == 10){
			timeoutcounter = 0;
		
			time2string( textstring, mytime );
			/*if(buttonstatus){
				int switchstatus = getsw();
				int time = buttontime(buttonstatus, switchstatus, mytime);
				mytime = time;
			} */
		display_string( 3, textstring );
	display_update();
	tick( &mytime );
	
	
	
		}
	
	} 
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
void switch_enable(void){
	IFSSET(0) = 0x8000;
	IECSET(0) = 0x8000;
	IPCSET(3) = 0xA000000;
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
	switch_enable();
	
	
	enable_interrupt();
	
	
	
  return;
}

/* This function is called repetitively from the main program */
void labwork( void )
{
	
	prime = nextprime( prime );
	display_string( 0, itoaconv( prime ) );
	display_update();
		
  } 
 
  //display_debug(&test);  used to print adress+ value of var on chipkit
  

