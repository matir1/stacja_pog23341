/*
 * ADC.c
 *
 *  Created on: 14 kwi 2026
 *      Author: matir
 */

#include "ADC.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"
#include "hal/adc_types.h"
 
adc_oneshot_unit_handle_t adc_1;
adc_cali_handle_t calib;
int adc_raw;
int adc_cal;
 

void adc_init(uint8_t ADC_3P3, uint8_t ADC_bat, uint8_t ADC_temp){
	adc_oneshot_unit_init_cfg_t adc_1_config={
		.unit_id = ADC_UNIT_1
	};
	adc_oneshot_new_unit(&adc_1_config, &adc_1);
	
	adc_oneshot_chan_cfg_t chan_config={
		.bitwidth=ADC_BITWIDTH_DEFAULT,
		.atten=ADC_ATTEN_DB_12
	};
	adc_oneshot_config_channel(adc_1, (adc_channel_t) ADC_3P3, &chan_config);
	adc_oneshot_config_channel(adc_1, (adc_channel_t) ADC_bat, &chan_config);
	adc_oneshot_config_channel(adc_1, (adc_channel_t) ADC_temp, &chan_config);
	
	adc_cali_curve_fitting_config_t cali_curve={
		.atten=ADC_ATTEN_DB_12,
		.bitwidth=ADC_BITWIDTH_DEFAULT,
		.unit_id=ADC_UNIT_1
	};
	adc_cali_create_scheme_curve_fitting(&cali_curve, &calib);
}

int get_voltage(uint8_t channel){
	adc_oneshot_read(adc_1, (adc_channel_t) channel, &adc_raw);
	adc_cali_raw_to_voltage(calib, adc_raw, &adc_cal);
	return adc_cal;
}












