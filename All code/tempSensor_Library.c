

#include "xc.h"
#include "tempSensor_Library.h"

void i2c_init(void) 
{
    TRISBbits.TRISB0 = 1; // Digital Output (make it input only when reading data)
    TRISBbits.TRISB1 = 1; // Digital Output
    TRISBbits.TRISB4=0; //Testing LED output
    I2C1CON=0x0000; //clear control bits
    I2C1STAT=0x0000; //clear status bits
    I2C1CONbits.DISSLW=1; //Disable slew rate control
    I2C1CONbits.SMEN=0; //disable SMBUS
    I2C1BRG=0x9D; //Baud Rate 100 kHz
    IFS1bits.MI2C1IF = 0; //reset IF
    I2C1CONbits.I2CEN=1;
    I2C1CONbits.SEN = 0; //Force Idle
}

void i2c_start(void) 
{
    IFS1bits.MI2C1IF = 0; //clear flag
    while ( I2C1STATbits.RBF ) ; // wait for idle condition
    I2C1CONbits.SEN = 1; //Initiate START condition
    while (!IFS1bits.MI2C1IF) ; // wait for a flag to be set
    IFS1bits.MI2C1IF = 0; //clear flag
    
}

void i2c_stop(void)
{
    IFS1bits.MI2C1IF = 0; //clear flag
    while ( I2C1STATbits.RBF ) ; // wait for idle condition
    I2C1CONbits.PEN = 1; //Initiate STOP condition
    while (!IFS1bits.MI2C1IF) ; // wait for a flag to be set
    IFS1bits.MI2C1IF = 0; //clear flag
}

unsigned char i2c_write(unsigned char i2cWriteData)
{
    IFS1bits.MI2C1IF = 0; //clear flag
    while (I2C1STATbits.RBF) ; // wait for idle condition
    I2C1TRN = i2cWriteData;
    while (!IFS1bits.MI2C1IF) ; // wait for a flag to be set
    IFS1bits.MI2C1IF = 0; //clear flag
    return ( !I2C1STATbits.ACKSTAT ); // function returns '1' if transmission is acknowledged
}

unsigned char i2c_read( unsigned char ack ) 
{
    unsigned char i2cReadData;
    IFS1bits.MI2C1IF = 0; //reset IF
    while (I2C1STATbits.RBF); // wait for idle condition
    I2C1CONbits.RCEN = 1; // enable receive mode
    while (!IFS1bits.MI2C1IF); // wait for a flag to be set
    IFS1bits.MI2C1IF = 0; //reset IF
    i2cReadData = I2C1RCV; // Read SSPBUF and put it in i2cReadData
    if (ack) // if ack=1
    { 
        I2C1CONbits.ACKDT = 0; // then transmit an Acknowledge
    } 
    else 
    {
        I2C1CONbits.ACKDT = 1;  // otherwise transmit a Not Acknowledge
    }
    I2C1CONbits.ACKEN = 1; // send acknowledge sequence
    while (!IFS1bits.MI2C1IF) ; // wait for a flag to be set
    IFS1bits.MI2C1IF = 0; //reset IF
    return(i2cReadData); // return the value read from SSPBUF
} 

void i2c_config(void)
{
    i2c_start(); // send START command
    i2c_write(0b00110000); //WRITE Command (see Section 4.1.4 ?Address Byte?)
    i2c_write(0x01); // Write CONFIG Register
    i2c_write(0x00); // Write data
    i2c_write(0x08); // Write data
    i2c_stop(); // send STOP command
    i2c_start(); // send START command
    i2c_write(0b00110000); //WRITE Command (see Section 4.1.4 ?Address Byte?)
    i2c_write(0x05); // Write TA Register Address
    i2c_start(); //Repeat START
    i2c_write(0b00110001); // READ Command (see Section 4.1.4 ?Address Byte?)
}

void initPushButton(void)
{
    LATB=0x0000;
    TRISBbits.TRISB15=1; //Button input
    CNPU1bits.CN11PUE=1; //Pullup resistor for RB15 Button
    T2CON = 0x0020; //Stop Timer, Tcy clk source, PRE 1:256
    TMR2 = 0;     // Initialize to zero (also best practice)
    PR2 = 62499; // Set period to be larger than max external sig duration
    T2CONbits.TON = 1; // Restart 16-bit Timer2
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;

}
