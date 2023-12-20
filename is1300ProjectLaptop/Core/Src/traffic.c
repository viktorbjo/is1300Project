/**
******************************************************************************
@brief main file for traffic program with state machine
@file traffic.c
@author Viktor Björkén
@version 1.0
@date December-2023
@brief State machine for traffic-program
******************************************************************************
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


//all Varibles that are stated in the assignment
uint16_t toggleFreq = 200;
uint16_t pedestrianDelay = 5000;
uint16_t walkingDelay = 5000;
uint16_t orangeDelay = 2000;

/**
 @brief Implements the main traffic light control logic.
 */
void traffic(void)
{
	State = Start;
	NextState = Start;

	while(1){

		State = NextState;
				switch (State) // State machine
				{
					case Start:
					    uint8_t startLeds[3] = {0x20, 0x0C, 0}; //Bits for PL=red and TL=green
					    uint8_t blinkingLED[3] = {0x20, 0x2C, 0}; //Keeping the same bits on but also blue indicator

					    ShiftREG(startLeds, 3);

					    if (HAL_GPIO_ReadPin(GPIOB ,PL2_Switch_Pin) == GPIO_PIN_RESET)
					    	    {
					    	// when pressing button first blinking with same leds before changing state
					    	toggleLEDWithFrequency(toggleFreq, (pedestrianDelay-orangeDelay),blinkingLED, startLeds);
					    	      NextState = BlinkingWhileOrange;
					    	        }

					break;

					case BlinkingWhileOrange: //next state
					{

							uint8_t blinkingLED[3] = {0x10, 0x2A, 0}; //same logic as above, bur rather TL=Orange
							uint8_t leds[3] = {0x10, 0x0A, 0};

							toggleLEDWithFrequency(toggleFreq, orangeDelay, blinkingLED, leds);

							NextState = PedestrianWalk;

					}
					break;
					case PedestrianWalk: //next state
					{
						uint8_t herrgoman[3] = {0x08, 0x11, 0}; //green for PL, red for TL

							ShiftREG(herrgoman,3);
							HAL_Delay(walkingDelay); //keeping lights in this stage for walkingDelay ms
							NextState = OrangeForCars;

					}
					break;
					case OrangeForCars: // next state, same logic as above but different leds.
					{
						uint8_t orange[3] = {0x10, 0x0A, 0};

							ShiftREG(orange,3);
							HAL_Delay(orangeDelay);
							NextState = Start; // go back to start

					}
					default:
					break;
					}
				}
			}
