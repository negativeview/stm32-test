/*
 * custom.c
 *
 *  Created on: Nov 26, 2021
 *      Author: dgrace
 */

#include "custom.h"

#include <string.h>
#include <stdio.h>

// Buffer for i2c communication. Probably won't survive into production code.
uint8_t buf[12];

UART_HandleTypeDef *debugUART = 0;
I2C_HandleTypeDef *tempSensor = 0;
// The i2c address of the thing we want to talk to. Always a 7-bit address, so you have to shift the actual address over by one.
static const uint8_t TMP102_ADDR = 0x48 << 1;

// Register of the data we want to read. This may or may not work a similar way with other sensors, but putting here for completeness.
static const uint8_t TEMP_ADDR = 0x00;

void custom_setup_debug_uart(UART_HandleTypeDef *_debugUART)
{
	debugUART = _debugUART;
}

void custom_setup_temperature_sensor(I2C_HandleTypeDef *_tempSensor)
{
	tempSensor = _tempSensor;
}

void custom_tick()
{
	// Return value of certain HAL calls.
	HAL_StatusTypeDef ret;

	// Raw temperature data read from the TMP102. Actually a 12-bit value, but 16 is smallest it fits into.
	int16_t val;
	// The user-readable value.
	float temp_c;

	// Byte 0 is the address we want to operate on. At least with the TMP102, not sure if many work this way.
	buf[0] = TEMP_ADDR;
	// TODO: THIS BLOCKS!!
	ret = HAL_I2C_Master_Transmit(tempSensor, TMP102_ADDR, buf, 1, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		strcpy((char *)buf, "Error Tx\r\n");
	} else {
		// TODO: THIS BLOCKS!!
		ret = HAL_I2C_Master_Receive(tempSensor, TMP102_ADDR, buf, 2, HAL_MAX_DELAY);
		if (ret != HAL_OK) {
			strcpy((char *)buf, "Error Rx\r\n");
		} else {
			// Combine bytes
			val = ((int16_t)buf[0] << 4) | (buf[1] >> 4);

			// Convert to 2's complement
			if (val > 0x7FF) {
				val |= 0xF000;
			}

			temp_c = val * 0.0625;

			temp_c *= 100;
			sprintf((char *)buf, "%u.%02u C\r\n",
					((unsigned int)temp_c / 100),
					((unsigned int)temp_c % 100)
			);
		}
	}

	if (debugUART != 0) {
		// Sample code for UART communication.
		// TODO: THIS BLOCKS!!
		HAL_UART_Transmit(debugUART, buf, strlen((char *)buf), HAL_MAX_DELAY);
		HAL_Delay(500);
	}



}
