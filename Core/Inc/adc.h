/*
 * adc.h
 *
 *  Created on: Jan 6, 2024
 *      Author: Martin
 */

#include "main.h"

#ifndef INC_ADC_H_
#define INC_ADC_H_

#define ADCDEBUG 1

#define MAXADC 3
#define MAXADCCH 6

typedef struct {
	uint8_t isInit;
	uint8_t useAvg;
	uint32_t average;
	uint8_t useMedian;
	uint32_t median;
} adcfilter_t;

typedef struct {
	uint8_t active;
	uint8_t nrchannels;
	ADC_HandleTypeDef adchan;
	uint16_t *buffer;
	uint32_t bufsize;
	adcfilter_t filter;
} adc_t;

uint8_t adc_Init(adc_t, ADC_HandleTypeDef, uint16_t *, uint32_t);
uint8_t adc_InitFilter(adcfilter_t filter, uint32_t average, uint32_t median);

uint8_t adc_setADC(adc_t adc, ADC_HandleTypeDef newadc);

#if DEBUGTESTS==1
#if ADCDEBUG==1
uint8_t adc_runDebugTests();
#endif
#endif

#endif /* INC_ADC_H_ */
