/* 
 * File:   iLED_Library.h
 * Author: linct
 *
 * Created on April 26, 2020, 5:39 PM
 */

#ifndef ILED_LIBRARY_H
#define	ILED_LIBRARY_H

#include <xc.h>
#include "stdint.h"  

#ifdef	__cplusplus
extern "C" {
#endif

    void setupLED(void);
    void delay(int delay_in_ms);
    void writeColor(int r, int g, int b);

#ifdef	__cplusplus
}
#endif

#endif	/* ILED_LIBRARY_H */

