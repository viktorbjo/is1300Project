/**
******************************************************************************
@brief  Testing functions for traffic light project
@file test.c
@author Viktor Björkén
@version 1.0
@date December-2023
@brief Testing functions for traffic light project
******************************************************************************
*/
#include <stdint.h>
#include "stdbool.h"
#include "test.h"
#include "traffic.h"
#include "gpio.h"
#include "spi.h"

void Test_program(void)
{
	//testSwitches();
	//ButtonTest();
	//testPL();
	blink();
	//toggleBlink();

}

/**
@brief shiftOut, shifts value into SR to turn LED
@param uint8_t buffer[] the buffer of bits that represent a LED
@param uint8_t regs, the size of the amount of data
@return void
*/
void shiftOut(uint8_t buffer[], uint8_t regs)
{
	//HAL_GPIO_WritePin(Reset_595_GPIO_Port, Reset_595_Pin, GPIO_PIN_SET);   // Set MR high (active low)
	//HAL_GPIO_WritePin(Enable_595_GPIO_Port, Enable_595_Pin, GPIO_PIN_RESET); // Reset OE low (active low)
	//HAL_Delay(1);
	HAL_SPI_Transmit(&hspi3, &buffer[0], regs, 100);
	HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(STCP_595_GPIO_Port, STCP_595_Pin, GPIO_PIN_SET);
	HAL_Delay(1);

}



/**
@brief testSwitches, tests the switches, turn on light if switch is on.
@param void
@return void
*/
void testSwitches(void){
	uint8_t setLED[3] = { 0,0x07,0 };
	uint8_t testLED[3] = { 0,0x38,0 };
	uint8_t resetLED[3] = {0, 0x00,0 };
 while(1){
	if(HAL_GPIO_ReadPin(TL2_Car_GPIO_Port ,TL2_Car_Pin) == GPIO_PIN_SET)
		shiftOut(setLED, 3);

	if(HAL_GPIO_ReadPin(TL4_Car_GPIO_Port ,TL4_Car_Pin) == GPIO_PIN_SET)
			shiftOut(testLED, 3);

	HAL_Delay(1);
	shiftOut(resetLED, 3);
 }
}

/**
@brief ButtonTest, Initial tests the pedestrian buttons.
@param void
@return void
*/
void ButtonTest(void){
    uint8_t ledBuffer[3] = {0,0,0};  // Initialize to turn off all LEDs
    //uint8_t currentLED = 0;
    uint8_t leds4[3] = { 0x38, 0x3F, 0 };
    //uint8_t led5[2] = {0x07};

    while(1)
        {

            if(HAL_GPIO_ReadPin(GPIOB ,PL2_Switch_Pin) == GPIO_PIN_RESET)
                shiftOut(leds4, 3);

            HAL_Delay(1);
            shiftOut(ledBuffer, 3);
        }

    }

/**
@brief testPL, improved test for buttons but also for all leds
@param void
@return void
*/
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
      shiftOut(ledBuffer, 3);
    }
  }
}

/**
@brief blink,test if blinking works when pressing button
@param void
@return void
*/
void blink(void){

	while(1)
	        {

	    if (HAL_GPIO_ReadPin(GPIOB ,PL2_Switch_Pin) == GPIO_PIN_RESET)
	    {
	      // Debounce the button
	      HAL_Delay(50);
	      while (HAL_GPIO_ReadPin(GPIOB ,PL2_Switch_Pin) == GPIO_PIN_RESET)
	        ;

	      	  toggleBlink();
	        }
	    }
}

/**
@brief toggleBlink, turns led on and off while keeping some light on all the time
@param void
@return void
*/
void toggleBlink(void) {
	    uint8_t ledState = 0; // 0 for OFF, 1 for ON
	    uint8_t staticleds[3] = {0x08, 0x01, 0};
	    uint8_t blinkingLED[3] = {0x08, 0x21, 0};

	    while (1) {

	        ledState = !ledState;

	        if (ledState) {
	            ShiftREG(blinkingLED, 3);  // Turn on the LED
	        } else {

	            ShiftREG(staticleds, 3);
	        }

	        HAL_Delay(500);
	    }
	}
