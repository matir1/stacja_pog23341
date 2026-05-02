/*
 * I2C.h
 *
 *  Created on: 14 kwi 2026
 *      Author: matir
 */

#ifndef MAIN_I2C_H_
#define MAIN_I2C_H_

typedef struct {
	float temperature;
	float humidity;
} weather_data;

void I2C_init();
weather_data I2C_read_data();

#endif /* MAIN_I2C_H_ */
