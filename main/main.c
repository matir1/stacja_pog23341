#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "portmacro.h"

#include "ADC.h"
#include "IO.h"
#include "I2C.h"
#include "Timer.h"

#define LED_3P3 3
#define LED_bat 10
#define CTRL_heater 20
#define CTRL_fan 21

#define TIM_freq 1000000

#define ADC_bat 1
#define ADC_3P3 0
#define ADC_temp 4
#define ADC_mV_0 1567.0

QueueHandle_t queue_bat;
QueueHandle_t queue_3P3;

QueueHandle_t queue_temp_adc;

QueueHandle_t queue_temp_hum;

QueueHandle_t queue_heater;
QueueHandle_t queue_fan;

void Measurments(void* pvParameters){
	volatile float level_bat=0;
	volatile int level_bat_int=0;
	volatile float level_3P3=0;
	volatile int level_3P3_int=0;
	volatile float temp_debug=0;
	float temp=0;
	volatile int temp_int=0;
	bool flag_bat=0;
	bool flag_3P3=0;
	
	weather_data current_data;
	
	volatile uint32_t timer_val=0;
	bool flag_heater=0;
	bool flag_fan=0;
	
	while(1){
		
		level_3P3_int=get_voltage(ADC_3P3);
		level_bat_int=get_voltage(ADC_bat);
		
		level_3P3 = (float)level_3P3_int*5.7/1000;
		level_bat = (float)level_bat_int*11/1000;
		
		temp_int=get_voltage(ADC_temp);
		temp = ((float)temp_int-ADC_mV_0)/(-8.2);
		temp_debug = ((float)temp_int-ADC_mV_0)/(-8.2);
		
		current_data=I2C_read_data();
		
		timer_val=(uint32_t)get_time();
		if(timer_val>=25000000){
			flag_heater=1;
		}
		if(timer_val>=40000000){
			flag_fan=1;
		}
		if(timer_val>=50000000){
			flag_heater=0;
			flag_fan=0;
			reset_timer();
		}
		
		if(level_bat>10.0){
			flag_bat=1;
			xQueueSend(queue_bat, &flag_bat, pdMS_TO_TICKS(10));
		}
		else{
			flag_bat=0;
			xQueueSend(queue_bat, &flag_bat, pdMS_TO_TICKS(10));
		}
		if(level_3P3>3.0){
			flag_3P3=1;
			xQueueSend(queue_3P3, &flag_3P3, pdMS_TO_TICKS(10));
		}
		else{
			flag_3P3=0;
			xQueueSend(queue_3P3, &flag_3P3, pdMS_TO_TICKS(10));
		}
		
		xQueueSend(queue_temp_adc,&temp,pdMS_TO_TICKS(20));
		
		xQueueSend(queue_temp_hum,&current_data,pdMS_TO_TICKS(40));
		
		xQueueSend(queue_fan, &flag_fan, pdMS_TO_TICKS(10));
		xQueueSend(queue_heater, &flag_heater, pdMS_TO_TICKS(10));
		
		vTaskDelay(2000/portTICK_PERIOD_MS);
	}
}

void Control(void* pvParameters){
	bool flag_bat=0;
	bool flag_3P3=0;
	
	bool flag_heater=0;
	bool flag_fan=0;
	
	while(1){
		
		if(xQueueReceive(queue_bat, &flag_bat, pdMS_TO_TICKS(10))==pdPASS){
			if(flag_bat){
				set_GPIO(LED_bat,1);
			}
			else{
				set_GPIO(LED_bat,1);
			}
		}
		if(xQueueReceive(queue_3P3, &flag_3P3, pdMS_TO_TICKS(10))==pdPASS){
			if(flag_3P3){
				set_GPIO(LED_3P3,1);
			}
			else{
				set_GPIO(LED_3P3,1);
			}
		}
		
		if(xQueueReceive(queue_fan, &flag_fan, pdMS_TO_TICKS(10))==pdPASS){
			if(flag_fan){
				set_GPIO(CTRL_fan,1 );
			}
			else{
				set_GPIO(CTRL_fan, 0);
			}
		}
		if(xQueueReceive(queue_heater, &flag_heater, pdMS_TO_TICKS(10))==pdPASS){
			if(flag_heater){
				set_GPIO(CTRL_heater,1 );
			}
			else{
				set_GPIO(CTRL_heater, 0);
			}
		}
		vTaskDelay(500/portTICK_PERIOD_MS);
	}
}

void print_data(void* pvParameters){
	float temp=0;
	weather_data current_data;
	
	while(1){
		xQueueReceive(queue_temp_hum, &current_data, pdMS_TO_TICKS(40));
		xQueueReceive(queue_temp_adc, &temp, pdMS_TO_TICKS(20));
		vTaskDelay(15000/portTICK_PERIOD_MS);
	}
	
}

void app_main(void)
{
	gpio_init(LED_bat, LED_3P3, CTRL_fan, CTRL_heater);
	adc_init(ADC_3P3, ADC_bat, ADC_temp);
	I2C_init();
	timer_init(TIM_freq);
	queue_bat=xQueueCreate(1,sizeof(bool));
	queue_3P3=xQueueCreate(1,sizeof(bool));
	queue_temp_adc=xQueueCreate(1,sizeof(float));
	queue_temp_hum=xQueueCreate(1,sizeof(weather_data));
	queue_fan=xQueueCreate(1,sizeof(bool));
	queue_heater=xQueueCreate(1,sizeof(bool));
    xTaskCreate(&Measurments, "Measurments Task", 4096, NULL, 5, NULL);
	xTaskCreate(&Control, "Control Task", 4096, NULL, 4, NULL);
	xTaskCreate(&print_data, "Wifi Task",  8192, NULL, 3, NULL);
}


