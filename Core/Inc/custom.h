/*
 * custom.h
 *
 *  Created on: Nov 26, 2021
 *      Author: dgrace
 */

#ifndef INC_CUSTOM_H_
#define INC_CUSTOM_H_

#include "stm32g4xx_hal.h"

void custom_tick();
void custom_setup_debug_uart(UART_HandleTypeDef *_debugUART);
void custom_setup_temperature_sensor(I2C_HandleTypeDef *_tempSensor);

#endif /* INC_CUSTOM_H_ */
