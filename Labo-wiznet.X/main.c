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
#include "socket.h"

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

intr_kind wiznet_interrupt = 0;
char wiznet_interrupt_flag = 0;

void wizchip_cris_enter(void);
void wizchip_cris_exit(void);
void wizchip_cs_select(void);
void wizchip_cs_deselect(void);
uint8_t wizchip_spi_readbyte(void);
void wizchip_spi_writebyte(uint8_t wb);

void interrupt ISR() {
    if (INT0IF == 1) {
        INT0IF = 0;
        wiznet_interrupt = wizchip_getinterrupt();

    }
}

int main(int argc, char** argv) {
    OSCCON = 0b01110010;
    ADCON1 = 0xFF;
    TRISC = (1 << 4);
    TRISB = (1 << 0);
    wizchip_cris_enter();
    wizchip_cs_deselect();
    OpenSPI(SPI_FOSC_4, MODE_00, SMPMID);
    GIE = 1;


    return (EXIT_SUCCESS);
}

void wizchip_cris_enter(void) {
    INT0IE = 1;
}

void wizchip_cris_exit(void) {
    INT0IE = 0;
}

void wizchip_cs_select(void) {
    LATD &= ~(1 << 4);
}

void wizchip_cs_deselect(void) {
    LATD |= 1 << 4;
}

uint8_t wizchip_spi_readbyte(void) {
    return ReadSPI();
}

void wizchip_spi_writebyte(uint8_t wb) {
    WriteSPI(wb);
}