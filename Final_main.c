/* 
 * File:   Final_main.c
 * Author: linct
 *
 * Created on April 26, 2020, 5:53 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "stdint.h"                
#include <p24Fxxxx.h>
#include "xc.h"
#include "brow5190_lab2b_asmLib_v001.h" //Library for certain time delays
#include "iLED_Library.h" //Library for all functions related to RGB iLED
#include "brow5190_Lab5_Header.h" //Library for all LCD functions
#include "tempSensor_Library.h" //Library for all MCP9808 temp Sensor functions

#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
#define PERIOD 5

volatile unsigned int overflow=0;
void __attribute__((interrupt, auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0;
    overflow++;
}

int main(void) 
{
    setupLED();
    delay(1);
    setupLCD();
    delay(1);
    lcd_init();
    delay(1);
    lcd_setCursor(0,0);
    delay(1);
    i2c_init();
    initPushButton();
    
    char str[80]; //To store the temp data
    unsigned char UpperByte, LowerByte; //Ambient Temperature data from register
    float Temperature=0;  //Temperature in Celsius
    float tempF=0.0; //Temperature in Fahrenheit
    int t1=0; //To change the R value
    int t2=0; //To change the B value
    int i; //To iterate through tempArr
    int k; //To iterate through tempArr1
    char TorC=0b0; //Flips between 0/1 to display either Celsius or Fahrenheit
    unsigned int Press; //Button pressed or not pressed
    unsigned int lastPress=1; //Last button press
    TMR2=15626; //Initialize
    
    float tempArr[255]; //Store preset checkpoints of temperatures<70 deg. to compare
    for(i=0;i<255;i++)
    {
        tempArr[i]=(70-i*0.15);
    }
    float tempArr1[255]; //Store preset checkpoints of temperatures>70 deg. to compare
    for(i=0;i<255;i++)
    {
        tempArr1[i]=(70+i*0.15);
    }
    
    
    while(1)        //while loop forever
    {
        delay(1);
        Press=PORTBbits.RB15;
        i2c_config();
        UpperByte = i2c_read(1); // READ 8 bits and send ACK
        LowerByte = i2c_read(0); // READ 8 bits and send NACK
        i2c_stop(); // send STOP command //Convert the temperature data //First Check flag bits
        if ((UpperByte & 0x80) == 0x80) //TA = TCRIT
        {}
        if ((UpperByte & 0x40) == 0x40)//TA > TUPPER
        {
//            lcd_printStr("ERROR:  T>Bound");
//            break;
        }
        if ((UpperByte & 0x20) == 0x20) //TA < TLOWER
        {
//            lcd_printStr("ERROR:  T<Bound");
//            break;
        }
        UpperByte = UpperByte & 0x1F; //Clear flag bits
        if ((UpperByte & 0x10) == 0x10) //TA < 0°C
        { 
            UpperByte = UpperByte & 0x0F; //Clear SIGN
            Temperature = 256 - ((float)UpperByte * 16 + (float)LowerByte / 16);
        }
        else //TA ³ 0°C
        {
            Temperature = ((float)UpperByte * 16 + (float)LowerByte/16);
        } 
        tempF=Temperature*1.8+32.0; //Conversion of C to F
         
        if(Press == 0 && lastPress==1) //If button is pressed
        {
            if(TMR2<62000 && overflow==0)
            {
                TorC=!TorC; //Switch between Celsius and Fahrenheit
            }
            TMR2=0;
            overflow=0;
        }
        lastPress=Press;
        delay(2);
        if(TorC)
        {
            sprintf(str, "%7.5f Deg. F", (tempF)); 
            lcd_printStr(str); //Print Fahrenheit Temperature on LCD
        }
        else 
        {
            sprintf(str, "%7.5f Deg. C", (Temperature));
            lcd_printStr(str); //Print Celsius Temperature on LCD
        }
        for(k=0; k<255; k++)
        {
            if(tempF>tempArr1[k]) //>70 deg.
            {
                t2=k;
            }
            if(tempF<tempArr[k]) //<70 deg.
            {
                t1=k;
            }
        }
        writeColor(255-t1,0,255-t2);
    }
}

