/*
 * IO.c
 *
 *  Created on: 14 kwi 2026
 *      Author: matir
 */

#include "IO.h"
#include "driver/gpio.h"
#include "hal/gpio_types.h"
 
void gpio_init(uint8_t led_bat, uint8_t led_3P3, uint8_t ctrl_fan, uint8_t ctrl_heat){
	gpio_config_t io_config = {
		.pin_bit_mask = (1ULL<<led_bat) | (1ULL<<led_3P3) | (1ULL<<ctrl_fan) | (1ULL<<ctrl_heat),
		.mode = GPIO_MODE_OUTPUT,
		.pull_up_en = GPIO_PULLUP_DISABLE,
		.pull_down_en = GPIO_PULLDOWN_ENABLE,
		.intr_type = GPIO_INTR_DISABLE
	};
	gpio_config(&io_config);
}
 
void set_GPIO(uint8_t GPIO_id, bool state){
	gpio_set_level((gpio_num_t)GPIO_id, state);
}