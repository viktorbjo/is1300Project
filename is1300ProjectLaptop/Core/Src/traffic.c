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
	BlinkingWhileOrange,
	PedestrianWalk,
	OrangeForCars
} states;

static states State, NextState;

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
					    uint8_t startLeds[3] = {0x20, 0x0C, 0};
					    uint8_t blinkingLED[3] = {0x20, 0x2C, 0};

					    ShiftLED(startLeds, 3);

					    if (HAL_GPIO_ReadPin(GPIOB ,PL2_Switch_Pin) == GPIO_PIN_RESET)
					    	    {
					    		//ShiftLED(ledsOff,3);
					    	toggleLEDWithFrequency(toggleFreq, (pedestrianDelay-orangeDelay),blinkingLED, startLeds);
					    	      NextState = BlinkingWhileOrange;
					    	        }

					break;

					case BlinkingWhileOrange:
					{

							uint8_t blinkingLED[3] = {0x10, 0x2A, 0};
							uint8_t leds[3] = {0x10, 0x0A, 0};

							toggleLEDWithFrequency(toggleFreq, orangeDelay, blinkingLED, leds);

							NextState = PedestrianWalk;

					}
					break;
					case PedestrianWalk:
					{
						uint8_t herrgoman[3] = {0x08, 0x11, 0};

							ShiftLED(herrgoman,3);
							HAL_Delay(walkingDelay);
							NextState = OrangeForCars;

					}
					break;
					case OrangeForCars:
					{
						uint8_t orange[3] = {0x10, 0x0A, 0};

							ShiftLED(orange,3);
							HAL_Delay(orangeDelay);
							NextState = Start;

					}
					default:
					break;
					}
				}
			}
