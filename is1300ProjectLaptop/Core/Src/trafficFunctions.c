/**
******************************************************************************
@brief functions for traffic-program
@file trafficFunctions.c
@author Viktor Björkén
@version 1.0
@date December-2023
@brief Functions and structures for program traffic
******************************************************************************
*/

#include <stdint.h>
#include "stdbool.h"
#include "gpio.h"
#include "spi.h"

#include "test.h"
#include "traffic.h"

/**
@brief Shifts data to a shift register.
@param uint8_t buffer[], the buffer of bits that represent a LED
@param uint8_t regs, the size of the amount of data (Number of registers)
*/
void ShiftREG(uint8_t buffer[], uint8_t regs)
{
	HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &buffer[0], regs, HAL_MAX_DELAY);

	HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_SET);
	HAL_Delay(1);

}


/**
  @brief toggleLEDWithFrequency, Toggles an LED with a specified frequency for a given duration.
  @param uint16_t toggleFrequency Frequency of LED toggle in milliseconds.
  @param uint16_t duration_ms Duration of LED toggle in milliseconds.
  @param uint8_t blinkingLED Array representing the blinking LED configuration.
  @param uint8_t staticleds Array representing the static LED configuration.
 */
void toggleLEDWithFrequency(uint16_t toggleFrequency, uint16_t duration_ms,  uint8_t blinkingLED[3], uint8_t staticleds[3] ) {
	    uint8_t ledState = 0; // 0 for OFF, 1 for ON
	    uint32_t startTime = HAL_GetTick();
	    uint32_t currentTime;

	    while (1) {
	        currentTime = HAL_GetTick();

	        // Check if the specified duration has elapsed
	        if ((currentTime - startTime) >= duration_ms) {
	            // Duration has elapsed, exit the loop
	            break;
	        }

	        // Toggle the LED state
	        ledState = !ledState;

	        // Update the LED based on the state
	        if (ledState) {
	            ShiftREG(blinkingLED, 3);  // Turn on the LED
	        } else {

	            ShiftREG(staticleds, 3);
	        }

	        // Introduce a delay based on the toggle frequency
	        HAL_Delay(toggleFrequency);
	    }
	}
