/*
 * Timer.h
 *
 *  Created on: 14 kwi 2026
 *      Author: matir
 */

#ifndef MAIN_TIMER_H_
#define MAIN_TIMER_H_

#include "stdint.h"

void timer_init(uint32_t freq);
uint64_t get_time();
void reset_timer();


#endif /* MAIN_TIMER_H_ */
