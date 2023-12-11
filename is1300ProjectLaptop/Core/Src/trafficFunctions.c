/*
 * trafficFunctions.c
 *
 *  Created on: Dec 11, 2023
 *      Author: viktorb
 */

#include <stdint.h>
#include "stdbool.h"
#include "gpio.h"
#include "spi.h"

#include "test.h"
#include "traffic.h"


void toggleLEDWithFrequency(uint16_t toggleFrequency, uint16_t duration_ms,  uint8_t blinkingLED[3], uint8_t leds[3] ) {
	   //uint8_t blinkingLED[3] = {0x10, 0x22, 0};
	    //uint8_t leds[3] = {0x10, 0x02, 0};
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
	            ShiftLED(blinkingLED, 3);  // Turn on the LED
	        } else {

	            ShiftLED(leds, 3);
	        }

	        // Introduce a delay based on the toggle frequency
	        HAL_Delay(toggleFrequency);
	    }
	}


void addLights(uint8_t result[3], const uint8_t array1[3], const uint8_t array2[3]) {
    for (int i = 0; i < 3; ++i) {
        result[i] = array1[i] + array2[i];
    }
}
