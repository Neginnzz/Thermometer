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

/* Address of the temperature sensor on the I2C bus */
#define TEMP_SENSOR_ADDR 0x48

/* Temperature sensor internal registers */
typedef enum TempSensorReg TempSensorReg;
enum TempSensorReg {
    TEMP_SENSOR_REG_TEMP,
    TEMP_SENSOR_REG_CONF,
    TEMP_SENSOR_REG_HYST,
    TEMP_SENSOR_REG_LIMIT,
};
char textstring[] = "text, more text, and even more text!";

// for printouts
char *s, *t;

int format = 0;
int pause = 0;
uint32_t strlen(char *str) {
    uint32_t n = 0;
    while (*str++)
        n++;
    return n;
}

// translates temp to string
char *fixed_to_string(uint16_t num, char *buf) {
    bool neg = false;
    uint32_t n;
    char *tmp;

    if (num & 0x8000) {
        num = ~num + 1;
        neg = true;
    }

    buf += 4;
    n = num >> 8;
    tmp = buf;
    do {
        *--tmp = (n % 10) + '0';
        n /= 10;
    } while (n);
    if (neg)
        *--tmp = '-';

    n = num;
    if (!(n & 0xFF)) {
        *buf = 0;
        return tmp;
    }
    *buf++ = '.';
    while ((n &= 0xFF)) {
        n *= 10;
        *buf++ = (n >> 8) + '0';
    }
    *buf = 0;

    return tmp;
}

//following is small functions for handling temperature and i2c
/* Wait for I2C bus to become idle */
void i2c_idle() {
    while (I2C1CON & 0x1F || I2C1STAT & (1 << 14)); //TRSTAT
}

/* Send one byte on I2C bus, return ack/nack status of transaction */
bool i2c_send(uint8_t data) {
    i2c_idle();
    I2C1TRN = data;
    i2c_idle();
    return !(I2C1STAT & (1 << 15)); //ACKSTAT
}

/* Receive one byte from I2C bus */
uint8_t i2c_recv() {
    i2c_idle();
    I2C1CONSET = 1 << 3; //RCEN = 1
    i2c_idle();
    I2C1STATCLR = 1 << 6; //I2COV = 0
    return I2C1RCV;
}

/* Send acknowledge conditon on the bus */
void i2c_ack() {
    i2c_idle();
    I2C1CONCLR = 1 << 5; //ACKDT = 0
    I2C1CONSET = 1 << 4; //ACKEN = 1
}

/* Send not-acknowledge conditon on the bus */
void i2c_nack() {
    i2c_idle();
    I2C1CONSET = 1 << 5; //ACKDT = 1
    I2C1CONSET = 1 << 4; //ACKEN = 1
}

/* Send start conditon on the bus */
void i2c_start() {
    i2c_idle();
    I2C1CONSET = 1 << 0; //SEN
    i2c_idle();
}

/* Send restart conditon on the bus */
void i2c_restart() {
    i2c_idle();
    I2C1CONSET = 1 << 1; //RSEN
    i2c_idle();
}

/* Send stop conditon on the bus */
void i2c_stop() {
    i2c_idle();
    I2C1CONSET = 1 << 2; //PEN
    i2c_idle();
}

// end of i2c preparations


void light(void) {
    static int counter = 1;
    volatile int *pointer = (volatile int *) 0xbf886110;
    *pointer = counter;
    counter++;
    return;
}

/* Interrupt Service Routine */
void user_isr(void) {
    static int timeoutcounter = 0;


    // this is the interrupts for counting
    //int buttonstatus = getbtns();

    if (IFS(0) & 0x100) {

        IFSCLR(0) = 0x100;
        timeoutcounter++;
        if (timeoutcounter == 10) {
            timeoutcounter = 0;

            display_debug(&IFS(0));
            display_update();


        }
    }

    if (IFS(0) & 0x80) {
        format = 0;
        IFSCLR(0) = 0x80;
    }
    if (IFS(0) & 0x800) {
        format = 2;
        IFSCLR(0) = 0x800;
    }

    if (IFS(0) & 0x8000) {
        format = 1;
        IFSCLR(0) = 0x8000;
    }

    if (IFS(0) & 0x8) {
        IFSCLR(0) = 0x8;
        if (pause) {
            pause--;
        }
        if (!pause) {
            pause++;
        }
    }
}

int temp_F(temp) {
    temp = (temp * 1.8) + 32;
	
    return temp;
}

int temp_K(temp) {
    temp += 273;
    return temp;
}

void timer(void) {
    T2CON = 0x0070;
    PR2 = 31250;
    TMR2 = 0;
    IECSET(0) = 0x100;
    IPCSET(2) = 0x5;
    T2CONSET = 0x8000;

    return;
}


/* Lab-specific initialization goes here */
void labinit(void) {
    volatile int *pointer = (volatile int *) 0xbf886110;
    int mask = 0xFF;
    mask = ~mask; //~ betyder 2:nd komplement 
    *pointer = *pointer & mask;
    TRISDSET = 0xFE0;

     timer();
   
    IFSSET(0) = 0x8888; //Enablar alla switchar
	IECSET(0) = 0x8888;
	IPCSET(3) = 0xA000000; //För switch 3
	IPCSET(2) = 0xA000000; //För switch 2
	IPCSET(1) = 0xA000000; //För switch 1
	IPCSET(0) = 0xA000000; //För switch 0
	
    enable_interrupt();

    // init of temperature reader

    /* Set up i2c */
    I2C1CON = 0x0;
    /* I2C Baud rate should be less than 400 kHz, is generated by dividing
    the 40 MHz peripheral bus clock down */
    I2C1BRG = 0x0C2;
    I2C1STAT = 0x0;
    I2C1CONSET = 1 << 13; //SIDL = 1
    I2C1CONSET = 1 << 15; // ON = 1
    int temp = I2C1RCV; //Clear receive buffer

    /* Send start condition and address of the temperature sensor with
    write mode (lowest bit = 0) until the temperature sensor sends
    acknowledge condition */

    do {
        i2c_start();
    } while (!i2c_send(TEMP_SENSOR_ADDR << 1));
    /* Send register number we want to access */
    i2c_send(TEMP_SENSOR_REG_CONF);
    /* Set the config register to 0 */
    i2c_send(0x0);
    /* Send stop condition */
    i2c_stop();


    return;
}

/* This function is called repetitively from the main program */
void labwork(void) {
    while(pause) {
	}
	char buf[32];
    int temperature, temp;
    do {
        i2c_start();
    } while (!i2c_send(TEMP_SENSOR_ADDR << 1));
    /* Send register number we want to access */
    i2c_send(TEMP_SENSOR_REG_TEMP);

    /* Now send another start condition and address of the temperature sensor with
    read mode (lowest bit = 1) until the temperature sensor sends
    acknowledge condition */
    do {
        i2c_start();
    } while (!i2c_send((TEMP_SENSOR_ADDR << 1) | 1));

    /* Now we can start receiving data from the sensor data register */
    temp = i2c_recv() << 8;
    i2c_ack();
    temp |= i2c_recv();
    /* To stop receiving, send nack and stop */
    i2c_nack();
    i2c_stop();



    //check syntax for getTemp and switch!!!
    switch (temperature) {
        case 1 :
            temperature = temp_F(temp);
            break;
        case 2 :
            temperature = temp_K(temp);
            break;
        default :
            temperature = temp;

    }

    s = fixed_to_string(temp, buf);

    switch (temperature) {
        case 0 :
            t = s + strlen(s);
            *t++ = ' ';
            *t++ = 7;
            *t++ = 'C';
            *t++ = 0;
            break;
        case 1 :
            t = s + strlen(s);
            *t++ = ' ';
            *t++ = 7;
            *t++ = 'F';
            *t++ = 0;
            break;
        case 2 :
            t = s + strlen(s);
            *t++ = ' ';
            *t++ = 7;
            *t++ = 'K';
            *t++ = 0;
            break;

    }
    display_string(1, s);
    return;

}

//display_debug(&test);  used to print adress+ value of var on chipkit
  

