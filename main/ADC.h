/*
 * ADC.h
 *
 *  Created on: 14 kwi 2026
 *      Author: matir
 */

#ifndef MAIN_ADC_H_
#define MAIN_ADC_H_

#include "stdint.h"

void adc_init(uint8_t ADC_3P3, uint8_t ADC_bat, uint8_t ADC_temp);
int get_voltage(uint8_t channel);


#endif /* MAIN_ADC_H_ */
