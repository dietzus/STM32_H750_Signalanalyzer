/*
 * filters.c
 *
 *  Created on: Jan 15, 2024
 *      Author: Martin
 */

#include "filters.h"

filter_t *filter;
uint8_t nrfilter = 0;

uint8_t initAvgFilter(filter_t filter, uint32_t nrAvg) {
	filter.type = AVERAGE;
	filter.averages = nrAvg;
	filter.medians = 0;
	filter.order = 0;
	filter.coef = 0;
	initFilterBuffer(filter, filter.coef, (uint32_t)0);

	return 1;
}

uint8_t initMedFilter(filter_t filter, uint32_t nrMed) {
	filter.type = MEDIAN;

	return 1;
}

uint8_t initAvgMedFilter(filter_t filter, uint32_t nrAvg, uint32_t nrMed) {
	filter.type = AVGMEDIAN;

	return 1;
}

uint8_t initFIRFilter(filter_t filter, uint8_t order, float *coef) {
	filter.type = FIR;

	return 1;
}

uint8_t initIIRFilter(filter_t filter, uint8_t order, float *coef) {
	filter.type = IIR;

	return 1;
}

uint8_t initFilterBuffer(filter_t filter, float *buffer, uint32_t bufsize) {
	if(bufsize > 0) {
		filter.inPlace = 1;
		filter.buffer = buffer;
		filter.bufsize = bufsize;
		return 0;
	}
	filter.inPlace = 0;
	filter.buffer = 0;
	filter.bufsize = 0;
	return 0;
}

int32_t addInternalFilter(filter_t) {

	return -1;
}

uint8_t applyFilter(filter_t filter, uint16_t *values, uint32_t nrvalues) {
	uint32_t *tempvalues;

	switch(filter.type) {
	case(AVERAGE):
		tempvalues = malloc(filter.averages)

		break;
	case(MEDIAN):

		break;
	case(AVGMEDIAN):

		break;
	case(FIR):

		break;
	case(IIR):

		break;
	default:
		return 1;
	}
}
