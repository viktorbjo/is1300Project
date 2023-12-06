/* test.c
 *
 *  Created on: Nov 29, 2023
 *      Author: vikto
 */
#include <stdint.h>
#include "stdbool.h"
#include "test.h"
#include "gpio.h"
#include "spi.h"

uint8_t dataout[4] = {0x00, 0xf0, 0x0f, 0xff};

#define NUM_LEDS 8
//uint8_t leds1[] = { 0x07 };

void Test_program(void)
{
	testButtons();
	//testPL();
}

/*
void shiftOut(uint8_t * data, uint16_t size){

	HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_RESET);

	HAL_SPI_Transmit(&hspi2, data, size, 100);

	HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_SET);
}
*/

void ShiftLED(uint8_t ledbuffer[], uint8_t bytesize)
{
	//HAL_GPIO_WritePin(Reset_595_GPIO_Port, Reset_595_Pin, GPIO_PIN_SET);   // Set MR high (active low)
	//HAL_GPIO_WritePin(Enable_595_GPIO_Port, Enable_595_Pin, GPIO_PIN_RESET); // Reset OE low (active low)

	//HAL_Delay(1);

	HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit(&hspi3, &ledbuffer[0], bytesize, 100);

	HAL_Delay(1);

	HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_SET);
	HAL_Delay(1);

}

void Test_ShiftLeds(void)
{
	uint8_t ledBuffer[NUM_LEDS];

	    // Turn on each LED one by one and test
	    for (uint8_t i = 0; i < NUM_LEDS; ++i) {
	        // Reset the buffer
	        for (uint8_t j = 0; j < NUM_LEDS; ++j) {
	            ledBuffer[j] = 0x00;  // Assuming 0x00 means LED off
	        }

	        // Set the current LED to ON
	        ledBuffer[i] = 0xFF;  // Assuming 0xFF means LED on

	        // Shift the LED buffer to the shift register
	        ShiftLED(ledBuffer, NUM_LEDS);

	        // Add a delay here if necessary to observe the LED state
	    }
	}

void testButtons(void){
	uint8_t setLED[] = { 0x07 };
	uint8_t testLED[] = { 0x0F };
	uint8_t resetLED[] = { 0x00 };
 while(1){
	if(HAL_GPIO_ReadPin(TL2_Car_GPIO_Port ,TL2_Car_Pin) == GPIO_PIN_SET)
		ShiftLED(setLED, 3);

	if(HAL_GPIO_ReadPin(TL4_Car_GPIO_Port ,TL4_Car_Pin) == GPIO_PIN_SET)
			ShiftLED(testLED, 3);

	HAL_Delay(1);
	ShiftLED(resetLED, 3);
 }
}

//funkar ej
void testPL(void){
	uint8_t ledBuffer[3] = {0, 0, 0};  // Initialize to turn off all LEDs
	uint8_t currentLED = 0;

while (1)
  {
    // Check if the button is pressed
    if (HAL_GPIO_ReadPin(GPIOB ,PL2_Switch_Pin) == GPIO_PIN_RESET)
    {
      // Debounce the button
      HAL_Delay(50);
      while (HAL_GPIO_ReadPin(GPIOB ,PL2_Switch_Pin) == GPIO_PIN_RESET)
        ;

      // Shift to the next LED
      currentLED = (currentLED + 1) % (3 * 8);

      // Update the LED buffer based on the current LED state
      for (int i = 0; i < 3; i++)
      {
        ledBuffer[i] = (currentLED >= (i * 8) && currentLED < ((i + 1) * 8)) ? (1 << (currentLED % 8)) : 0x00;
      }

      // Shift the updated LED buffer to the shift registers
      ShiftLED(ledBuffer, 3);
    }
  }
}
