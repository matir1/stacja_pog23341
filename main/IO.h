/*
 * IO.h
 *
 *  Created on: 14 kwi 2026
 *      Author: matir
 */

#ifndef MAIN_IO_H_
#define MAIN_IO_H_

#include "stdint.h"

void set_GPIO(uint8_t GPIO_id, bool state);
void gpio_init(uint8_t led_bat, uint8_t led_3P3, uint8_t ctrl_fan, uint8_t ctrl_heat);

#endif /* MAIN_IO_H_ */
