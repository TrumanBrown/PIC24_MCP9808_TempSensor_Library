//Date:3/23/2020
//Name: Truman Brown
//Student ID number: 5447775
//Course number: EE2361
//Term: Spring 2020
//Lab assignment number: Lab 5 
//Short Program Description: Use I2C to communicate with LCD

#ifndef BROW5190_LAB5_HEADER_H
#define	BROW5190_LAB5_HEADER_H

#include <xc.h> // include processor files - each processor file is guarded.  

#ifdef	__cplusplus
extern "C" {
#endif

    void setupLCD(void);
    void lcd_cmd(char command);
    void lcd_init(void);
    void lcd_setCursor(char Columnx, char Rowy);
    void lcd_printChar(char myChar);
    void lcd_printStr(const char s[]);


#ifdef	__cplusplus
}
#endif

#endif	/* BROW5190_LAB5_HEADER_H */

