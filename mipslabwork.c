
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

#include <stdbool.h>

#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200
#define temp_offset 3

#include <stdio.h>
  int main() {
	  
	  int win = 0, count = 0;
	  int squares [9], index, sign, turn, flag, i, k, j;
	  for (i = 0; i<9; i++)
		  data[i] = '';
	  while (count < 9) {
                flag = 0;
                system("clear");
                printf("\n\n");
		    printf("\t\t\t  %c | %c  | %c  \n", data[0], data[1], data[2]);
                printf("\t\t\t----+----+----\n");
                printf("\t\t\t  %c | %c  | %c  \n", data[3], data[4], data[5]);
                printf("\t\t\t----+----+---\n");
                printf("\t\t\t  %c | %c  | %c  \n", data[6], data[7], data[8]);
                if (count % 2 == 0) {
                        sign = 'X';
                        player = 1;
                } else {
			sign = 'O';
			player = 0;
			
                 }
		  
	  }



//display_debug(&test);  used to print adress+ value of var on chipkit
  

