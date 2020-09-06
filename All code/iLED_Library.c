#include "xc.h"
#include "stdint.h"  
#include "iLED_Library.h"
#include "brow5190_lab2b_asmLib_v001.h"

void setupLED(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;            //sets all pins to digital I/O
    TRISA = 0b0;   //set RA0 as output port
    LATA = 1;               //set RA0 to high     
    //TRISB=0b0;
}

void delay(int delay_in_ms)
{
    unsigned int i;
    for(i=0; i<delay_in_ms; i++)
    {
        delay_1ms();
    }
}

void writeColor(int r, int g, int b)
{
    delay_100us();
    unsigned long mask=0x80;    //set mask to 10000000
    unsigned int cnt=0;
    while(cnt < 8)              //8 bits for int r
    {
        if(mask & r)
        {
            write_1();
        }
        else
        {
            write_0();
        }
        mask= mask >> 1;
        cnt++;
    }
    mask=0x80;                  //reset mask to 10000000
    cnt=0;
    while(cnt < 8)              //8 bits for int g
    {
        if(mask & g)
        {
            write_1();
        }
        else
        {
            write_0();
        }
        mask= mask >> 1;
        cnt++;
    }
    mask=0x80;              //reset mask to 10000000
    cnt=0;
    while(cnt < 8)          //8 bits for int b
    {
        if(mask & b)
        {
            write_1();
        }
        else
        {
            write_0();
        }
        mask= mask >> 1;
        cnt++;
    }
    
}

