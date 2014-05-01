/* 
 * File:   main.c
 * Author: g.lagrange
 *
 * Created on 1 mai 2014, 10:47
 */
#define USE_OR_MASKS
#define _XTAL_FREQ 8000000

#include <stdio.h>
#include <stdlib.h>
#include <plib/spi.h>

#define W5500_RDY_PORT  PORTD
#define W5500_RDY_PIN   7
#define W5500_RST_PORT  LATD
#define W5500_RST_PIN   6
#define W5500_INT_PORT  PORTD
#define W5500_INT_PIN   5
#define W5500_SS_PORT   LATD
#define W5500_SS_PIN    4

#pragma config  OSC = INTIO67, BOREN = OFF, PWRT = ON
#pragma config  WDT = OFF, DEBUG = ON, LVP = OFF

int main(int argc, char** argv) {
    OSCCON = 0b01110010;
    ADCON1 = 0xFF;
    TRISC = (1 << 4);
    TRISD = (1 << 5);

    return (EXIT_SUCCESS);
}

