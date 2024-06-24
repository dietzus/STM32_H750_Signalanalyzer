/*
 * filters.c
 *
 *  Created on: Jan 15, 2024
 *      Author: Martin
 */

#include "filters.h"

#include <stdlib.h>

filter_t *filter;
uint8_t nrfilter = 0;

uint8_t initAvgFilter(filter_t *filter, uint32_t nrAvg) {
	filter->type = AVERAGE;
	filter->averages = nrAvg;
	filter->medians = 0;
	filter->order = 0;
	filter->coef = 0;
	initFilterBuffer(filter, (uint16_t)0, (uint32_t)0);

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

uint8_t initFilterBuffer(filter_t *filter, uint16_t *buffer, uint32_t bufsize) {
	if(bufsize > 0) {
		filter->inPlace = 1;
		filter->buffer = buffer;
		filter->bufsize = bufsize;
		return 0;
	}
	filter->inPlace = 0;
	filter->buffer = 0;
	filter->bufsize = 0;
	return 0;
}

int32_t addInternalFilter(filter_t filter) {

	return -1;
}

uint8_t applyFilter(filter_t filter, uint16_t *values, uint32_t nrvalues) {
	if(!filter.inPlace) {
		filter.buffer = values;
	}
	uint16_t *tempvalues;

	switch(filter.type) {
	case(AVERAGE):
		tempvalues = malloc(filter.averages * sizeof(uint16_t));
		uint32_t curBuf = 0;
		uint32_t newestValue = 0;
		uint32_t avgValue = 0;

		for(uint32_t i=0; i < filter.averages; i++) {
			tempvalues[i] = values[i];
			avgValue += tempvalues[i];
		}
		avgValue = ((float)avgValue) / filter.averages + 0.5;

		for(; newestValue < (filter.averages/2); newestValue++) {
			filter.buffer[newestValue] = avgValue;
		}

		for(; newestValue < nrvalues-(filter.averages/2); newestValue++) {
			tempvalues[curBuf] = values[newestValue];
			avgValue = 0;
			for(uint32_t i=0; i<filter.averages; i++) {
				avgValue += tempvalues[i];
			}
			filter.buffer[newestValue] = avgValue / filter.averages;
			if(++curBuf >= filter.averages) curBuf = 0;
		}

		avgValue = ((float)avgValue) / filter.averages + 0.5;
		for(; newestValue < nrvalues; newestValue++) {
			filter.buffer[newestValue] = avgValue;
		}

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
	free(tempvalues);
	return 0;
}

#if FILTERDEBUGTESTS==1
#define min(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

uint8_t filterdebug_checkWithTolerance(uint32_t a, uint32_t b, uint32_t tolerance) {
	if(max(a, b) - min(a, b) > tolerance) return 1;
	return 0;
}

void filterdebug_fillDebugBuffer(uint16_t *buffer, uint32_t size, uint32_t start, uint32_t stop, uint32_t step) {
	uint32_t tempnum = start;
	for(uint32_t i=0; i<size; i = i+step) {
		buffer[i] = tempnum++;
		if(tempnum > stop) tempnum=start;
	}
}

uint8_t testFilters(uint32_t filtersize) {
	uint16_t *testbuffer = 0;
	if((testbuffer = malloc(filtersize*sizeof(uint16_t))) == 0) return 1;

	uint32_t errorcode = 2;

	uint32_t highestdivider = 1;
	for(uint32_t i=filtersize-1; i>1; i--) {
		if(filtersize%i == 0) {
			highestdivider = i;
			break;
		}
	}

	filter_t avgfilter;
	initAvgFilter(&avgfilter, highestdivider);
	initFilterBuffer(&avgfilter, testbuffer, 0);

	uint32_t tempstart = 0;
	uint32_t tempstop = 0;
	uint32_t tempstep = 1;
	filterdebug_fillDebugBuffer(testbuffer, filtersize, tempstart, tempstop, tempstep);

	applyFilter(avgfilter, testbuffer, filtersize);

	for(uint32_t i=0; i<filtersize; i++) {
		if(testbuffer[i] != 0) {
			return errorcode;
		}
	}
	errorcode++;

	tempstart = 1;
	tempstop = 5;
	tempstep = 1;
	filterdebug_fillDebugBuffer(testbuffer, filtersize, tempstart, tempstop, tempstep);

	applyFilter(avgfilter, testbuffer, filtersize);

	for(uint32_t i=0; i<filtersize; i++) {
		if(filterdebug_checkWithTolerance(testbuffer[i], 3, 2)) {
			return errorcode;
		}
	}
	errorcode++;

	tempstart = 1;
	tempstop = highestdivider;
	tempstep = 1;
	filterdebug_fillDebugBuffer(testbuffer, filtersize, tempstart, tempstop, tempstep);

	applyFilter(avgfilter, testbuffer, filtersize);

	uint32_t comparevalue = highestdivider / 2;
	for(uint32_t i=0; i<filtersize; i++) {
		if(filterdebug_checkWithTolerance(testbuffer[i], comparevalue, 1)) {
			__attribute__((unused)) uint32_t tempvalue = testbuffer[i];
			return errorcode;
		}
	}
	errorcode++;

	tempstart = UINT16_MAX;
	tempstop = UINT16_MAX;
	tempstep = 1;
	filterdebug_fillDebugBuffer(testbuffer, filtersize, tempstart, tempstop, tempstep);

	applyFilter(avgfilter, testbuffer, filtersize);

	comparevalue = UINT16_MAX;
	for(uint32_t i=0; i<filtersize; i++) {
		if(testbuffer[i] != comparevalue) {
			return errorcode;
		}
	}
	errorcode++;

	return 0;
}
#endif
