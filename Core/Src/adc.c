/*
 * adc.c
 *
 *  Created on: Jan 6, 2024
 *      Author: Martin
 */

#include "adc.h"

adc_t *curadc;

void adc_halfcallback(struct __DMA_HandleTypeDef *adc) {

	for(uint32_t i=0; i<curadc->bufsize; i++)


	return;
}

void adc_callback(DMA_HandleTypeDef *adc) {




	return;
}



uint8_t adc_Init(adc_t adc, ADC_HandleTypeDef newadc, uint16_t *buffer, uint32_t bufsize) {
	adc.active = 0;
	adc.adchan = newadc;
	adc.buffer = buffer;
	adc.bufsize = bufsize;
	adc.startbuffer = buffer;
	adc.halfbuffer = &buffer[bufsize];

	adc.nrchannels = 1;

	initAvgFilter(&adc.filter, 1);

	curadc = &adc;

	return 0;
}

uint8_t adc_setADC(adc_t adc, ADC_HandleTypeDef newadc) {
	adc.adchan = newadc;
	return 0;
}

ADC_HandleTypeDef adc_getADC(adc_t adc) {
	return adc.adchan;
}

uint8_t adc_setFilter(adc_t adc, filter_t newfilter) {
	adc.filter = newfilter;
	return 0;
}

filter_t adc_getFilter(adc_t adc) {
	return adc.filter;
}

uint8_t adc_startDMA(adc_t adc) {
	if( adc.active) {
		HAL_ADC_Stop_DMA(&adc.adchan);
		adc.active = 0;
	}

	__HAL_DMA_ENABLE_IT(&adc.adchan, DMA_IT_HT);
	adc.adchan.DMA_Handle->XferHalfCpltCallback = adc_halfcallback;
	adc.adchan.DMA_Handle->XferCpltCallback = adc_callback;
	HAL_ADC_Start_DMA(&adc.adchan, (uint32_t*)adc.buffer, adc.bufsize * 2);

	adc.active = 1;
	return 0;
}

#if ADCDEBUG==1
uint8_t adc_runDebugTests() {



	return 0;
}
#endif
