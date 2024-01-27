/*
 * filters.h
 *
 *  Created on: Jan 15, 2024
 *      Author: Martin
 */

#ifndef INC_FILTERS_H_
#define INC_FILTERS_H_

#include "main.h"

#if DEBUGTESTS==1
#define FILTERDEBUGTESTS 1
#endif

typedef enum {
    AVERAGE,
    MEDIAN,
    AVGMEDIAN,
    FIR,
	IIR,
	NRFILTERS
} FilterType;

typedef struct {
	FilterType type;
	uint32_t averages;
	uint32_t medians;
	uint8_t order;
	float *coef;
	uint8_t inPlace;
	uint16_t *buffer;
	uint32_t bufsize;
} filter_t;



uint8_t initFilterBuffer(filter_t *, uint16_t *, uint32_t);

#if FILTERDEBUGTESTS==1
uint8_t testFilters(uint32_t);
#endif

#endif /* INC_FILTERS_H_ */
