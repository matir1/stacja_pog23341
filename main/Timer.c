/*
 * Timer.c
 *
 *  Created on: 14 kwi 2026
 *      Author: matir
 */

#include "Timer.h"
#include "driver/gptimer.h"
#include "driver/gptimer_types.h"
#include "esp_err.h"
#include "hal/timer_types.h"
#include "soc/clk_tree_defs.h"

gptimer_handle_t timer_1 = NULL;
uint64_t timer_val;

void timer_init(uint32_t freq){
	gptimer_config_t timer_config = {
		.clk_src = GPTIMER_CLK_SRC_DEFAULT,
		.resolution_hz = freq,
		.direction = GPTIMER_COUNT_UP
	};
	ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &timer_1));
	ESP_ERROR_CHECK(gptimer_enable(timer_1));
	ESP_ERROR_CHECK(gptimer_start(timer_1));
}

uint64_t get_time(){
	gptimer_get_raw_count(timer_1, &timer_val);
	return timer_val;
}

void reset_timer(){
	gptimer_set_raw_count(timer_1, 0);
}























