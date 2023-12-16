/*
 * traffic.h
 *
 *  Created on: Dec 7, 2023
 *      Author: viktorb
 */

#ifndef INC_TRAFFIC_H_
#define INC_TRAFFIC_H_



#endif /* INC_TRAFFIC_H_ */


void traffic(void);


//void toggleLEDWithFrequency(uint16_t toggleFrequency, uint16_t duration_ms);
//void toggleLEDWithFrequency2(uint16_t toggleFrequency, uint16_t duration_ms);
void addLights(uint8_t result[3], const uint8_t array1[3], const uint8_t array2[3]);
void toggleLEDWithFrequency(uint16_t toggleFrequency, uint16_t duration_ms,  uint8_t blinkingLED[3], uint8_t staticleds[3]);
