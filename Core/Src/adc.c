/*
 * adc.c
 *
 *  Created on: Jan 6, 2024
 *      Author: Martin
 */

#include "adc.h"

uint8_t adc_Init(adc_t adc, ADC_HandleTypeDef newadc, uint16_t *buffer, uint32_t bufsize) {
	adc.active = 0;
	adc.adchan = newadc;
	adc.buffer = buffer;
	adc.bufsize = bufsize;

	adc.nrchannels = 1;

	adc.filter.isInit = 0;
	adc_InitFilter(adc.filter, 0, 0);
	return 0;
}

uint8_t adc_InitFilter(adcfilter_t filter, uint32_t average, uint32_t median) {
	if(filter.isInit == 0) {
		filter.average = 0;
		filter.useAvg = 0;
		filter.median = 0;
		filter.useMedian = 0;
	}
	uint8_t useAvg, useMedian = 0;

	if(average != 0) useAvg = 1;
	if(median != 0) useMedian = 1;

	if(useAvg && useMedian && ((average+1) >= median)) return 1;

	if((average%2) != useAvg || (median%2) != useMedian) return 2;

	filter.useAvg = useAvg;
	filter.average = average;
	filter.useMedian = useMedian;
	filter.median = median;
	filter.isInit = 1;
	return 0;
}

uint8_t adc_setADC(adc_t adc, ADC_HandleTypeDef newadc) {
	adc.adchan = newadc;
	return 0;
}

#if ADCDEBUG==1
uint8_t adc_runDebugTests() {



	return 0;
}
#endif
