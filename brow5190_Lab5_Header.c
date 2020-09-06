//Date:3/23/2020
//Name: Truman Brown
//Student ID number: 5447775
//Course number: EE2361
//Term: Spring 2020
//Lab assignment number: Lab 5 
//Short Program Description: Use I2C to communicate with LCD


#include "xc.h"
#include "brow5190_Lab5_Header.h"
#include "brow5190_lab2b_asmLib_v001.h"

void setupLCD(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;            //sets all pins to digital I/O
    //I2C2CONbits.I2CEN=0;
    I2C2BRG=0x9D; //100 kHz
    IFS3bits.MI2C2IF = 0; //reset IF
    I2C2CONbits.I2CEN=1;
}

void lcd_cmd(char command){
    I2C2CONbits.SEN = 1; //initiate start condition
    while(I2C2CONbits.SEN == 1); //SEN will clear when start bit is complete
    IFS3bits.MI2C2IF = 0; //reset IF
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0; //reset IF
    I2C2TRN = 0b00000000; // 8-bits consisting of control byte
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0; //reset IF
    I2C2TRN = command; //8-bits consisting of data
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0; //reset IF
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1); //PEN will clear when Stop bit is complete
}


char contrast=0b01110000; 

void lcd_init(void)
{
    delay(50);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extended instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(contrast); // contrast C3-C0
    lcd_cmd(0b01010110); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control
    delay(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    delay(2);
}

void lcd_setCursor(char Columnx, char Rowy)
{
    if (Rowy == 0)        //If row 1
    {
        lcd_cmd(0b10000000 + Columnx);
    }
    else if (Rowy==1)    //If row 2
    {
        lcd_cmd(0b11000000 + Columnx);   
    }
}

void lcd_printChar(char myChar)
{
    I2C2CONbits.SEN=1;
    while(I2C2CONbits.SEN==1);    //Wait for SEN==0
    IFS3bits.MI2C2IF = 0; //reset IF
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    while(IFS3bits.MI2C2IF == 0); //wait for MI2C2IF==1
    IFS3bits.MI2C2IF = 0; //reset IF
    I2C2TRN = 0b01000000; // 8-bits consisting of control byte
    while(IFS3bits.MI2C2IF == 0); //wait for MI2C2IF==1
    IFS3bits.MI2C2IF = 0; //reset IF
    I2C2TRN = myChar; // 8-bits consisting of the data byte
    while(IFS3bits.MI2C2IF == 0); //wait for MI2C2IF==1
    IFS3bits.MI2C2IF = 0; //reset IF
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN==1);    //Wait for PEN==0  
}

void lcd_printStr(const char s[])
{
    unsigned int i;
    unsigned int length=0;
    unsigned int row=0;
    unsigned int column=0;
    while(s[length]!= '\0')
    {
        length++;
    }
    for(i=0; i< length; i++)
    {
        if(column>7)
        {
            row=!row;
            column=0;
        }
        lcd_setCursor(column,row);
        lcd_printChar(s[i]);
        column++;
    }
}



