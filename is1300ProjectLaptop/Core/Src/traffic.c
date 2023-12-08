/*
 * traffic.c
 *
 *  Created on: Dec 7, 2023
 *      Author: viktorb
 */
#include <stdint.h>
#include "stdbool.h"
#include "gpio.h"
#include "spi.h"

#include "test.h"
#include "traffic.h"

typedef enum
{
	Start,
	Something,
	Something2,
	Something3
} states;

static states State, NextState;

uint8_t ledsOff[3] = {0, 0, 0};
uint16_t toggleFreq = 200;
uint16_t pedestrianDelay = 5000;
uint16_t walkingDelay = 5000;
uint16_t orangeDelay = 2000;

void traffic(void)
{
	State = Start;
	NextState = Start;

	while(1){

		State = NextState;
				switch (State) // State machine
				{
					case Start:
					    uint8_t leds[3] = {0x20, 0x0C, 0};


					    ShiftLED(leds, 3);

					    if (HAL_GPIO_ReadPin(GPIOB ,PL2_Switch_Pin) == GPIO_PIN_RESET)
					    	    {
					    		//ShiftLED(ledsOff,3);
					    	toggleLEDWithFrequency2(toggleFreq, pedestrianDelay);
					    	      NextState = Something;
					    	        }




						 // Stay in Start state
					break;

					case Something:
					{



							toggleLEDWithFrequency(toggleFreq, (pedestrianDelay-orangeDelay));

							NextState = Something2;

					}
					break;
					case Something2:
					{
						uint8_t herrgoman[3] = {0x08, 0x11, 0};

							ShiftLED(herrgoman,3);
							HAL_Delay(walkingDelay);
							NextState = Something3;

					}
					break;
					case Something3:
										{
											uint8_t orange[3] = {0x10, 0x0A, 0};

												ShiftLED(orange,3);
												HAL_Delay(2000);
												NextState = Start;

										}
										default:
										break;
									}
						}
					}


void toggleLEDWithFrequency(uint16_t toggleFrequency, uint16_t duration_ms) {
	   uint8_t blinkingLED[3] = {0x10, 0x22, 0};
	    uint8_t leds[3] = {0x10, 0x02, 0};
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

void toggleLEDWithFrequency2(uint16_t toggleFrequency, uint16_t duration_ms) {
	   uint8_t blinkingLED[3] = {0x20, 0x24, 0};
	    uint8_t leds[3] = {0x20, 0x04, 0};
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


void blink(void){

	while(1)
	        {

	    if (HAL_GPIO_ReadPin(GPIOB ,PL2_Switch_Pin) == GPIO_PIN_RESET)
	    {
	      // Debounce the button
	      HAL_Delay(50);
	      while (HAL_GPIO_ReadPin(GPIOB ,PL2_Switch_Pin) == GPIO_PIN_RESET)
	        ;

	       toggleLEDWithFrequency(200,5000);
	        }


	        }
}
