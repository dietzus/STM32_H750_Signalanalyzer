/*
 * adc.h
 *
 *  Created on: Jan 6, 2024
 *      Author: Martin
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#define ADCDEBUG 1

#define MAXADC 3
#define MAXADCCH 6

typedef struct {
	uint8_t active;
	uint8_t nrchannels;
	ADC_HandleTypeDef adchan;
	uint16_t buffer[10000];
	uint32_t bufsize;
} adc_t;

#endif /* INC_ADC_H_ */
