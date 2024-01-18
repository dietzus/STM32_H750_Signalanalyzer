/*
 * filters.h
 *
 *  Created on: Jan 15, 2024
 *      Author: Martin
 */

#ifndef INC_FILTERS_H_
#define INC_FILTERS_H_

#include "main.h"

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
	float *buffer;
	uint32_t bufsize;
} filter_t;



uint8_t initFilterBuffer(filter_t, float *, uint32_t);

#endif /* INC_FILTERS_H_ */
