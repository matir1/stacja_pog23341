/*
 * I2C.c
 *
 *  Created on: 14 kwi 2026
 *      Author: matir
 */
 
#include "I2C.h"
#include "driver/i2c_master.h"
#include "driver/i2c_types.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "hal/i2c_types.h"
#include "soc/clk_tree_defs.h"
#include "freertos/task.h"
 
i2c_master_bus_handle_t bus_handle;
i2c_master_dev_handle_t dev_handle;


uint8_t cmd[2]={0x24,0x00};
uint8_t data[6];
int temp_register;
int humidity_register;
weather_data current_data;
 
 void I2C_init(){
	i2c_master_bus_config_t bus_config={
		.clk_source=I2C_CLK_SRC_DEFAULT,
		.scl_io_num=8,
		.sda_io_num=2,
		.i2c_port=I2C_NUM_0,
		.glitch_ignore_cnt=10
	};
	i2c_new_master_bus(&bus_config, &bus_handle);
	
	printf("Skanowanie magistrali I2C...\n");
	for (uint8_t addr = 1; addr < 128; addr++) {
	    esp_err_t err = i2c_master_probe(bus_handle, addr, 50);
	    if (err == ESP_OK) {
	        printf("SUKCES! Znaleziono fizyczne urządzenie pod adresem: 0x%02X\n", addr);
	    }
	}
	printf("Skanowanie zakończone.\n");
	
	i2c_device_config_t dev_config={
		.scl_speed_hz=100000,
		.dev_addr_length=I2C_ADDR_BIT_LEN_7,
		.device_address=0x44
	};
	i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle);
 }
 
weather_data I2C_read_data(){
	esp_err_t err;
	err=i2c_master_transmit(dev_handle, cmd, 2, 50);
	/*if(err!=ESP_OK){printf("Błąd transmisji! Kod: %s\n", esp_err_to_name(err));}
	else{printf("trans ok\n");}*/
	vTaskDelay(pdMS_TO_TICKS(20));
	err=i2c_master_receive(dev_handle, data, 6, 100);
	
	/*if(err!=ESP_OK){printf("błąd odbioru\n");}
	else{printf("odb ok\n");}*/
	temp_register=data[0];
	temp_register=temp_register<<8|data[1];
	humidity_register=data[3];
	humidity_register=(humidity_register<<8)|data[4];
	current_data.temperature=-45.0+175.0*(float)temp_register/65535;
	current_data.humidity=100*(float)humidity_register/65535;
	return current_data;
 }


