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
#include <xc.h>
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

#define SOCKET_TCP      0
#define SOCKET_UDP      1

#define BUFFER_SIZE     2048

#pragma config  OSC = INTIO67, BOREN = OFF, PWRT = ON
#pragma config  WDT = OFF, DEBUG = ON, LVP = OFF

intr_kind wiznet_interrupt = 0;
char wiznet_interrupt_flag = 0;
const uint8_t network_ip[] = {192, 168, 1, 204};
const uint8_t network_gw[] = {192, 168, 1, 1};
const uint8_t network_dns[] = {8, 8, 8, 8};
const uint8_t network_sn[] = {255, 255, 255, 0};
const uint8_t network_mac[] = {0x36, 0x8A, 0xB6, 0x89, 0x90, 0x87};
uint8_t memsize[2][8] = {
    {2, 2, 2, 2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2, 2, 2, 2}
};
char init_error = 0;
char phy_link_status = 0;
uint8_t network_id[6] = {0};
uint8_t buffer[BUFFER_SIZE];
wiz_NetInfo networkConfig = {
    {0x36, 0x8A, 0xB6, 0x89, 0x90, 0x87},
    {192, 168, 1, 204},
    {255, 255, 255, 0},
    {192, 168, 1, 1},
    {8, 8, 8, 8},
    NETINFO_STATIC
};

void cris_enter(void);
void cris_exit(void);
void cs_select(void);
void cs_deselect(void);
uint8_t spi_readbyte(void);
void spi_writebyte(uint8_t wb);
void send_UDP_test();
void send_TCP_test();

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
    TRISCbits.RC3 = 0; //SCK
    TRISCbits.RC5 = 0; //SDO
    //    cris_enter();
    cs_deselect();
    OpenSPI(SPI_FOSC_64, MODE_00, SMPEND);
//    GIE = 1;
    reg_wizchip_cs_cbfunc(cs_select, cs_deselect);
    reg_wizchip_spi_cbfunc(spi_readbyte, spi_writebyte);
    if (ctlwizchip(CW_INIT_WIZCHIP, (void*) memsize) == -1) {
        init_error = 1;
    }
    do {
        if (ctlwizchip(CW_GET_PHYLINK, (void*) &phy_link_status) == -1);
    } while (phy_link_status == PHY_LINK_OFF);
    ctlnetwork(CN_SET_NETINFO, (void*) &networkConfig);
    ctlnetwork(CN_GET_NETINFO, (void*) &networkConfig);
    ctlwizchip(CW_GET_ID, (void*) network_id);

    sprintf(buffer, "Test UDP 1234567890");
    send_UDP_test();

    return (EXIT_SUCCESS);
}

void cris_enter(void) {
    INT0IE = 1;
}

void cris_exit(void) {
    INT0IE = 0;
}

void cs_select(void) {
    LATD &= ~(1 << 4);
}

void cs_deselect(void) {
    LATD |= 1 << 4;
}

uint8_t spi_readbyte(void) {
    return ReadSPI();
}

void spi_writebyte(uint8_t wb) {
    WriteSPI(wb);
}

void send_UDP_test() {
    uint8_t dest_ip[4] = {192, 168, 1, 255};
    uint8_t port_udp = 2702;
    socket(SOCKET_UDP, Sn_MR_UDP, port_udp, 0);
    while (1) {
        sendto(SOCKET_UDP, buffer, BUFFER_SIZE, dest_ip, port_udp);
    }
}

void send_TCP_test() {
    uint8_t port_tcp = 2999;
    socket(SOCKET_TCP, Sn_MR_TCP, port_tcp, 0);
}