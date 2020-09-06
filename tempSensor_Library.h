/* 
 * File:   tempSensor_Library.h
 * Author: linct
 *
 * Created on April 30, 2020, 6:23 PM
 */

#ifndef TEMPSENSOR_LIBRARY_H
#define	TEMPSENSOR_LIBRARY_H

#include <xc.h> // include processor files

#ifdef	__cplusplus
extern "C" {
#endif

    void i2c_init(void);
    void i2c_start(void);
    void i2c_stop(void);
    unsigned char i2c_write(unsigned char i2cWriteData);
    unsigned char i2c_read( unsigned char ack );
    void i2c_config(void);
    void initPushButton(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* TEMPSENSOR_LIBRARY_H */

