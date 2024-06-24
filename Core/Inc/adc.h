/*
 * adc.h
 *
 *  Created on: Jan 6, 2024
 *      Author: Martin
 */


#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "main.h"

#include "filters.h"

#define ADCDEBUG 1

#define MAXADC 3
#define MAXADCCH 6

typedef struct {
	uint8_t active;
	uint8_t nrchannels;
	ADC_HandleTypeDef adchan;
	uint16_t *buffer;
	uint16_t *startbuffer;
	uint16_t *halfbuffer;
	uint32_t bufsize;
	filter_t filter;
	uint32_t curValue;
} adc_t;

uint8_t adc_Init(adc_t, ADC_HandleTypeDef, uint16_t *, uint32_t);

uint8_t adc_setADC(adc_t, ADC_HandleTypeDef);
ADC_HandleTypeDef adc_getADC(adc_t adc);
uint8_t adc_setFilter(adc_t adc, filter_t newfilter);
filter_t adc_getFilter(adc_t adc);
uint8_t adc_setDoubleBuffer(adc_t adc, uint8_t newState);
uint8_t adc_getDoubleBuffer(adc_t adc);
uint8_t adc_startDMA(adc_t adc);

#if DEBUGTESTS==1
#if ADCDEBUG==1
uint8_t adc_runDebugTests();
#endif
#endif

#endif /* INC_ADC_H_ */
